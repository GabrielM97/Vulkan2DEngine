#pragma once

#include "Camera2D.h"
#include "GameObject.h"
#include "SpriteAnimationSet.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct CameraCommand
{
    float moveX = 0.0f;
    float moveY = 0.0f;
    float zoomDelta = 0.0f;
};

class IRenderer2D;

class Scene
{
public:
    GameObject& CreateGameObject(const std::string& name = "GameObject", GameObjectID parentID = 0);

    void Render(IRenderer2D& renderer);
    void Update(float deltaTime);
    void UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight);

    void DestroyGameObject(GameObject& object);
    void DestroyGameObject(GameObjectID id);
    size_t GetGameObjectCount() const;
    GameObject* GetGameObject(size_t index);
    const GameObject* GetGameObject(size_t index) const;

    GameObject* FindGameObjectByID(GameObjectID id);
    const GameObject* FindGameObjectByID(GameObjectID id) const;
    GameObject* FindGameObjectByName(const std::string& name);
    const GameObject* FindGameObjectByName(const std::string& name) const;
    
    std::vector<std::unique_ptr<GameObject>>& GetGameObjects();
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

    Camera2D& GetCamera() { return m_Camera; }
    const Camera2D& GetCamera() const { return m_Camera; }
    
    bool SetParent(GameObjectID childID, GameObjectID parentID);
    bool ClearParent(GameObjectID childID);
    Transform2D GetWorldTransform(GameObjectID id) const;

private:
    const SpriteAnimationSet* GetOrLoadAnimationSet(const std::string& path);
    std::vector<const GameObject*> SortForRendering();
    void DestroyPendingGameObjects();
    bool WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const;
    
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
    Camera2D m_Camera;
    std::unordered_map<std::string, SpriteAnimationSet> m_AnimationSetCache;
    GameObjectID m_NextGameObjectID = 1;
};
