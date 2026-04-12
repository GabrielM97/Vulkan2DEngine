#pragma once

#include <cstdint>
#include <optional>

#include "SpriteRenderer.h"
#include "SpriteAnimation.h"

#include <string>

#include "Math/Transform2D.h"

using GameObjectID = uint64_t;

class GameObject
{
public:
    GameObjectID GetID() const { return id; }
    std::string GetName() const{return name;}
    void SetName(const std::string& inName){name = inName;}
    
    void setActive(bool inActive){active = inActive;}
    bool isActive() const { return active; }
    
    void Destroy()
    {
        pendingDestroy = true;
        active = false;
    }
    
    bool isPendingDestroy() const { return pendingDestroy; }
    
    GameObjectID GetParentID() const { return parentID; }
    bool HasParent() const { return parentID != 0; }
    void ClearParent() { parentID = 0; }

    int GetSiblingOrder() const { return siblingOrder; }
    void SetSiblingOrder(int order) { siblingOrder = order; }
    
    Transform2D transform;
    SpriteRenderer sprite;
    
    std::optional<SpriteAnimation> animation;
    
private:
    void SetID(GameObjectID inID){id = inID;}
    void SetParentID(GameObjectID inParentID) { parentID = inParentID; }
    
    GameObjectID id = 0;
    std::string name = "GameObject";
    bool active = true;
    bool pendingDestroy = false;
    GameObjectID parentID = 0;
    int siblingOrder = 0;
    Transform2D cachedWorldTransform{};
    bool transformDirty = true;

    friend class Scene;
};
