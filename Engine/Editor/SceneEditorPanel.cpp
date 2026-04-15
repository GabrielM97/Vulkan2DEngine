#include "Editor/SceneEditorPanel.h"

#include <array>
#include <cstddef>
#include <cstdio>
#include <limits>
#include <string>

#include <imgui.h>

#include "Component/SceneComponentRegistry.h"
#include "Math/Transform2D.h"
#include "Reflection/Property.h"
#include "Scene/Scene.h"

namespace
{
    bool DrawReflectedPropertyEditor(
        const Property& property,
        void* componentData,
        const SceneComponent& component,
        GameObjectID objectID)
    {
        if ((property.flags & EditAnywhere) == 0 || componentData == nullptr)
            return false;

        std::byte* base = static_cast<std::byte*>(componentData);
        void* valuePtr = base + property.offset;
        const std::string label =
            std::string(property.name) +
            "##" +
            component.GetTypeName() +
            std::to_string(objectID);

        switch (property.type)
        {
        case PropertyType::Float:
            return ImGui::DragFloat(label.c_str(), static_cast<float*>(valuePtr), 0.1f);

        case PropertyType::Bool:
            return ImGui::Checkbox(label.c_str(), static_cast<bool*>(valuePtr));

        case PropertyType::Int:
            return ImGui::DragInt(label.c_str(), static_cast<int*>(valuePtr), 1.0f);

        case PropertyType::UInt:
        {
            uint32_t* value = static_cast<uint32_t*>(valuePtr);
            int signedValue = static_cast<int>(*value);
            if (!ImGui::DragInt(label.c_str(), &signedValue, 1.0f, 0, std::numeric_limits<int>::max()))
                return false;

            *value = static_cast<uint32_t>(signedValue);
            return true;
        }

        case PropertyType::String:
        {
            auto* value = static_cast<std::string*>(valuePtr);
            std::array<char, 256> buffer{};
            std::snprintf(buffer.data(), buffer.size(), "%s", value->c_str());
            if (!ImGui::InputText(label.c_str(), buffer.data(), buffer.size()))
                return false;

            *value = buffer.data();
            return true;
        }

        case PropertyType::Vec2:
            return ImGui::DragFloat2(label.c_str(), &static_cast<glm::vec2*>(valuePtr)->x, 0.1f);

        case PropertyType::Vec4:
            return ImGui::DragFloat4(label.c_str(), &static_cast<glm::vec4*>(valuePtr)->x, 0.1f);
        }

        return false;
    }

    void DrawSceneComponentInspector(Entity& entity, GameObjectID objectID)
    {
        auto componentIDs = entity.GetTrackedComponentIDs();
        bool removedComponent = false;
        bool drewAnyComponent = false;

        for (ComponentTypeID componentID : componentIDs)
        {
            const SceneComponent* component = SceneComponentRegistry::Get().Find(componentID);
            if (component == nullptr || !component->Has(entity))
                continue;

            drewAnyComponent = true;
            ImGui::PushID(static_cast<int>(componentID));

            ImGui::Separator();
            ImGui::TextUnformatted(component->GetTypeName());
            ImGui::SameLine();
            const float removeButtonWidth = 70.0f;
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - removeButtonWidth);

            if (ImGui::SmallButton("Remove"))
            {
                component->Remove(entity);
                removedComponent = true;
                ImGui::PopID();
                break;
            }

            void* componentData = component->GetMutableData(entity);
            for (const Property& property : component->GetProperties())
            {
                DrawReflectedPropertyEditor(property, componentData, *component, objectID);
            }

            ImGui::PopID();
        }

        if (removedComponent)
            return;

        if (!drewAnyComponent)
            ImGui::TextDisabled("No user scene components on this object.");

        if (ImGui::Button("Add Component"))
        {
            ImGui::SetNextWindowSize(ImVec2(260.0f, 0.0f), ImGuiCond_Appearing);
            ImGui::OpenPopup("AddSceneComponent");
        }

        if (ImGui::BeginPopup("AddSceneComponent"))
        {
            bool hasAvailableComponent = false;

            for (const SceneComponent* component : SceneComponentRegistry::Get().GetAll())
            {
                if (component == nullptr || component->Has(entity))
                    continue;

                hasAvailableComponent = true;
                if (ImGui::Selectable(component->GetTypeName()))
                {
                    component->Ensure(entity);
                    ImGui::CloseCurrentPopup();
                    break;
                }
            }

            if (!hasAvailableComponent)
                ImGui::TextDisabled("No available scene components to add.");

            ImGui::EndPopup();
        }
    }

    void DrawIdentitySection(Entity& entity, std::array<char, 256>& nameBuffer)
    {
        if (!ImGui::CollapsingHeader("Identity", ImGuiTreeNodeFlags_DefaultOpen))
            return;

        if (ImGui::InputText("Name", nameBuffer.data(), nameBuffer.size()))
            entity.SetName(nameBuffer.data());

        bool active = entity.IsActive();
        if (ImGui::Checkbox("Active", &active))
            entity.SetActive(active);
    }

    void DrawTransformSection(Entity& entity)
    {
        if (!ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            return;

        Transform2D worldTransform = entity.GetTransform();
        bool worldChanged = false;
        worldChanged |= ImGui::DragFloat2("Position", &worldTransform.position.x, 1.0f);
        worldChanged |= ImGui::DragFloat2("Scale", &worldTransform.scale.x, 0.01f, 0.0f, 1000.0f);
        worldChanged |= ImGui::DragFloat2("Pivot", &worldTransform.pivot.x, 0.01f, 0.0f, 1.0f);
        worldChanged |= ImGui::DragFloat("Rotation", &worldTransform.rotationDegrees, 1.0f);

        if (worldChanged)
            entity.SetTransform(worldTransform);

        if (ImGui::TreeNodeEx("Local Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Transform2D localTransform = entity.GetLocalTransform();
            bool localChanged = false;
            localChanged |= ImGui::DragFloat2("Local Position", &localTransform.position.x, 1.0f);
            localChanged |= ImGui::DragFloat2("Local Scale", &localTransform.scale.x, 0.01f, 0.0f, 1000.0f);
            localChanged |= ImGui::DragFloat2("Local Pivot", &localTransform.pivot.x, 0.01f, 0.0f, 1.0f);
            localChanged |= ImGui::DragFloat("Local Rotation", &localTransform.rotationDegrees, 1.0f);

            if (localChanged)
                entity.SetLocalTransform(localTransform);

            ImGui::TreePop();
        }
    }

    void DrawSpriteSection(
        Entity& entity,
        std::array<char, 260>& texturePathBuffer,
        int (&sourceRectValues)[4],
        int (&sourceGridValues)[4])
    {
        if (!ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
            return;

        if (ImGui::InputText("Texture Path", texturePathBuffer.data(), texturePathBuffer.size()))
            entity.SetSpriteTexturePath(texturePathBuffer.data());

        glm::vec2 spriteSize = entity.GetSpriteSize();
        if (ImGui::DragFloat2("Sprite Size", &spriteSize.x, 1.0f, 0.0f, 4096.0f))
            entity.SetSpriteSize(spriteSize);

        glm::vec4 tint = entity.GetSpriteTint();
        if (ImGui::ColorEdit4("Tint", &tint.x))
            entity.SetSpriteTint(tint);

        int layer = entity.GetSpriteLayer();
        if (ImGui::DragInt("Layer", &layer, 1.0f))
            entity.SetSpriteLayer(layer);

        bool visible = entity.IsSpriteVisible();
        if (ImGui::Checkbox("Visible", &visible))
            entity.SetSpriteVisible(visible);

        bool flipX = entity.IsSpriteFlippedX();
        if (ImGui::Checkbox("Flip X", &flipX))
            entity.SetSpriteFlipX(flipX);

        bool flipY = entity.IsSpriteFlippedY();
        if (ImGui::Checkbox("Flip Y", &flipY))
            entity.SetSpriteFlipY(flipY);

        bool usesSourceRect = entity.SpriteUsesSourceRect();
        if (ImGui::Checkbox("Use Source Rect", &usesSourceRect))
        {
            if (usesSourceRect)
            {
                entity.SetSpriteSourceRect(
                    sourceRectValues[0],
                    sourceRectValues[1],
                    sourceRectValues[2],
                    sourceRectValues[3]
                );
            }
            else
            {
                entity.ClearSpriteSourceRect();
            }
        }

        if (usesSourceRect && ImGui::InputInt4("Source Rect", sourceRectValues))
        {
            entity.SetSpriteSourceRect(
                sourceRectValues[0],
                sourceRectValues[1],
                sourceRectValues[2],
                sourceRectValues[3]
            );
        }

        ImGui::SeparatorText("Grid");
        ImGui::InputInt("Grid Column", &sourceGridValues[0]);
        ImGui::InputInt("Grid Row", &sourceGridValues[1]);
        ImGui::InputInt("Cell Width", &sourceGridValues[2]);
        ImGui::InputInt("Cell Height", &sourceGridValues[3]);

        if (ImGui::Button("Apply Grid Frame"))
        {
            entity.SetSpriteSourceRectFromGrid(
                sourceGridValues[0],
                sourceGridValues[1],
                sourceGridValues[2],
                sourceGridValues[3]
            );

            const IntRect sourceRect = entity.GetSpriteSourceRect();
            sourceRectValues[0] = sourceRect.x;
            sourceRectValues[1] = sourceRect.y;
            sourceRectValues[2] = sourceRect.width;
            sourceRectValues[3] = sourceRect.height;
        }
    }

    void DrawAnimationSection(
        Entity& entity,
        std::array<char, 260>& animationSetPathBuffer,
        std::array<char, 128>& animationClipBuffer)
    {
        if (!ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
            return;

        bool hasAnimation = entity.HasAnimation();
        if (ImGui::Checkbox("Has Animation", &hasAnimation))
        {
            if (hasAnimation)
                entity.EnsureAnimation();
            else
                entity.RemoveAnimation();
        }

        if (!hasAnimation)
            return;

        if (ImGui::InputText("Animation Set Path", animationSetPathBuffer.data(), animationSetPathBuffer.size()))
            entity.SetAnimationSetPath(animationSetPathBuffer.data());

        ImGui::InputText("Clip Name", animationClipBuffer.data(), animationClipBuffer.size());

        if (ImGui::Button("Play Clip"))
            entity.PlayAnimation(animationClipBuffer.data(), true);

        ImGui::SameLine();
        if (ImGui::Button("Stop Clip"))
            entity.StopAnimation();

        ImGui::SameLine();
        if (ImGui::Button("Reset Clip"))
            entity.ResetAnimation();

        ImGui::Text("Playing: %s", entity.IsAnimationPlaying() ? "Yes" : "No");
        ImGui::Text("Finished: %s", entity.HasAnimationFinished() ? "Yes" : "No");
    }

    void DrawRelationshipSection(
        Scene& scene,
        Entity& entity,
        GameObjectID selectedID,
        int& reparentTargetID)
    {
        if (!ImGui::CollapsingHeader("Relationship", ImGuiTreeNodeFlags_DefaultOpen))
            return;

        if (entity.HasParent())
        {
            const GameObjectID parentID = entity.GetParentID();
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
            entity.GetChildDestroyPolicy() == ChildDestroyPolicy::DestroyWithParent ? 1 : 0;

        const char* destroyPolicyLabels[] = { "Detach To Root", "Destroy With Parent" };
        if (ImGui::Combo(
                "On Parent Destroy",
                &destroyPolicyIndex,
                destroyPolicyLabels,
                IM_ARRAYSIZE(destroyPolicyLabels)))
        {
            entity.SetChildDestroyPolicy(
                destroyPolicyIndex == 1
                    ? ChildDestroyPolicy::DestroyWithParent
                    : ChildDestroyPolicy::DetachToRoot
            );
        }

        std::string comboLabel = "None";
        if (reparentTargetID != 0)
            comboLabel = std::to_string(reparentTargetID);

        if (ImGui::BeginCombo("Reparent To", comboLabel.c_str()))
        {
            const bool noParentSelected = (reparentTargetID == 0);
            if (ImGui::Selectable("None", noParentSelected))
                reparentTargetID = 0;

            for (GameObjectID candidateID : scene.GetGameObjectIDs())
            {
                if (candidateID == selectedID)
                    continue;

                const bool candidateSelected = (reparentTargetID == static_cast<int>(candidateID));
                const std::string candidateLabel =
                    scene.GetGameObjectName(candidateID) + " (" + std::to_string(candidateID) + ")";

                if (ImGui::Selectable(candidateLabel.c_str(), candidateSelected))
                    reparentTargetID = static_cast<int>(candidateID);
            }

            ImGui::EndCombo();
        }

        if (ImGui::Button("Set Parent"))
        {
            if (reparentTargetID == 0)
                entity.ClearParent();
            else
                entity.SetParent(static_cast<GameObjectID>(reparentTargetID));
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear Parent"))
        {
            entity.ClearParent();
            reparentTargetID = 0;
        }
    }
}

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

    ImGui::Text("Object ID: %llu", static_cast<unsigned long long>(selectedID));

    if (ImGui::Button("Destroy Object"))
    {
        selected.Destroy();
        SelectObject(scene, 0);
        ImGui::End();
        return;
    }

    DrawIdentitySection(selected, m_NameBuffer);
    DrawTransformSection(selected);
    DrawSpriteSection(selected, m_TexturePathBuffer, m_SourceRectValues, m_SourceGridValues);
    DrawAnimationSection(selected, m_AnimationSetPathBuffer, m_AnimationClipBuffer);
    DrawRelationshipSection(scene, selected, selectedID, m_ReparentTargetID);

    ImGui::SeparatorText("User Components");
    DrawSceneComponentInspector(selected, selectedID);

    ImGui::End();
}
