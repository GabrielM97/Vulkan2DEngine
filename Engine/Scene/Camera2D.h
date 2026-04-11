#pragma once

#include <algorithm>
#include <glm/vec2.hpp>

class Camera2D
{
public:
    Camera2D() = default;

    const glm::vec2& GetTopLeftPosition() const { return m_TopLeftPosition; }
    float GetZoom() const { return m_Zoom; }

    void SetTopLeftPosition(const glm::vec2& position);
    glm::vec2 GetCenter(float viewWidth, float viewHeight) const;
    void SetZoom(float zoom);
    void Move(const glm::vec2& delta);
    void Move(float deltaX, float deltaY);
    void AddZoom(float deltaZoom, float viewportWidth, float viewportHeight);
    void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
    void SetZoomSpeed(float speed) { m_ZoomSpeed = speed; }

    float GetMoveSpeed() const { return m_MoveSpeed; }
    float GetZoomSpeed() const { return m_ZoomSpeed; }

    void SetZoomLimits(float minZoom, float maxZoom);
    void Update(float moveX, float moveY, float zoomDelta, float deltaTime, float viewportWidth, float viewportHeight);

private:
    glm::vec2 m_TopLeftPosition{ 0.0f, 0.0f };
    float m_Zoom = 1.0f;

    float m_MoveSpeed = 400.0f;
    float m_ZoomSpeed = 1.5f;
    float m_MinZoom = 0.25f;
    float m_MaxZoom = 4.0f;
};
