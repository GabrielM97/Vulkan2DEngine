#include "InputState.h"

bool InputState::IsMouseButtonDown(int button) const
{
    return IsValidMouseButton(button) && m_CurrentMouseButtons[button];
}

bool InputState::WasMouseButtonPressed(int button) const
{
    return IsValidMouseButton(button) && m_CurrentMouseButtons[button] && !m_PreviousMouseButtons[button];
}

bool InputState::IsValidMouseButton(int button)
{
    return button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST;
}

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
    
    m_PreviousMouseButtons = m_CurrentMouseButtons;
    
    m_PreviousKeys = m_CurrentKeys;
    m_ViewportVisible = viewportVisible;
    m_ViewportFocused = viewportFocused;
    m_TextInputActive = textInputActive;
    m_IsEditorPlaying = editorPlaying;

    if (window == nullptr)
    {
        m_CurrentKeys.fill(false);
        m_CurrentMouseButtons.fill(false);
        return;
    }

    for (int key = 0; key <= GLFW_KEY_LAST; ++key)
        m_CurrentKeys[key] = glfwGetKey(window, key) == GLFW_PRESS;
    
    for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
        m_CurrentMouseButtons[button] = glfwGetMouseButton(window, button) == GLFW_PRESS;

    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    m_MouseScreenPosition = glm::vec2{
        static_cast<float>(mouseX),
        static_cast<float>(mouseY)
    };
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
