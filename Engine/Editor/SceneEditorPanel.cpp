#include "Editor/SceneEditorPanel.h"

#include <cstdio>
#include <string>

#include <imgui.h>

#include "Math/Transform2D.h"
#include "Scene/Scene.h"

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

    if (!scene.IsValidGameObject(m_InspectorObjectID))
    {
        m_NameBuffer[0] = '\0';
        m_TexturePathBuffer[0] = '\0';
        m_AnimationSetPathBuffer[0] = '\0';
        m_AnimationClipBuffer[0] = '\0';
        m_ReparentTargetID = 0;
        m_SourceRectValues[0] = 0;
        m_SourceRectValues[1] = 0;
        m_SourceRectValues[2] = 0;
        m_SourceRectValues[3] = 0;
        return;
    }

    const Entity selected = scene.GetEntity(m_InspectorObjectID);

    std::snprintf(m_NameBuffer.data(), m_NameBuffer.size(), "%s", selected.GetName().c_str());
    std::snprintf(
        m_TexturePathBuffer.data(),
        m_TexturePathBuffer.size(),
        "%s",
        selected.GetSpriteTexturePath().c_str()
    );
    std::snprintf(
        m_AnimationSetPathBuffer.data(),
        m_AnimationSetPathBuffer.size(),
        "%s",
        selected.GetAnimationSetPath().c_str()
    );
    std::snprintf(
        m_AnimationClipBuffer.data(),
        m_AnimationClipBuffer.size(),
        "%s",
        selected.GetAnimationClipName().c_str()
    );

    const IntRect sourceRect = selected.GetSpriteSourceRect();
    m_SourceRectValues[0] = sourceRect.x;
    m_SourceRectValues[1] = sourceRect.y;
    m_SourceRectValues[2] = sourceRect.width;
    m_SourceRectValues[3] = sourceRect.height;
    m_ReparentTargetID = selected.HasParent()
        ? static_cast<int>(selected.GetParentID())
        : 0;
}

void SceneEditorPanel::DrawHierarchyPanel(Scene& scene)
{
    ImGui::Begin("Hierarchy");

    const std::vector<GameObjectID> roots = scene.GetRootGameObjects();
    for (GameObjectID id : roots)
        DrawHierarchyNode(scene, id);

    ImGui::End();
}

void SceneEditorPanel::DrawHierarchyNode(Scene& scene, GameObjectID id)
{
    const bool isSelected = (m_SelectedObjectID == id);
    const bool hasChildren = scene.HasChildren(id);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (!hasChildren)
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;

    const std::string label = scene.GetGameObjectName(id) + "##" + std::to_string(id);

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
        const std::vector<GameObjectID> children = scene.GetChildGameObjects(id);
        for (GameObjectID childID : children)
            DrawHierarchyNode(scene, childID);

        ImGui::TreePop();
    }
}

void SceneEditorPanel::DrawInspectorPanel(Scene& scene)
{
    ImGui::Begin("Inspector");

    if (!scene.IsValidGameObject(m_SelectedObjectID))
    {
        if (m_SelectedObjectID != 0)
            SelectObject(scene, 0);

        ImGui::TextUnformatted("No object selected.");
        ImGui::End();
        return;
    }

    if (m_InspectorObjectID != m_SelectedObjectID)
        SyncInspectorFromSelection(scene);

    const GameObjectID selectedID = m_SelectedObjectID;
    Entity selected = scene.GetEntity(selectedID);

    ImGui::Text("Selected: %s", selected.GetName().c_str());
    ImGui::Text("ID: %llu", static_cast<unsigned long long>(selectedID));
    ImGui::Separator();

    if (ImGui::InputText("Name", m_NameBuffer.data(), m_NameBuffer.size()))
        selected.SetName(m_NameBuffer.data());

    bool active = selected.IsActive();
    if (ImGui::Checkbox("Active", &active))
        selected.SetActive(active);

    if (ImGui::Button("Destroy Object"))
    {
        selected.Destroy();
        SelectObject(scene, 0);
        ImGui::End();
        return;
    }

    Transform2D localTransform = selected.GetLocalTransform();
    bool localChanged = false;
    localChanged |= ImGui::DragFloat2("Local Position", &localTransform.position.x, 1.0f);
    localChanged |= ImGui::DragFloat2("Local Scale", &localTransform.scale.x, 0.01f, 0.0f, 1000.0f);
    localChanged |= ImGui::DragFloat2("Local Pivot", &localTransform.pivot.x, 0.01f, 0.0f, 1.0f);
    localChanged |= ImGui::DragFloat("Local Rotation", &localTransform.rotationDegrees, 1.0f);

    if (localChanged)
        selected.SetLocalTransform(localTransform);

    Transform2D worldTransform = selected.GetTransform();
    bool worldChanged = false;

    ImGui::SeparatorText("Transform");
    worldChanged |= ImGui::DragFloat2("Position", &worldTransform.position.x, 1.0f);
    worldChanged |= ImGui::DragFloat2("Scale", &worldTransform.scale.x, 0.01f, 0.0f, 1000.0f);
    worldChanged |= ImGui::DragFloat2("Pivot", &worldTransform.pivot.x, 0.01f, 0.0f, 1.0f);
    worldChanged |= ImGui::DragFloat("Rotation", &worldTransform.rotationDegrees, 1.0f);

    if (worldChanged)
        selected.SetTransform(worldTransform);

    ImGui::SeparatorText("Sprite");

    if (ImGui::InputText("Texture Path", m_TexturePathBuffer.data(), m_TexturePathBuffer.size()))
        selected.SetSpriteTexturePath(m_TexturePathBuffer.data());

    glm::vec2 spriteSize = selected.GetSpriteSize();
    if (ImGui::DragFloat2("Sprite Size", &spriteSize.x, 1.0f, 0.0f, 4096.0f))
        selected.SetSpriteSize(spriteSize);

    glm::vec4 tint = selected.GetSpriteTint();
    if (ImGui::ColorEdit4("Tint", &tint.x))
        selected.SetSpriteTint(tint);

    int layer = selected.GetSpriteLayer();
    if (ImGui::DragInt("Layer", &layer, 1.0f))
        selected.SetSpriteLayer(layer);

    bool visible = selected.IsSpriteVisible();
    if (ImGui::Checkbox("Visible", &visible))
        selected.SetSpriteVisible(visible);

    bool flipX = selected.IsSpriteFlippedX();
    if (ImGui::Checkbox("Flip X", &flipX))
        selected.SetSpriteFlipX(flipX);

    bool flipY = selected.IsSpriteFlippedY();
    if (ImGui::Checkbox("Flip Y", &flipY))
        selected.SetSpriteFlipY(flipY);

    bool usesSourceRect = selected.SpriteUsesSourceRect();
    if (ImGui::Checkbox("Use Source Rect", &usesSourceRect))
    {
        if (usesSourceRect)
        {
            selected.SetSpriteSourceRect(
                m_SourceRectValues[0],
                m_SourceRectValues[1],
                m_SourceRectValues[2],
                m_SourceRectValues[3]
            );
        }
        else
        {
            selected.ClearSpriteSourceRect();
        }
    }

    if (usesSourceRect && ImGui::InputInt4("Source Rect", m_SourceRectValues))
    {
        selected.SetSpriteSourceRect(
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
        selected.SetSpriteSourceRectFromGrid(
            m_SourceGridValues[0],
            m_SourceGridValues[1],
            m_SourceGridValues[2],
            m_SourceGridValues[3]
        );

        const IntRect sourceRect = selected.GetSpriteSourceRect();
        m_SourceRectValues[0] = sourceRect.x;
        m_SourceRectValues[1] = sourceRect.y;
        m_SourceRectValues[2] = sourceRect.width;
        m_SourceRectValues[3] = sourceRect.height;
    }

    ImGui::SeparatorText("Animation");

    bool hasAnimation = selected.HasAnimation();
    if (ImGui::Checkbox("Has Animation", &hasAnimation))
    {
        if (hasAnimation)
            selected.EnsureAnimation();
        else
            selected.RemoveAnimation();
    }

    if (hasAnimation)
    {
        if (ImGui::InputText("Animation Set Path", m_AnimationSetPathBuffer.data(), m_AnimationSetPathBuffer.size()))
            selected.SetAnimationSetPath(m_AnimationSetPathBuffer.data());

        ImGui::InputText("Clip Name", m_AnimationClipBuffer.data(), m_AnimationClipBuffer.size());

        if (ImGui::Button("Play Clip"))
            selected.PlayAnimation(m_AnimationClipBuffer.data(), true);

        ImGui::SameLine();

        if (ImGui::Button("Stop Clip"))
            selected.StopAnimation();

        ImGui::SameLine();

        if (ImGui::Button("Reset Clip"))
            selected.ResetAnimation();

        ImGui::Text("Playing: %s", selected.IsAnimationPlaying() ? "Yes" : "No");
        ImGui::Text("Finished: %s", selected.HasAnimationFinished() ? "Yes" : "No");
    }

    ImGui::SeparatorText("Hierarchy");

    if (selected.HasParent())
    {
        const GameObjectID parentID = selected.GetParentID();
        if (scene.IsValidGameObject(parentID))
        {
            ImGui::Text(
                "Parent: %s (%llu)",
                scene.GetGameObjectName(parentID).c_str(),
                static_cast<unsigned long long>(parentID)
            );
        }
        else
        {
            ImGui::Text(
                "Parent: Missing (%llu)",
                static_cast<unsigned long long>(parentID)
            );
        }
    }
    else
    {
        ImGui::TextUnformatted("Parent: None");
    }
    
    int destroyPolicyIndex =
        selected.GetChildDestroyPolicy() == ChildDestroyPolicy::DestroyWithParent ? 1 : 0;

    const char* destroyPolicyLabels[] = { "Detach To Root", "Destroy With Parent" };
    if (ImGui::Combo(
            "On Parent Destroy",
            &destroyPolicyIndex,
            destroyPolicyLabels,
            IM_ARRAYSIZE(destroyPolicyLabels)))
    {
        selected.SetChildDestroyPolicy(
            destroyPolicyIndex == 1
                ? ChildDestroyPolicy::DestroyWithParent
                : ChildDestroyPolicy::DetachToRoot
        );
    }

    std::string comboLabel = "None";
    if (m_ReparentTargetID != 0)
        comboLabel = std::to_string(m_ReparentTargetID);

    if (ImGui::BeginCombo("Reparent To", comboLabel.c_str()))
    {
        const bool noParentSelected = (m_ReparentTargetID == 0);
        if (ImGui::Selectable("None", noParentSelected))
            m_ReparentTargetID = 0;

        for (GameObjectID candidateID : scene.GetGameObjectIDs())
        {
            if (candidateID == selectedID)
                continue;

            const bool candidateSelected = (m_ReparentTargetID == static_cast<int>(candidateID));
            const std::string candidateLabel =
                scene.GetGameObjectName(candidateID) + " (" + std::to_string(candidateID) + ")";

            if (ImGui::Selectable(candidateLabel.c_str(), candidateSelected))
                m_ReparentTargetID = static_cast<int>(candidateID);
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Set Parent"))
    {
        if (m_ReparentTargetID == 0)
            selected.ClearParent();
        else
            selected.SetParent(static_cast<GameObjectID>(m_ReparentTargetID));
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Parent"))
    {
        selected.ClearParent();
        m_ReparentTargetID = 0;
    }

    ImGui::End();
}
