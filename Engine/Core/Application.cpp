#include "Application.h"

#include <chrono>

#include "Editor/EditorLayer.h"

Application::Application()
{
    window = std::make_unique<Window>(1920, 1080, "Vulkan Engine Window");
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

        editorLayer->Draw(
            GetEditorScene(),
            vulkanRenderer,
            vulkanRenderer.GetSceneViewportTextureID(),
            IsEditorPlaying()
        );

        if (editorLayer->ConsumePlayRequest())
            OnEditorPlay();

        if (editorLayer->ConsumeStopRequest())
            OnEditorStop();

        inputState.BeginFrame(
            window->GetNativeWindow(),
            viewportState.visible,
            viewportState.focused,
            imguiLayer.WantsTextInput(),
            IsEditorPlaying()
        );

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
