#pragma once

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
    
    virtual void Initialize() = 0;
};
