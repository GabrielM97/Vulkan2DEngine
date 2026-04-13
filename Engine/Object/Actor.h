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

    Transform2D GetWorldTransform() const { return GetEntity().GetWorldTransform(); }
    LocalTransformComponent GetLocalTransform() const { return GetEntity().GetLocalTransform(); }
    void SetWorldTransform(const Transform2D& transform) { GetEntity().SetWorldTransform(transform); }
    void SetLocalTransform(const LocalTransformComponent& transform) { GetEntity().SetLocalTransform(transform); }

    virtual void Initialize() = 0;
};
