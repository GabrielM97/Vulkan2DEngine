#include "Editor/SceneEditorPanel.h"

#include "Math/Transform2D.h"
#include "Scene/Scene.h"

#include <imgui.h>
#include <cstdio>
#include <string>

void SceneEditorPanel::Draw(Scene& scene)
{
    DrawHierarchyPanel(scene);
    DrawInspectorPanel(scene);
}

void SceneEditorPanel::SelectObject(Scene& scene, GameObjectID id)
{
    m_SelectedObjectID = id;
    SyncInspectorFromSelection(scene);
}

void SceneEditorPanel::SyncInspectorFromSelection(Scene& scene)
{
    m_InspectorObjectID = m_SelectedObjectID;

    GameObject* selected = scene.FindGameObjectByID(m_InspectorObjectID);
    if (selected == nullptr)
    {
        m_NameBuffer[0] = '\0';
        m_TexturePathBuffer[0] = '\0';
        m_ReparentTargetID = 0;
        m_SourceRectValues[0] = 0;
        m_SourceRectValues[1] = 0;
        m_SourceRectValues[2] = 0;
        m_SourceRectValues[3] = 0;
        return;
    }

    std::snprintf(m_NameBuffer.data(), m_NameBuffer.size(), "%s", selected->GetName().c_str());
    std::snprintf(
        m_TexturePathBuffer.data(),
        m_TexturePathBuffer.size(),
        "%s",
        scene.GetSpriteTexturePath(m_InspectorObjectID).c_str()
    );

    const IntRect sourceRect = scene.GetSpriteSourceRect(m_InspectorObjectID);
    m_SourceRectValues[0] = sourceRect.x;
    m_SourceRectValues[1] = sourceRect.y;
    m_SourceRectValues[2] = sourceRect.width;
    m_SourceRectValues[3] = sourceRect.height;
    m_ReparentTargetID = selected->HasParent()
        ? static_cast<int>(selected->GetParentID())
        : 0;
}

void SceneEditorPanel::DrawHierarchyPanel(Scene& scene)
{
    ImGui::Begin("Hierarchy");

    const std::vector<const GameObject*> roots = scene.GetRootGameObjects();
    for (const GameObject* object : roots)
    {
        if (object != nullptr)
            DrawHierarchyNode(scene, *object);
    }

    ImGui::End();
}

void SceneEditorPanel::DrawHierarchyNode(Scene& scene, const GameObject& object)
{
    const GameObjectID id = object.GetID();
    const bool isSelected = (m_SelectedObjectID == id);
    const bool hasChildren = scene.HasChildren(id);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (!hasChildren)
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;

    const std::string label = object.GetName() + "##" + std::to_string(id);

    const bool opened = ImGui::TreeNodeEx(
        reinterpret_cast<void*>(static_cast<uintptr_t>(id)),
        flags,
        "%s",
        label.c_str()
    );

    if (ImGui::IsItemClicked())
        SelectObject(scene, id);

    ImGui::SameLine();
    ImGui::TextDisabled("(ID: %llu)", static_cast<unsigned long long>(id));

    if (opened)
    {
        const std::vector<const GameObject*> children = scene.GetChildGameObjects(id);
        for (const GameObject* child : children)
        {
            if (child != nullptr)
                DrawHierarchyNode(scene, *child);
        }

        ImGui::TreePop();
    }
}

void SceneEditorPanel::DrawInspectorPanel(Scene& scene)
{
    ImGui::Begin("Inspector");

    GameObject* selected = scene.FindGameObjectByID(m_SelectedObjectID);
    if (selected == nullptr)
    {
        if (m_SelectedObjectID != 0)
            SelectObject(scene, 0);

        ImGui::TextUnformatted("No object selected.");
        ImGui::End();
        return;
    }

    if (m_InspectorObjectID != m_SelectedObjectID)
        SyncInspectorFromSelection(scene);

    ImGui::Text("Selected: %s", selected->GetName().c_str());
    ImGui::Text("ID: %llu", static_cast<unsigned long long>(selected->GetID()));
    ImGui::Separator();

    if (ImGui::InputText("Name", m_NameBuffer.data(), m_NameBuffer.size()))
        scene.SetGameObjectName(selected->GetID(), m_NameBuffer.data());

    bool active = selected->isActive();
    if (ImGui::Checkbox("Active", &active))
        scene.SetGameObjectActive(selected->GetID(), active);

    Transform2D worldTransform = scene.GetWorldTransform(selected->GetID());
    bool transformChanged = false; 
    
    ImGui::SeparatorText("World Transform");
    transformChanged |= ImGui::DragFloat2("Position", &worldTransform.position.x, 1.0f);
    transformChanged |= ImGui::DragFloat2("Scale", &worldTransform.scale.x, 0.01f, 0.0f, 1000.0f);
    transformChanged |= ImGui::DragFloat2("Pivot", &worldTransform.pivot.x, 0.01f, 0.0f, 1.0f);
    transformChanged |= ImGui::DragFloat("Rotation", &worldTransform.rotationDegrees, 1.0f);

    if (transformChanged)
        scene.SetLocalTransform(selected->GetID(), worldTransform);

    ImGui::SeparatorText("Sprite");

    if (ImGui::InputText("Texture Path", m_TexturePathBuffer.data(), m_TexturePathBuffer.size()))
        scene.SetSpriteTexturePath(selected->GetID(), m_TexturePathBuffer.data());

    glm::vec2 spriteSize = scene.GetSpriteSize(selected->GetID());
    if (ImGui::DragFloat2("Sprite Size", &spriteSize.x, 1.0f, 0.0f, 4096.0f))
        scene.SetSpriteSize(selected->GetID(), spriteSize);

    glm::vec4 tint = scene.GetSpriteTint(selected->GetID());
    if (ImGui::ColorEdit4("Tint", &tint.x))
        scene.SetSpriteTint(selected->GetID(), tint);

    int layer = scene.GetSpriteLayer(selected->GetID());
    if (ImGui::DragInt("Layer", &layer, 1.0f))
        scene.SetSpriteLayer(selected->GetID(), layer);

    bool visible = scene.IsSpriteVisible(selected->GetID());
    if (ImGui::Checkbox("Visible", &visible))
        scene.SetSpriteVisible(selected->GetID(), visible);

    bool flipX = scene.IsSpriteFlippedX(selected->GetID());
    if (ImGui::Checkbox("Flip X", &flipX))
        scene.SetSpriteFlipX(selected->GetID(), flipX);

    bool flipY = scene.IsSpriteFlippedY(selected->GetID());
    if (ImGui::Checkbox("Flip Y", &flipY))
        scene.SetSpriteFlipY(selected->GetID(), flipY);

    bool usesSourceRect = scene.SpriteUsesSourceRect(selected->GetID());
    if (ImGui::Checkbox("Use Source Rect", &usesSourceRect))
    {
        if (usesSourceRect)
        {
            scene.SetSpriteSourceRect(
                selected->GetID(),
                m_SourceRectValues[0],
                m_SourceRectValues[1],
                m_SourceRectValues[2],
                m_SourceRectValues[3]
            );
        }
        else
        {
            scene.ClearSpriteSourceRect(selected->GetID());
        }
    }

    if (usesSourceRect && ImGui::InputInt4("Source Rect", m_SourceRectValues))
    {
        scene.SetSpriteSourceRect(
            selected->GetID(),
            m_SourceRectValues[0],
            m_SourceRectValues[1],
            m_SourceRectValues[2],
            m_SourceRectValues[3]
        );
    }

    ImGui::InputInt("Grid Column", &m_SourceGridValues[0]);
    ImGui::InputInt("Grid Row", &m_SourceGridValues[1]);
    ImGui::InputInt("Cell Width", &m_SourceGridValues[2]);
    ImGui::InputInt("Cell Height", &m_SourceGridValues[3]);

    if (ImGui::Button("Apply Grid Frame"))
    {
        scene.SetSpriteSourceRectFromGrid(
            selected->GetID(),
            m_SourceGridValues[0],
            m_SourceGridValues[1],
            m_SourceGridValues[2],
            m_SourceGridValues[3]
        );

        const IntRect sourceRect = scene.GetSpriteSourceRect(selected->GetID());
        m_SourceRectValues[0] = sourceRect.x;
        m_SourceRectValues[1] = sourceRect.y;
        m_SourceRectValues[2] = sourceRect.width;
        m_SourceRectValues[3] = sourceRect.height;
    }

    ImGui::SeparatorText("Hierarchy");

    if (selected->HasParent())
    {
        const GameObject* parent = scene.FindGameObjectByID(selected->GetParentID());
        if (parent != nullptr)
        {
            ImGui::Text(
                "Parent: %s (%llu)",
                parent->GetName().c_str(),
                static_cast<unsigned long long>(parent->GetID())
            );
        }
        else
        {
            ImGui::Text(
                "Parent: Missing (%llu)",
                static_cast<unsigned long long>(selected->GetParentID())
            );
        }
    }
    else
    {
        ImGui::TextUnformatted("Parent: None");
    }

    std::string comboLabel = "None";
    if (m_ReparentTargetID != 0)
        comboLabel = std::to_string(m_ReparentTargetID);

    if (ImGui::BeginCombo("Reparent To", comboLabel.c_str()))
    {
        const bool noParentSelected = (m_ReparentTargetID == 0);
        if (ImGui::Selectable("None", noParentSelected))
            m_ReparentTargetID = 0;

        for (const auto& objectPtr : scene.GetGameObjects())
        {
            const GameObject& candidate = *objectPtr;
            if (candidate.GetID() == selected->GetID())
                continue;

            const bool candidateSelected = (m_ReparentTargetID == static_cast<int>(candidate.GetID()));
            const std::string candidateLabel =
                candidate.GetName() + " (" + std::to_string(candidate.GetID()) + ")";

            if (ImGui::Selectable(candidateLabel.c_str(), candidateSelected))
                m_ReparentTargetID = static_cast<int>(candidate.GetID());
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Set Parent"))
    {
        if (m_ReparentTargetID == 0)
            scene.ClearParent(selected->GetID());
        else
            scene.SetParent(selected->GetID(), static_cast<GameObjectID>(m_ReparentTargetID));
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Parent"))
    {
        scene.ClearParent(selected->GetID());
        m_ReparentTargetID = 0;
    }

    ImGui::End();
}
