#pragma once

#include <string>

#include "Object.h"

class Actor : public Object
{
public:
    Actor() = default;
    using Object::Object;
    ~Actor() override = default;

    bool HasParent() const { return GetEntity().HasParent(); }
    GameObjectID GetParentID() const { return GetEntity().GetParentID(); }
    void SetParent(GameObjectID parentID) { GetEntity().SetParent(parentID); }
    void ClearParent() { GetEntity().ClearParent(); }

    Transform2D GetTransform() const { return GetEntity().GetTransform(); }
    void SetTransform(const Transform2D& transform) { GetEntity().SetTransform(transform); }

    glm::vec2 GetPosition() const { return GetEntity().GetPosition(); }
    void SetPosition(const glm::vec2& position) { GetEntity().SetPosition(position); }
    glm::vec2 GetLocalPosition() const { return GetEntity().GetLocalPosition(); }
    void SetLocalPosition(const glm::vec2& position) { GetEntity().SetLocalPosition(position); }
    
    float GetRotation() const { return GetEntity().GetRotation(); }
    void SetRotation(float rotInDeg) { GetEntity().SetRotation(rotInDeg); }
    float GetLocalRotation() const { return GetEntity().GetLocalRotation(); }
    void SetLocalRotation(float rotInDeg) { GetEntity().SetLocalRotation(rotInDeg); }
    
    glm::vec2 GetScale() const { return GetEntity().GetScale(); }
    void SetScale(const glm::vec2& scale) { GetEntity().SetScale(scale); }
    glm::vec2 GetLocalScale() const { return GetEntity().GetLocalScale(); }
    void SetLocalScale(const glm::vec2& scale) { GetEntity().SetLocalScale(scale); }
    
    Transform2D GetWorldTransform() const { return GetTransform(); }
    void SetWorldTransform(const Transform2D& transform) { SetTransform(transform); }
    Transform2D GetLocalTransform() const { return GetEntity().GetLocalTransform(); }
    void SetLocalTransform(const Transform2D& transform) { GetEntity().SetLocalTransform(transform); }

    std::string GetSpriteTexturePath() const { return GetEntity().GetSpriteTexturePath(); }
    IntRect GetSpriteSourceRect() const { return GetEntity().GetSpriteSourceRect(); }
    bool SpriteUsesSourceRect() const { return GetEntity().SpriteUsesSourceRect(); }
    glm::vec2 GetSpriteSize() const { return GetEntity().GetSpriteSize(); }
    glm::vec4 GetSpriteTint() const { return GetEntity().GetSpriteTint(); }
    int GetSpriteLayer() const { return GetEntity().GetSpriteLayer(); }
    bool IsSpriteVisible() const { return GetEntity().IsSpriteVisible(); }
    bool IsSpriteFlippedX() const { return GetEntity().IsSpriteFlippedX(); }
    bool IsSpriteFlippedY() const { return GetEntity().IsSpriteFlippedY(); }

    void SetSpriteTexturePath(const std::string& path) { GetEntity().SetSpriteTexturePath(path); }
    void SetSpriteSourceRect(int x, int y, int width, int height)
    {
        GetEntity().SetSpriteSourceRect(x, y, width, height);
    }
    
    void SetSpriteSourceRectFromGrid(int x, int y, int width, int height)
    {
        GetEntity().SetSpriteSourceRectFromGrid(x, y, width, height);
    } 
    
    void SetSpriteSourceRectFromGrid(int x, int y)
    {
        GetEntity().SetSpriteSourceRectFromGrid(x, y);
    }

    void ClearSpriteSourceRect() { GetEntity().ClearSpriteSourceRect(); }
    void SetSpriteSize(const glm::vec2& size) { GetEntity().SetSpriteSize(size); }
    void SetSpriteTint(const glm::vec4& tint) { GetEntity().SetSpriteTint(tint); }
    void SetSpriteLayer(int layer) { GetEntity().SetSpriteLayer(layer); }
    void SetSpriteVisible(bool visible) { GetEntity().SetSpriteVisible(visible); }
    void SetSpriteFlipX(bool flip) { GetEntity().SetSpriteFlipX(flip); }
    void SetSpriteFlipY(bool flip) { GetEntity().SetSpriteFlipY(flip); }
    
    void ConfigureSprite(
        const std::string& texturePath,
        const glm::vec2& size = glm::vec2{1.0f, 1.0f},
        const glm::vec4& tint = glm::vec4{1.0f},
        int layer = 0)
    {
        GetEntity().SetSpriteTexturePath(texturePath);
        GetEntity().SetSpriteSize(size);
        GetEntity().SetSpriteTint(tint);
        GetEntity().SetSpriteLayer(layer);
    }

    bool HasAnimation() const { return GetEntity().HasAnimation(); }
    void EnsureAnimation() { GetEntity().EnsureAnimation(); }
    void RemoveAnimation() { GetEntity().RemoveAnimation(); }
    std::string GetAnimationSetPath() const { return GetEntity().GetAnimationSetPath(); }
    std::string GetAnimationClipName() const { return GetEntity().GetAnimationClipName(); }
    void SetAnimationSetPath(const std::string& path) { GetEntity().SetAnimationSetPath(path); }
    void PlayAnimation(const std::string& clipName, bool restartIfSame = false)
    {
        GetEntity().PlayAnimation(clipName, restartIfSame);
    }
    void StopAnimation() { GetEntity().StopAnimation(); }
    void ResetAnimation() { GetEntity().ResetAnimation(); }
    bool IsAnimationPlaying() const { return GetEntity().IsAnimationPlaying(); }
    bool HasAnimationFinished() const { return GetEntity().HasAnimationFinished(); }
    bool IsPlayingAnimationClip(const std::string& clipName) const
    {
        return GetEntity().IsPlayingAnimationClip(clipName);
    }
    
    virtual void Initialize() = 0;
};
