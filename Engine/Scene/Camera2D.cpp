#include "Camera2D.h"

void Camera2D::SetTopLeftPosition(const glm::vec2& position)
{
    m_TopLeftPosition = position;
}

glm::vec2 Camera2D::GetCenter(float viewWidth, float viewHeight) const
{
    return m_TopLeftPosition + glm::vec2(viewWidth * 0.5f, viewHeight * 0.5f);
}

void Camera2D::SetZoom(float zoom)
{
    m_Zoom = std::clamp(zoom, m_MinZoom, m_MaxZoom);
}

void Camera2D::Move(const glm::vec2& delta)
{
    m_TopLeftPosition += delta;
}

void Camera2D::Move(float deltaX, float deltaY)
{
    m_TopLeftPosition.x += deltaX;
    m_TopLeftPosition.y += deltaY;
}

void Camera2D::AddZoom(float deltaZoom, float viewportWidth, float viewportHeight)
{
    if (viewportWidth <= 0.0f || viewportHeight <= 0.0f)
    {
        SetZoom(m_Zoom + deltaZoom);
        return;
    }

    const float oldViewWidth = viewportWidth / m_Zoom;
    const float oldViewHeight = viewportHeight / m_Zoom;
    const glm::vec2 center = GetCenter(oldViewWidth, oldViewHeight);

    SetZoom(m_Zoom + deltaZoom);

    const float newViewWidth = viewportWidth / m_Zoom;
    const float newViewHeight = viewportHeight / m_Zoom;
    m_TopLeftPosition = center - glm::vec2(newViewWidth * 0.5f, newViewHeight * 0.5f);
}

void Camera2D::SetZoomLimits(float minZoom, float maxZoom)
{
    m_MinZoom = minZoom;
    m_MaxZoom = maxZoom;
    m_Zoom = std::clamp(m_Zoom, m_MinZoom, m_MaxZoom);
}

void Camera2D::Update(float moveX, float moveY, float zoomDelta, float deltaTime, float viewportWidth, float viewportHeight)
{
    Move(
        moveX * m_MoveSpeed * deltaTime,
        moveY * m_MoveSpeed * deltaTime
    );

    AddZoom(
        zoomDelta * m_ZoomSpeed * deltaTime,
        viewportWidth,
        viewportHeight
    );
}
