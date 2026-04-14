#pragma once

#include <memory>

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

    VulkanRenderer& GetRenderer() { return vulkanRenderer; }

    bool IsKeyDown(int key) const;
    bool IsKeyboardCapturedByUI() const;

private:
    std::unique_ptr<Window> window = nullptr;
    std::unique_ptr<EditorLayer> editorLayer = nullptr;
    VulkanRenderer vulkanRenderer;
    ImGuiLayer imguiLayer;
    
    bool isRunning = false;
    bool isShutdown = false;
};
