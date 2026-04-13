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

    std::snprintf(m_NameBuffer.data(), m_NameBuffer.size(), "%s", scene.GetGameObjectName(m_InspectorObjectID).c_str());
    std::snprintf(
        m_TexturePathBuffer.data(),
        m_TexturePathBuffer.size(),
        "%s",
        scene.GetSpriteTexturePath(m_InspectorObjectID).c_str()
    );
    std::snprintf(
        m_AnimationSetPathBuffer.data(),
        m_AnimationSetPathBuffer.size(),
        "%s",
        scene.GetAnimationSetPath(m_InspectorObjectID).c_str()
    );
    std::snprintf(
        m_AnimationClipBuffer.data(),
        m_AnimationClipBuffer.size(),
        "%s",
        scene.GetAnimationClipName(m_InspectorObjectID).c_str()
    );

    const IntRect sourceRect = scene.GetSpriteSourceRect(m_InspectorObjectID);
    m_SourceRectValues[0] = sourceRect.x;
    m_SourceRectValues[1] = sourceRect.y;
    m_SourceRectValues[2] = sourceRect.width;
    m_SourceRectValues[3] = sourceRect.height;
    m_ReparentTargetID = scene.HasParent(m_InspectorObjectID)
        ? static_cast<int>(scene.GetParentID(m_InspectorObjectID))
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

    ImGui::Text("Selected: %s", scene.GetGameObjectName(selectedID).c_str());
    ImGui::Text("ID: %llu", static_cast<unsigned long long>(selectedID));
    ImGui::Separator();

    if (ImGui::InputText("Name", m_NameBuffer.data(), m_NameBuffer.size()))
        scene.SetGameObjectName(selectedID, m_NameBuffer.data());

    bool active = scene.IsGameObjectActive(selectedID);
    if (ImGui::Checkbox("Active", &active))
        scene.SetGameObjectActive(selectedID, active);

    if (ImGui::Button("Destroy Object"))
    {
        scene.DestroyGameObject(selectedID);
        SelectObject(scene, 0);
        ImGui::End();
        return;
    }

    Transform2D localTransform = scene.GetLocalTransform(selectedID);
    bool localChanged = false;
    localChanged |= ImGui::DragFloat2("Local Position", &localTransform.position.x, 1.0f);
    localChanged |= ImGui::DragFloat2("Local Scale", &localTransform.scale.x, 0.01f, 0.0f, 1000.0f);
    localChanged |= ImGui::DragFloat2("Local Pivot", &localTransform.pivot.x, 0.01f, 0.0f, 1.0f);
    localChanged |= ImGui::DragFloat("Local Rotation", &localTransform.rotationDegrees, 1.0f);

    if (localChanged)
        scene.SetLocalTransform(selectedID, localTransform);

    Transform2D worldTransform = scene.GetWorldTransform(selectedID);
    bool worldChanged = false;

    ImGui::SeparatorText("World Transform");
    worldChanged |= ImGui::DragFloat2("World Position", &worldTransform.position.x, 1.0f);
    worldChanged |= ImGui::DragFloat2("World Scale", &worldTransform.scale.x, 0.01f, 0.0f, 1000.0f);
    worldChanged |= ImGui::DragFloat2("World Pivot", &worldTransform.pivot.x, 0.01f, 0.0f, 1.0f);
    worldChanged |= ImGui::DragFloat("World Rotation", &worldTransform.rotationDegrees, 1.0f);

    if (worldChanged)
        scene.SetWorldTransform(selectedID, worldTransform);

    ImGui::SeparatorText("Sprite");

    if (ImGui::InputText("Texture Path", m_TexturePathBuffer.data(), m_TexturePathBuffer.size()))
        scene.SetSpriteTexturePath(selectedID, m_TexturePathBuffer.data());

    glm::vec2 spriteSize = scene.GetSpriteSize(selectedID);
    if (ImGui::DragFloat2("Sprite Size", &spriteSize.x, 1.0f, 0.0f, 4096.0f))
        scene.SetSpriteSize(selectedID, spriteSize);

    glm::vec4 tint = scene.GetSpriteTint(selectedID);
    if (ImGui::ColorEdit4("Tint", &tint.x))
        scene.SetSpriteTint(selectedID, tint);

    int layer = scene.GetSpriteLayer(selectedID);
    if (ImGui::DragInt("Layer", &layer, 1.0f))
        scene.SetSpriteLayer(selectedID, layer);

    bool visible = scene.IsSpriteVisible(selectedID);
    if (ImGui::Checkbox("Visible", &visible))
        scene.SetSpriteVisible(selectedID, visible);

    bool flipX = scene.IsSpriteFlippedX(selectedID);
    if (ImGui::Checkbox("Flip X", &flipX))
        scene.SetSpriteFlipX(selectedID, flipX);

    bool flipY = scene.IsSpriteFlippedY(selectedID);
    if (ImGui::Checkbox("Flip Y", &flipY))
        scene.SetSpriteFlipY(selectedID, flipY);

    bool usesSourceRect = scene.SpriteUsesSourceRect(selectedID);
    if (ImGui::Checkbox("Use Source Rect", &usesSourceRect))
    {
        if (usesSourceRect)
        {
            scene.SetSpriteSourceRect(
                selectedID,
                m_SourceRectValues[0],
                m_SourceRectValues[1],
                m_SourceRectValues[2],
                m_SourceRectValues[3]
            );
        }
        else
        {
            scene.ClearSpriteSourceRect(selectedID);
        }
    }

    if (usesSourceRect && ImGui::InputInt4("Source Rect", m_SourceRectValues))
    {
        scene.SetSpriteSourceRect(
            selectedID,
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
            selectedID,
            m_SourceGridValues[0],
            m_SourceGridValues[1],
            m_SourceGridValues[2],
            m_SourceGridValues[3]
        );

        const IntRect sourceRect = scene.GetSpriteSourceRect(selectedID);
        m_SourceRectValues[0] = sourceRect.x;
        m_SourceRectValues[1] = sourceRect.y;
        m_SourceRectValues[2] = sourceRect.width;
        m_SourceRectValues[3] = sourceRect.height;
    }

    ImGui::SeparatorText("Animation");

    bool hasAnimation = scene.HasAnimation(selectedID);
    if (ImGui::Checkbox("Has Animation", &hasAnimation))
    {
        if (hasAnimation)
            scene.EnsureAnimation(selectedID);
        else
            scene.RemoveAnimation(selectedID);
    }

    if (hasAnimation)
    {
        if (ImGui::InputText("Animation Set Path", m_AnimationSetPathBuffer.data(), m_AnimationSetPathBuffer.size()))
            scene.SetAnimationSetPath(selectedID, m_AnimationSetPathBuffer.data());

        ImGui::InputText("Clip Name", m_AnimationClipBuffer.data(), m_AnimationClipBuffer.size());

        if (ImGui::Button("Play Clip"))
            scene.PlayAnimation(selectedID, m_AnimationClipBuffer.data(), true);

        ImGui::SameLine();

        if (ImGui::Button("Stop Clip"))
            scene.StopAnimation(selectedID);

        ImGui::SameLine();

        if (ImGui::Button("Reset Clip"))
            scene.ResetAnimation(selectedID);

        ImGui::Text("Playing: %s", scene.IsAnimationPlaying(selectedID) ? "Yes" : "No");
        ImGui::Text("Finished: %s", scene.HasAnimationFinished(selectedID) ? "Yes" : "No");
    }

    ImGui::SeparatorText("Hierarchy");

    if (scene.HasParent(selectedID))
    {
        const GameObjectID parentID = scene.GetParentID(selectedID);
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
    scene.GetChildDestroyPolicy(selectedID) == ChildDestroyPolicy::DestroyWithParent ? 1 : 0;

    const char* destroyPolicyLabels[] = { "Detach To Root", "Destroy With Parent" };
    if (ImGui::Combo(
            "On Parent Destroy",
            &destroyPolicyIndex,
            destroyPolicyLabels,
            IM_ARRAYSIZE(destroyPolicyLabels)))
    {
        scene.SetChildDestroyPolicy(
            selectedID,
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
            scene.ClearParent(selectedID);
        else
            scene.SetParent(selectedID, static_cast<GameObjectID>(m_ReparentTargetID));
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Parent"))
    {
        scene.ClearParent(selectedID);
        m_ReparentTargetID = 0;
    }

    ImGui::End();
}
