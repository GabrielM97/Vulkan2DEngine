#pragma once

#include <algorithm>
#include <glm/vec2.hpp>

class Camera2D
{
public:
    Camera2D() = default;

    const glm::vec2& GetPosition() const { return m_Position; }
    float GetZoom() const { return m_Zoom; }

    void SetPosition(const glm::vec2& position)
    {
        m_Position = position;
    }

    void SetZoom(float zoom)
    {
        m_Zoom = std::clamp(zoom, m_MinZoom, m_MaxZoom);
    }

    void Move(const glm::vec2& delta)
    {
        m_Position += delta;
    }

    void Move(float deltaX, float deltaY)
    {
        m_Position.x += deltaX;
        m_Position.y += deltaY;
    }

    void AddZoom(float deltaZoom)
    {
        SetZoom(m_Zoom + deltaZoom);
    }

    void SetMoveSpeed(float speed)
    {
        m_MoveSpeed = speed;
    }

    void SetZoomSpeed(float speed)
    {
        m_ZoomSpeed = speed;
    }

    float GetMoveSpeed() const { return m_MoveSpeed; }
    float GetZoomSpeed() const { return m_ZoomSpeed; }

    void SetZoomLimits(float minZoom, float maxZoom)
    {
        m_MinZoom = minZoom;
        m_MaxZoom = maxZoom;
        m_Zoom = std::clamp(m_Zoom, m_MinZoom, m_MaxZoom);
    }

    void Update(float moveX, float moveY, float zoomDelta, float deltaTime)
    {
        m_Position.x += moveX * m_MoveSpeed * deltaTime;
        m_Position.y += moveY * m_MoveSpeed * deltaTime;
        m_Zoom = std::clamp(
            m_Zoom + zoomDelta * m_ZoomSpeed * deltaTime,
            m_MinZoom,
            m_MaxZoom
        );
    }

private:
    glm::vec2 m_Position{ 0.0f, 0.0f };
    float m_Zoom = 1.0f;

    float m_MoveSpeed = 400.0f;
    float m_ZoomSpeed = 1.5f;
    float m_MinZoom = 0.25f;
    float m_MaxZoom = 4.0f;
};