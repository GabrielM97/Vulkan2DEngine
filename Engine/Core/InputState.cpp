#include "InputState.h"

bool InputState::IsValidKey(int key)
{
    return key >= 0 && key <= GLFW_KEY_LAST;
}

void InputState::BeginFrame(
    GLFWwindow* window,
    bool viewportVisible,
    bool viewportFocused,
    bool textInputActive,
    bool editorPlaying
)
{
    m_PreviousKeys = m_CurrentKeys;
    m_ViewportVisible = viewportVisible;
    m_ViewportFocused = viewportFocused;
    m_TextInputActive = textInputActive;
    m_IsEditorPlaying = editorPlaying;

    if (window == nullptr)
    {
        m_CurrentKeys.fill(false);
        return;
    }

    for (int key = 0; key <= GLFW_KEY_LAST; ++key)
        m_CurrentKeys[key] = glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputState::IsKeyDown(int key) const
{
    return IsValidKey(key) && m_CurrentKeys[key];
}

bool InputState::WasKeyPressed(int key) const
{
    return IsValidKey(key) && m_CurrentKeys[key] && !m_PreviousKeys[key];
}

bool InputState::WasKeyReleased(int key) const
{
    return IsValidKey(key) && !m_CurrentKeys[key] && m_PreviousKeys[key];
}

bool InputState::CanUseViewportInput() const
{
    return m_ViewportVisible && m_ViewportFocused;
}

bool InputState::CanUseEditorViewportInput() const
{
    return !m_IsEditorPlaying && CanUseViewportInput();
}

bool InputState::CanUseRuntimeViewportInput() const
{
    return m_IsEditorPlaying && CanUseViewportInput();
}

bool InputState::CanUseEditorShortcuts() const
{
    return !m_IsEditorPlaying && !m_TextInputActive;
}
