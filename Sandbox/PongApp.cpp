#include "PongApp.h"

#include <GLFW/glfw3.h>

#include "Component/Gameplay/PlayerMovementComponent.h"
#include "Editor/EditorLayer.h"

void PongApp::OnInit()
{
    m_EditorSceneController.SetSceneFilePath(m_SceneFilePath);
    CreateDefaultScene();
}

void PongApp::CreateDefaultScene()
{
    m_ball = m_Scene.Place<Ball>();
    Entity ballEntity = m_Scene.GetEntity(m_ball.GetID());
    ballEntity.EnsureBoxCollider();
    ballEntity.SetBoxColliderSize({32.f, 32.f});
    ballEntity.SetColliderBodyType(ColliderBodyType::Dynamic);
    
    m_padel = m_Scene.Place<Padel>();
    m_padel2 = m_Scene.Place<Padel>();
    m_padel2.SetName("Padel 2");
    
    Entity map = m_Scene.CreateEntity("Map");
    map.AddComponent<TileMapComponent>();
    map.SetBoxColliderEnabled(false);
    map.ResizeTileMap(32, 16);
}

void PongApp::RefreshRuntimeHandles()
{
    if (m_Scene.IsValidGameObject(m_ball.GetID()))
    {
        m_ball = Ball(m_Scene.GetEntity(m_ball.GetID()));
    }
    else
    {
        m_ball = m_Scene.Place<Ball>();
    }
    
    if (m_Scene.IsValidGameObject(m_padel.GetID()))
    {
        m_padel = Padel(m_Scene.GetEntity(m_padel.GetID()));
    }
    else
    {
        m_padel = m_Scene.Place<Padel>();
    }
    
    if (m_Scene.IsValidGameObject(m_padel2.GetID()))
    {
        m_padel2 = Padel(m_Scene.GetEntity(m_padel2.GetID()));
    }
    else
    {
        m_padel2 = m_Scene.Place<Padel>();
    }
}

void PongApp::OnEnterPlayMode()
{
    GetEditorLayer().GetTileMapEditorPanel().SetTileMapViewEnabled(false);
}

void PongApp::OnExitPlayMode()
{
    RefreshRuntimeHandles();
}

void PongApp::OnUpdate(float deltaTime)
{
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

    PlayerMovementComponent& padelInput = m_Scene.GetEntity(m_padel.GetID()).GetComponent<PlayerMovementComponent>();
    PlayerMovementComponent& padel2Input = m_Scene.GetEntity(m_padel2.GetID()).GetComponent<PlayerMovementComponent>();
    
    if (input.CanUseRuntimeViewportInput())
    {
      
        if (input.IsKeyDown(GLFW_KEY_W))
        {
            padelInput.MoveDirection.y = -1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_S))
        {
            padelInput.MoveDirection.y = 1.0f;
        }
        
        if (input.IsKeyDown(GLFW_KEY_UP))
        {
            padel2Input.MoveDirection.y = -1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_DOWN))
        {
            padel2Input.MoveDirection.y = 1.0f;
        }
    }

    
    if (IsEditorPlaying())
    {
        m_ball.update(deltaTime);
        m_padel.update(deltaTime);
        m_padel2.update(deltaTime);
        m_Scene.Update(deltaTime);
    }
       
}

void PongApp::OnRender(VulkanRenderer& renderer)
{
    m_EditorSceneController.Render(m_Scene, GetEditorLayer(), renderer);
}
