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
        m_ball = {};
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

    // if (input.CanUseRuntimeViewportInput())
    // {
    //     if (input.IsKeyDown(GLFW_KEY_A))
    //     {
    //         playerInput.x -= 1.0f;
    //     }
    //     if (input.IsKeyDown(GLFW_KEY_D))
    //     {
    //         playerInput.x += 1.0f;
    //     }
    //     if (input.IsKeyDown(GLFW_KEY_W))
    //     {
    //         playerInput.y -= 1.0f;
    //     }
    //     if (input.IsKeyDown(GLFW_KEY_S))
    //     {
    //         playerInput.y += 1.0f;
    //     }
    // }

    
    if (IsEditorPlaying())
    {
        m_Scene.Update(deltaTime);
        m_ball.update(deltaTime);
    }
       
}

void PongApp::OnRender(VulkanRenderer& renderer)
{
    m_EditorSceneController.Render(m_Scene, GetEditorLayer(), renderer);
}
