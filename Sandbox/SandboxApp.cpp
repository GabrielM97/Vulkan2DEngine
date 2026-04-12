#include "SandboxApp.h"

#include <string>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    m_Player = m_Scene.CreateGameObjectHandle("Player Sprite");
    m_Player.SetLocalPosition({0.0f, 0.0f});
    m_Player.SetLocalRotation(0.0f);
    m_Player.SetSpriteTexturePath("Assets/Textures/character-spritesheet.png");
    m_Player.SetSpriteSourceRectFromGrid(0, 10, 64, 64);
    m_Player.SetSpriteTint(glm::vec4(1.0f));
    m_Player.SetSpriteLayer(0);

    if (GameObject* player = m_Scene.FindGameObjectByID(m_Player.GetID()))
    {
        player->animation.emplace();
        player->animation->SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
        player->animation->Play("Walk");
    }

    GameObjectHandle weapon = m_Scene.CreateGameObjectHandle("Weapon", m_Player.GetID());
    weapon.SetLocalPosition({50.f, 0.f});
    weapon.SetSpriteSize({16.f, 16.f});
    weapon.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    weapon.SetSpriteLayer(1);

    GameObjectHandle weapon2 = m_Scene.CreateGameObjectHandle("Weapon", m_Player.GetID());
    weapon2.SetLocalPosition({-25.f, 0.f});
    weapon2.SetSpriteSize({16.f, 16.f});
    weapon2.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    weapon2.SetSpriteLayer(1);
}

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};

    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureKeyboard)
    {
        if (IsKeyDown(GLFW_KEY_A))
            command.moveX -= 1.0f;
        if (IsKeyDown(GLFW_KEY_D))
            command.moveX += 1.0f;
        if (IsKeyDown(GLFW_KEY_W))
            command.moveY -= 1.0f;
        if (IsKeyDown(GLFW_KEY_S))
            command.moveY += 1.0f;

        if (IsKeyDown(GLFW_KEY_Q))
            command.zoomDelta -= 1.0f;
        if (IsKeyDown(GLFW_KEY_E))
            command.zoomDelta += 1.0f;
    }

    m_Scene.UpdateCamera(
        command,
        deltaTime,
        static_cast<float>(GetRenderer().GetFramebufferWidth()),
        static_cast<float>(GetRenderer().GetFramebufferHeight())
    );

    m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}

void SandboxApp::OnImGuiUpdate()
{
    DrawHierarchyPanel();
    DrawInspectorPanel();
}

void SandboxApp::SelectObject(GameObjectID id)
{
    m_SelectedObjectID = id;
    SyncInspectorFromSelection();
}

void SandboxApp::SyncInspectorFromSelection()
{
    m_InspectorObjectID = m_SelectedObjectID;

    GameObject* selected = m_Scene.FindGameObjectByID(m_InspectorObjectID);
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
        m_Scene.GetSpriteTexturePath(m_InspectorObjectID).c_str()
    );

    const IntRect sourceRect = m_Scene.GetSpriteSourceRect(m_InspectorObjectID);
    m_SourceRectValues[0] = sourceRect.x;
    m_SourceRectValues[1] = sourceRect.y;
    m_SourceRectValues[2] = sourceRect.width;
    m_SourceRectValues[3] = sourceRect.height;
    m_ReparentTargetID = selected->HasParent()
                             ? static_cast<int>(selected->GetParentID())
                             : 0;
}

void SandboxApp::DrawHierarchyPanel()
{
    ImGui::Begin("Hierarchy");

    const std::vector<const GameObject*> roots = m_Scene.GetRootGameObjects();
    for (const GameObject* object : roots)
    {
        if (object != nullptr)
            DrawHierarchyNode(*object);
    }

    ImGui::End();
}

void SandboxApp::DrawHierarchyNode(const GameObject& object)
{
    const GameObjectID id = object.GetID();
    const bool isSelected = (m_SelectedObjectID == id);
    const bool hasChildren = m_Scene.HasChildren(id);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (!hasChildren)
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;

    std::string label = object.GetName() + "##" + std::to_string(id);

    const bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uintptr_t>(id)), flags, "%s",
                                          label.c_str());

    if (ImGui::IsItemClicked())
        SelectObject(id);

    ImGui::SameLine();
    ImGui::TextDisabled("(ID: %llu)", static_cast<unsigned long long>(id));

    if (opened)
    {
        const std::vector<const GameObject*> children = m_Scene.GetChildGameObjects(id);
        for (const GameObject* child : children)
        {
            if (child != nullptr)
                DrawHierarchyNode(*child);
        }

        ImGui::TreePop();
    }
}

void SandboxApp::DrawInspectorPanel()
{
    ImGui::Begin("Inspector");

    GameObject* selected = m_Scene.FindGameObjectByID(m_SelectedObjectID);
    if (selected == nullptr)
    {
        if (m_SelectedObjectID != 0)
            SelectObject(0);

        ImGui::TextUnformatted("No object selected.");
        ImGui::End();
        return;
    }

    if (m_InspectorObjectID != m_SelectedObjectID)
        SyncInspectorFromSelection();

    ImGui::Text("Selected: %s", selected->GetName().c_str());
    ImGui::Text("ID: %llu", static_cast<unsigned long long>(selected->GetID()));
    ImGui::Separator();

    if (ImGui::InputText("Name", m_NameBuffer.data(), m_NameBuffer.size()))
        m_Scene.SetGameObjectName(selected->GetID(), m_NameBuffer.data());

    bool active = selected->isActive();
    if (ImGui::Checkbox("Active", &active))
        m_Scene.SetGameObjectActive(selected->GetID(), active);

    Transform2D localTransform = m_Scene.GetLocalTransform(selected->GetID());
    bool localChanged = false;
    localChanged |= ImGui::DragFloat2("Local Position", &localTransform.position.x, 1.0f);
    localChanged |= ImGui::DragFloat2("Local Scale", &localTransform.scale.x, 0.01f, 0.0f, 1000.0f);
    localChanged |= ImGui::DragFloat2("Local Pivot", &localTransform.pivot.x, 0.01f, 0.0f, 1.0f);
    localChanged |= ImGui::DragFloat("Local Rotation", &localTransform.rotationDegrees, 1.0f);

    if (localChanged)
        m_Scene.SetLocalTransform(selected->GetID(), localTransform);

    const Transform2D worldTransform = m_Scene.GetWorldTransform(selected->GetID());
    ImGui::SeparatorText("World Transform");
    ImGui::Text("Position: %.2f, %.2f", worldTransform.position.x, worldTransform.position.y);
    ImGui::Text("Scale: %.2f, %.2f", worldTransform.scale.x, worldTransform.scale.y);
    ImGui::Text("Pivot: %.2f, %.2f", worldTransform.pivot.x, worldTransform.pivot.y);
    ImGui::Text("Rotation: %.2f", worldTransform.rotationDegrees);

    ImGui::SeparatorText("Sprite");

    if (ImGui::InputText("Texture Path", m_TexturePathBuffer.data(), m_TexturePathBuffer.size()))
        m_Scene.SetSpriteTexturePath(selected->GetID(), m_TexturePathBuffer.data());

    glm::vec2 spriteSize = m_Scene.GetSpriteSize(selected->GetID());
    if (ImGui::DragFloat2("Sprite Size", &spriteSize.x, 1.0f, 0.0f, 4096.0f))
        m_Scene.SetSpriteSize(selected->GetID(), spriteSize);

    glm::vec4 tint = m_Scene.GetSpriteTint(selected->GetID());
    if (ImGui::ColorEdit4("Tint", &tint.x))
        m_Scene.SetSpriteTint(selected->GetID(), tint);

    int layer = m_Scene.GetSpriteLayer(selected->GetID());
    if (ImGui::DragInt("Layer", &layer, 1.0f))
        m_Scene.SetSpriteLayer(selected->GetID(), layer);

    bool visible = m_Scene.IsSpriteVisible(selected->GetID());
    if (ImGui::Checkbox("Visible", &visible))
        m_Scene.SetSpriteVisible(selected->GetID(), visible);

    bool flipX = m_Scene.IsSpriteFlippedX(selected->GetID());
    if (ImGui::Checkbox("Flip X", &flipX))
        m_Scene.SetSpriteFlipX(selected->GetID(), flipX);

    bool flipY = m_Scene.IsSpriteFlippedY(selected->GetID());
    if (ImGui::Checkbox("Flip Y", &flipY))
        m_Scene.SetSpriteFlipY(selected->GetID(), flipY);

    bool usesSourceRect = m_Scene.SpriteUsesSourceRect(selected->GetID());
    if (ImGui::Checkbox("Use Source Rect", &usesSourceRect))
    {
        if (usesSourceRect)
        {
            m_Scene.SetSpriteSourceRect(
                selected->GetID(),
                m_SourceRectValues[0],
                m_SourceRectValues[1],
                m_SourceRectValues[2],
                m_SourceRectValues[3]
            );
        }
        else
        {
            m_Scene.ClearSpriteSourceRect(selected->GetID());
        }
    }

    if (usesSourceRect)
    {
        if (ImGui::InputInt4("Source Rect", m_SourceRectValues))
        {
            m_Scene.SetSpriteSourceRect(
                selected->GetID(),
                m_SourceRectValues[0],
                m_SourceRectValues[1],
                m_SourceRectValues[2],
                m_SourceRectValues[3]
            );
        }
    }

    ImGui::InputInt("Grid Column", &m_SourceGridValues[0]);
    ImGui::InputInt("Grid Row", &m_SourceGridValues[1]);
    ImGui::InputInt("Cell Width", &m_SourceGridValues[2]);
    ImGui::InputInt("Cell Height", &m_SourceGridValues[3]);

    if (ImGui::Button("Apply Grid Frame"))
    {
        m_Scene.SetSpriteSourceRectFromGrid(
            selected->GetID(),
            m_SourceGridValues[0],
            m_SourceGridValues[1],
            m_SourceGridValues[2],
            m_SourceGridValues[3]
        );

        const IntRect sourceRect = m_Scene.GetSpriteSourceRect(selected->GetID());
        m_SourceRectValues[0] = sourceRect.x;
        m_SourceRectValues[1] = sourceRect.y;
        m_SourceRectValues[2] = sourceRect.width;
        m_SourceRectValues[3] = sourceRect.height;
    }

    ImGui::SeparatorText("Hierarchy");

    if (selected->HasParent())
    {
        const GameObject* parent = m_Scene.FindGameObjectByID(selected->GetParentID());
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

    if (ImGui::BeginCombo(
        "Reparent To",
        m_ReparentTargetID == 0 ? "None" : std::to_string(m_ReparentTargetID).c_str()))
    {
        const bool noParentSelected = (m_ReparentTargetID == 0);
        if (ImGui::Selectable("None", noParentSelected))
            m_ReparentTargetID = 0;

        for (const auto& objectPtr : m_Scene.GetGameObjects())
        {
            const GameObject& candidate = *objectPtr;
            if (candidate.GetID() == selected->GetID())
                continue;

            const bool candidateSelected =
                (m_ReparentTargetID == static_cast<int>(candidate.GetID()));

            std::string candidateLabel =
                candidate.GetName() + " (" + std::to_string(candidate.GetID()) + ")";

            if (ImGui::Selectable(candidateLabel.c_str(), candidateSelected))
                m_ReparentTargetID = static_cast<int>(candidate.GetID());
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Set Parent"))
    {
        if (m_ReparentTargetID == 0)
            m_Scene.ClearParent(selected->GetID());
        else
            m_Scene.SetParent(selected->GetID(), static_cast<GameObjectID>(m_ReparentTargetID));
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear Parent"))
    {
        m_Scene.ClearParent(selected->GetID());
        m_ReparentTargetID = 0;
    }

    ImGui::End();
}
