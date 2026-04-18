#pragma once

#include <memory>
#include <string>

#include "Core/InputState.h"
#include "Platform/Window.h"
#include "Renderer/VulkanRenderer.h"
#include "Renderer/ImGuiLayer.h"
#include "Editor/SceneViewportPanel.h"

class EditorLayer;
class Scene;

class Application
{
public:
    Application();
    virtual ~Application();

    bool Init();
    void Run();
    void Shutdown();
    
    const SceneViewportState& GetSceneViewportState() const;

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender(VulkanRenderer& renderer) {}
    virtual void OnShutdown() {}
    virtual Scene* GetEditorScene() { return nullptr; }
    bool IsEditorPlaying() const { return m_IsEditorPlaying; }
    virtual std::string GetPlayModeSnapshotPath() const { return {}; }
    virtual void OnEnterPlayMode() {}
    virtual void OnExitPlayMode() {}

    VulkanRenderer& GetRenderer() { return vulkanRenderer; }
    const InputState& GetInputState() const { return inputState; }
    
    EditorLayer& GetEditorLayer() { return *editorLayer; }
    const EditorLayer& GetEditorLayer() const { return *editorLayer; }

private:
    void EnterPlayMode();
    void ExitPlayMode();

    std::unique_ptr<Window> window = nullptr;
    std::unique_ptr<EditorLayer> editorLayer = nullptr;
    VulkanRenderer vulkanRenderer;
    ImGuiLayer imguiLayer;
    InputState inputState;
    
    bool isRunning = false;
    bool isShutdown = false;
    bool m_IsEditorPlaying = false;
};
