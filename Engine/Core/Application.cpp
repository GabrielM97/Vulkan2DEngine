#include "Application.h"

#include <GLFW/glfw3.h>
#include <chrono>

#include "Editor/EditorLayer.h"

Application::Application()
{
    window = std::make_unique<Window>(800, 600, "Vulkan Engine Window");
    editorLayer = std::make_unique<EditorLayer>();
}

Application::~Application()
{
    Shutdown();
}

bool Application::Init()
{
    if (!window->Init())
        return false;

    auto [framebufferWidth, framebufferHeight] = window->GetFramebufferSize();

    vulkanRenderer.Init(
        window->GetNativeWindow(),
        framebufferWidth,
        framebufferHeight
    );

    imguiLayer.Init(window->GetNativeWindow(), vulkanRenderer);
    vulkanRenderer.SetImGuiLayer(&imguiLayer);

    vulkanRenderer.SetImGuiRenderCallback(
        [this](VkCommandBuffer commandBuffer)
        {
            imguiLayer.Render(commandBuffer);
        }
    );

    OnInit();

    return true;
}

bool Application::IsKeyDown(int key) const
{
    return glfwGetKey(window->GetNativeWindow(), key) == GLFW_PRESS;
}

bool Application::IsKeyboardCapturedByUI() const
{
    return imguiLayer.WantsKeyboardCapture();
}

void Application::Run()
{
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window->ShouldClose())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        float deltaTime = std::chrono::duration<float>(
            currentTime - lastTime
        ).count();

        lastTime = currentTime;

        window->PollEvents();
        imguiLayer.BeginFrame();

        if (window->WasResized())
        {
            auto [framebufferWidth, framebufferHeight] = window->GetFramebufferSize();

            vulkanRenderer.OnFramebufferResized(
                framebufferWidth,
                framebufferHeight
            );

            window->ResetResizeFlag();
        }

        const SceneViewportState& viewportState = editorLayer->GetSceneViewportState();
        if (viewportState.visible)
            vulkanRenderer.EnsureSceneViewportTarget(viewportState.width, viewportState.height);

        editorLayer->Draw(GetEditorScene(), vulkanRenderer.GetSceneViewportTextureID());

        OnUpdate(deltaTime);

        vulkanRenderer.BeginFrame();
        OnRender(vulkanRenderer);

        imguiLayer.EndFrame();
        vulkanRenderer.EndFrame();
    }

    isRunning = false;
    Shutdown();
}

void Application::Shutdown()
{
    if (isShutdown)
        return;

    OnShutdown();
    imguiLayer.Shutdown();
    vulkanRenderer.Cleanup();

    isShutdown = true;
}

const SceneViewportState& Application::GetSceneViewportState() const
{
    return editorLayer->GetSceneViewportState();
}
