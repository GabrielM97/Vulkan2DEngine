#pragma once

#include <array>

#include <GLFW/glfw3.h>

class InputState
{
public:
    void BeginFrame(
        GLFWwindow* window,
        bool viewportVisible,
        bool viewportFocused,
        bool textInputActive,
        bool editorPlaying
    );

    bool IsKeyDown(int key) const;
    bool WasKeyPressed(int key) const;
    bool WasKeyReleased(int key) const;

    bool CanUseViewportInput() const;
    bool CanUseEditorViewportInput() const;
    bool CanUseRuntimeViewportInput() const;
    bool CanUseEditorShortcuts() const;

    bool IsTextInputActive() const { return m_TextInputActive; }
    bool IsEditorPlaying() const { return m_IsEditorPlaying; }
    bool IsViewportVisible() const { return m_ViewportVisible; }
    bool IsViewportFocused() const { return m_ViewportFocused; }

private:
    static bool IsValidKey(int key);

    std::array<bool, GLFW_KEY_LAST + 1> m_CurrentKeys{};
    std::array<bool, GLFW_KEY_LAST + 1> m_PreviousKeys{};
    bool m_ViewportVisible = false;
    bool m_ViewportFocused = false;
    bool m_TextInputActive = false;
    bool m_IsEditorPlaying = false;
};
