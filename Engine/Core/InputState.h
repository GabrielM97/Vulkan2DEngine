#pragma once

#include <array>

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

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
    
    bool IsMouseButtonDown(int button) const;
    bool WasMouseButtonPressed(int button) const;
    bool WasMouseButtonReleased(int button) const;
    static bool IsValidMouseButton(int button);

    glm::vec2 GetMouseScreenPosition() const { return m_MouseScreenPosition; }

private:
    static bool IsValidKey(int key);

    std::array<bool, GLFW_KEY_LAST + 1> m_CurrentKeys{};
    std::array<bool, GLFW_KEY_LAST + 1> m_PreviousKeys{};
    bool m_ViewportVisible = false;
    bool m_ViewportFocused = false;
    bool m_TextInputActive = false;
    bool m_IsEditorPlaying = false;
    
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> m_CurrentMouseButtons{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> m_PreviousMouseButtons{};
    glm::vec2 m_MouseScreenPosition{0.0f, 0.0f};
};
