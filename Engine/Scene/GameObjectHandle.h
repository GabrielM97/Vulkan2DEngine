#pragma once

#include "GameObject.h"

#include <string>
#include <glm/glm.hpp>

class Scene;

class GameObjectHandle
{
public:
    GameObjectHandle() = default;
    GameObjectHandle(Scene* scene, GameObjectID id)
        : m_Scene(scene), m_ID(id)
    {
    }

    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }
    GameObjectID GetID() const { return m_ID; }

    bool IsActive() const;
    void SetActive(bool active) const;
    void Destroy() const;
    bool HasParent() const;
    GameObjectID GetParentID() const;
    void SetParent(GameObjectID parentID) const;
    void ClearParent() const;

    Transform2D GetLocalTransform() const;
    Transform2D GetWorldTransform() const;

    glm::vec2 GetLocalPosition() const;
    glm::vec2 GetLocalScale() const;
    glm::vec2 GetLocalPivot() const;
    float GetLocalRotation() const;
    glm::vec2 GetWorldPosition() const;
    glm::vec2 GetWorldScale() const;
    glm::vec2 GetWorldPivot() const;
    float GetWorldRotation() const;

    void SetLocalTransform(const Transform2D& transform) const;
    void SetWorldTransform(const Transform2D& transform) const;

    void SetLocalPosition(const glm::vec2& position) const;
    void SetLocalScale(const glm::vec2& scale) const;
    void SetLocalPivot(const glm::vec2& pivot) const;
    void SetLocalRotation(float rotationDegrees) const;
    void SetWorldPosition(const glm::vec2& position) const;
    void SetWorldScale(const glm::vec2& scale) const;
    void SetWorldPivot(const glm::vec2& pivot) const;
    void SetWorldRotation(float rotationDegrees) const;

    void TranslateLocal(const glm::vec2& delta) const;

    std::string GetSpriteTexturePath() const;
    IntRect GetSpriteSourceRect() const;
    bool SpriteUsesSourceRect() const;
    glm::vec2 GetSpriteSize() const;
    glm::vec4 GetSpriteTint() const;
    int GetSpriteLayer() const;
    bool IsSpriteVisible() const;
    bool IsSpriteFlippedX() const;
    bool IsSpriteFlippedY() const;

    void SetSpriteTexturePath(const std::string& path) const;
    void SetSpriteSourceRect(int x, int y, int width, int height) const;
    void SetSpriteSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight) const;
    void ClearSpriteSourceRect() const;
    void SetSpriteSize(const glm::vec2& size) const;
    void SetSpriteTint(const glm::vec4& tint) const;
    void SetSpriteLayer(int layer) const;
    void SetSpriteVisible(bool visible) const;
    void SetSpriteFlipX(bool flip) const;
    void SetSpriteFlipY(bool flip) const;

    bool HasAnimation() const;
    void EnsureAnimation() const;
    void RemoveAnimation() const;
    std::string GetAnimationSetPath() const;
    std::string GetAnimationClipName() const;
    void SetAnimationSetPath(const std::string& path) const;
    void PlayAnimation(const std::string& clipName, bool restartIfSame = false) const;
    void StopAnimation() const;
    void ResetAnimation() const;
    bool IsAnimationPlaying() const;
    bool HasAnimationFinished() const;
    bool IsPlayingAnimationClip(const std::string& clipName) const;

    std::string GetName() const;
    void SetName(const std::string& name) const;

private:
    Scene* m_Scene = nullptr;
    GameObjectID m_ID = 0;
};
