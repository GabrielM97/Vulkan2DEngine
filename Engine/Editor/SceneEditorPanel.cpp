#include "Editor/SceneEditorPanel.h"

#include <array>
#include <cstddef>
#include <cstdio>
#include <limits>
#include <string>

#include <imgui.h>

#include "Component/SceneComponentRegistry.h"
#include "Editor/CoreComponentInspectorRegistry.h"
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

    void RegisterCoreInspectorSections()
    {
        static bool registered = false;
        if (registered)
            return;

        registered = true;

        CoreComponentInspectorRegistry::Get().Register({
            "Transform",
            [](const Entity&) { return true; },
            {},
            {},
            [](Entity& entity, SceneEditorPanel& panel) { panel.DrawTransformSection(entity); }
        });

        CoreComponentInspectorRegistry::Get().Register({
            "Sprite",
            [](const Entity&) { return true; },
            {},
            {},
            [](Entity& entity, SceneEditorPanel& panel) { panel.DrawSpriteSection(entity); }
        });

        CoreComponentInspectorRegistry::Get().Register({
            "Animation",
            [](const Entity& entity) { return entity.HasAnimation(); },
            [](Entity& entity) { entity.EnsureAnimation(); },
            [](Entity& entity) { entity.RemoveAnimation(); },
            [](Entity& entity, SceneEditorPanel& panel) { panel.DrawAnimationSection(entity); }
        });

        CoreComponentInspectorRegistry::Get().Register({
            "Box Collider",
            [](const Entity& entity) { return entity.HasBoxCollider(); },
            [](Entity& entity) { entity.EnsureBoxCollider(); },
            [](Entity& entity) { entity.RemoveBoxCollider(); },
            [](Entity& entity, SceneEditorPanel& panel) { panel.DrawBoxColliderSection(entity); }
        });
    }
}

void SceneEditorPanel::Draw(Scene& scene, GameObjectID& selectedObjectID)
{
    DrawHierarchyPanel(scene, selectedObjectID);
    DrawInspectorPanel(scene, selectedObjectID);
}

void SceneEditorPanel::SelectObject(Scene& scene, GameObjectID& selectedObjectID, GameObjectID id)
{
    selectedObjectID = id;
    SyncInspectorFromSelection(scene, selectedObjectID);
}

void SceneEditorPanel::SyncInspectorFromSelection(Scene& scene, GameObjectID selectedObjectID)
{
    m_InspectorObjectID = selectedObjectID;

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

void SceneEditorPanel::DrawHierarchyPanel(Scene& scene, GameObjectID& selectedObjectID)
{
    ImGui::Begin("Hierarchy");

    const std::vector<GameObjectID> roots = scene.GetRootGameObjects();
    for (GameObjectID id : roots)
        DrawHierarchyNode(scene, selectedObjectID, id);

    ImGui::End();
}

void SceneEditorPanel::DrawHierarchyNode(Scene& scene, GameObjectID& selectedObjectID, GameObjectID id)
{
    const bool isSelected = (selectedObjectID == id);
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
        SelectObject(scene, selectedObjectID, id);

    ImGui::SameLine();
    ImGui::TextDisabled("(ID: %llu)", static_cast<unsigned long long>(id));

    if (opened)
    {
        const std::vector<GameObjectID> children = scene.GetChildGameObjects(id);
        for (GameObjectID childID : children)
            DrawHierarchyNode(scene, selectedObjectID, childID);

        ImGui::TreePop();
    }
}

void SceneEditorPanel::DrawInspectorPanel(Scene& scene, GameObjectID& selectedObjectID)
{
    ImGui::Begin("Inspector");

    if (!scene.IsValidGameObject(selectedObjectID))
    {
        if (selectedObjectID != 0)
            SelectObject(scene, selectedObjectID, 0);

        ImGui::TextUnformatted("No object selected.");
        ImGui::End();
        return;
    }

    if (m_InspectorObjectID != selectedObjectID)
        SyncInspectorFromSelection(scene, selectedObjectID);

    const GameObjectID selectedID = selectedObjectID;
    Entity selected = scene.GetEntity(selectedID);

    ImGui::Text("Object ID: %llu", static_cast<unsigned long long>(selectedID));

    if (ImGui::Button("Destroy Object"))
    {
        selected.Destroy();
        SelectObject(scene, selectedObjectID, 0);
        ImGui::End();
        return;
    }

    DrawIdentitySection(selected, m_NameBuffer);
    DrawCoreComponentSections(selected);
    DrawRelationshipSection(scene, selected, selectedID, m_ReparentTargetID);

    ImGui::SeparatorText("User Components");
    DrawSceneComponentInspector(selected, selectedID);

    ImGui::End();
}

void SceneEditorPanel::DrawCoreComponentSections(Entity& entity)
{
    RegisterCoreInspectorSections();

    for (const CoreComponentInspectorEntry& entry : CoreComponentInspectorRegistry::Get().GetEntries())
    {
        const bool hasComponent = entry.hasComponent ? entry.hasComponent(entity) : true;

        if (!hasComponent)
        {
            if (entry.ensureComponent)
            {
                const std::string buttonLabel = "Add " + entry.sectionName;
                if (ImGui::Button(buttonLabel.c_str()))
                    entry.ensureComponent(entity);
            }
            continue;
        }

        if (entry.removeComponent)
        {
            const std::string buttonLabel = "Remove " + entry.sectionName;
            if (ImGui::Button(buttonLabel.c_str()))
            {
                entry.removeComponent(entity);
                continue;
            }
        }

        if (entry.draw)
            entry.draw(entity, *this);
    }
}

void SceneEditorPanel::DrawTransformSection(Entity& entity)
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

void SceneEditorPanel::DrawSpriteSection(Entity& entity)
{
    if (!ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    if (ImGui::InputText("Texture Path", m_TexturePathBuffer.data(), m_TexturePathBuffer.size()))
        entity.SetSpriteTexturePath(m_TexturePathBuffer.data());

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
                m_SourceRectValues[0],
                m_SourceRectValues[1],
                m_SourceRectValues[2],
                m_SourceRectValues[3]
            );
        }
        else
        {
            entity.ClearSpriteSourceRect();
        }
    }

    if (usesSourceRect && ImGui::InputInt4("Source Rect", m_SourceRectValues))
    {
        entity.SetSpriteSourceRect(
            m_SourceRectValues[0],
            m_SourceRectValues[1],
            m_SourceRectValues[2],
            m_SourceRectValues[3]
        );
    }

    ImGui::SeparatorText("Grid");
    ImGui::InputInt("Grid Column", &m_SourceGridValues[0]);
    ImGui::InputInt("Grid Row", &m_SourceGridValues[1]);
    ImGui::InputInt("Cell Width", &m_SourceGridValues[2]);
    ImGui::InputInt("Cell Height", &m_SourceGridValues[3]);

    if (ImGui::Button("Apply Grid Frame"))
    {
        entity.SetSpriteSourceRectFromGrid(
            m_SourceGridValues[0],
            m_SourceGridValues[1],
            m_SourceGridValues[2],
            m_SourceGridValues[3]
        );

        const IntRect sourceRect = entity.GetSpriteSourceRect();
        m_SourceRectValues[0] = sourceRect.x;
        m_SourceRectValues[1] = sourceRect.y;
        m_SourceRectValues[2] = sourceRect.width;
        m_SourceRectValues[3] = sourceRect.height;
    }
}

void SceneEditorPanel::DrawAnimationSection(Entity& entity)
{
    if (!ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    if (ImGui::InputText("Animation Set Path", m_AnimationSetPathBuffer.data(), m_AnimationSetPathBuffer.size()))
        entity.SetAnimationSetPath(m_AnimationSetPathBuffer.data());

    ImGui::InputText("Clip Name", m_AnimationClipBuffer.data(), m_AnimationClipBuffer.size());

    if (ImGui::Button("Play Clip"))
        entity.PlayAnimation(m_AnimationClipBuffer.data(), true);

    ImGui::SameLine();
    if (ImGui::Button("Stop Clip"))
        entity.StopAnimation();

    ImGui::SameLine();
    if (ImGui::Button("Reset Clip"))
        entity.ResetAnimation();

    ImGui::Text("Playing: %s", entity.IsAnimationPlaying() ? "Yes" : "No");
    ImGui::Text("Finished: %s", entity.HasAnimationFinished() ? "Yes" : "No");
}

void SceneEditorPanel::DrawBoxColliderSection(Entity& entity)
{
    if (!entity.HasBoxCollider())
        return;

    if (!ImGui::CollapsingHeader("Box Collider", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    glm::vec2 size = entity.GetBoxColliderSize();
    if (ImGui::DragFloat2("Collider Size", &size.x, 1.0f, 1.0f, 4096.0f))
        entity.SetBoxColliderSize(size);

    glm::vec2 offset = entity.GetBoxColliderOffset();
    if (ImGui::DragFloat2("Collider Offset", &offset.x, 1.0f, -4096.0f, 4096.0f))
        entity.SetBoxColliderOffset(offset);

    bool enabled = entity.IsBoxColliderEnabled();
    if (ImGui::Checkbox("Collider Enabled", &enabled))
        entity.SetBoxColliderEnabled(enabled);

    bool blocksMovement = entity.DoesColliderBlockMovement();
    if (ImGui::Checkbox("Blocks Movement", &blocksMovement))
        entity.SetColliderBlocksMovement(blocksMovement);

    bool isTrigger = entity.IsColliderTrigger();
    if (ImGui::Checkbox("Trigger", &isTrigger))
        entity.SetColliderTrigger(isTrigger);

    int bodyType = entity.GetColliderBodyType() == ColliderBodyType::Dynamic ? 1 : 0;
    if (ImGui::Combo("Body Type", &bodyType, "Static\0Dynamic\0"))
        entity.SetColliderBodyType(bodyType == 1 ? ColliderBodyType::Dynamic : ColliderBodyType::Static);
}
