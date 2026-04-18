#include "SandboxApp.h"

#include <GLFW/glfw3.h>

#include "Component/Gameplay/PlayerMovementComponent.h"
#include "Editor/EditorLayer.h"

void SandboxApp::OnInit()
{
    m_EditorSceneController.SetSceneFilePath(m_SceneFilePath);
    CreateDefaultScene();
}

void SandboxApp::CreateDefaultScene()
{
    m_Player = m_Scene.Place<Player>();
    Entity playerEntity = m_Scene.GetEntity(m_Player.GetID());
    playerEntity.EnsureBoxCollider();
    playerEntity.SetBoxColliderSize({64.f, 64.f});
    playerEntity.SetColliderBodyType(ColliderBodyType::Dynamic);
    
    Entity wall = m_Scene.CreateEntity("Wall");
    wall.SetPosition({200.0f, 0.0f});
    wall.SetSpriteSize({32.0f, 32.0f});
    wall.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    wall.EnsureBoxCollider();
    wall.SetBoxColliderSize({32.0f, 32.0f});
    wall.SetColliderBodyType(ColliderBodyType::Static);
    
    Entity map = m_Scene.CreateEntity("Map");
    map.AddComponent<TileMapComponent>();
    
    map.SetBoxColliderEnabled(false);
}

void SandboxApp::RefreshRuntimeHandles()
{
    if (m_Scene.IsValidGameObject(m_Player.GetID()))
    {
        m_Player = Player(m_Scene.GetEntity(m_Player.GetID()));
    }
    else
    {
        m_Player = {};
    }
}

void SandboxApp::OnEnterPlayMode()
{
    GetEditorLayer().GetTileMapEditorPanel().SetTileMapViewEnabled(false);
}

void SandboxApp::OnExitPlayMode()
{
    RefreshRuntimeHandles();
}

void SandboxApp::OnUpdate(float deltaTime)
{
    glm::vec2 playerInput{0.0f, 0.0f};

    const InputState& input = GetInputState();
    const SceneViewportState& viewportState = GetSceneViewportState();

    const bool sceneReloaded = m_EditorSceneController.Update(
        m_Scene,
        GetEditorLayer(),
        input,
        viewportState,
        IsEditorPlaying(),
        deltaTime
    );

    if (sceneReloaded)
        RefreshRuntimeHandles();

    m_TileMapEditorController.Update(
        m_Scene,
        GetEditorLayer().GetTileMapEditorPanel(),
        input,
        viewportState,
        !IsEditorPlaying()
    );

    if (input.CanUseRuntimeViewportInput())
    {
        if (input.IsKeyDown(GLFW_KEY_A))
        {
            playerInput.x -= 1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_D))
        {
            playerInput.x += 1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_W))
        {
            playerInput.y -= 1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_S))
        {
            playerInput.y += 1.0f;
        }
    }

    if (IsEditorPlaying() && m_Player.IsValid())
    {
        Entity playerEntity = m_Scene.GetEntity(m_Player.GetID());
        const glm::vec2 moveDelta = playerInput * 150.0f * deltaTime;
        playerEntity.MoveWithCollision(moveDelta);
    }

    if (IsEditorPlaying())
        m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_EditorSceneController.Render(m_Scene, GetEditorLayer(), renderer);
}
