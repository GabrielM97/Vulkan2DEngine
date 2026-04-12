#pragma once

#include "Camera2D.h"
#include "GameObject.h"
#include "SpriteAnimationSet.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "GameObjectHandle.h"

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
    std::vector<const GameObject*> GetRootGameObjects() const;
    std::vector<const GameObject*> GetChildGameObjects(GameObjectID parentID) const;
    bool HasChildren(GameObjectID parentID) const;
    bool SetParent(GameObjectID childID, GameObjectID parentID);
    bool ClearParent(GameObjectID childID);
    
    std::vector<std::unique_ptr<GameObject>>& GetGameObjects();
    const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;

    Camera2D& GetCamera() { return m_Camera; }
    const Camera2D& GetCamera() const { return m_Camera; }
    
    Transform2D GetWorldTransform(GameObjectID id) const;
    bool SetWorldTransform(GameObjectID id, const Transform2D& transform);
    Transform2D GetLocalTransform(GameObjectID id) const;
    bool SetLocalTransform(GameObjectID id, const Transform2D& transform);
    glm::vec2 GetLocalPosition(GameObjectID id) const;
    glm::vec2 GetLocalScale(GameObjectID id) const;
    glm::vec2 GetLocalPivot(GameObjectID id) const;
    float GetLocalRotation(GameObjectID id) const;
    glm::vec2 GetWorldPosition(GameObjectID id) const;
    glm::vec2 GetWorldScale(GameObjectID id) const;
    glm::vec2 GetWorldPivot(GameObjectID id) const;
    float GetWorldRotation(GameObjectID id) const;
    bool SetLocalPosition(GameObjectID id, const glm::vec2& position);
    bool SetLocalScale(GameObjectID id, const glm::vec2& scale);
    bool SetLocalPivot(GameObjectID id, const glm::vec2& pivot);
    bool SetLocalRotation(GameObjectID id, float rotationDegrees);
    bool SetWorldPosition(GameObjectID id, const glm::vec2& position);
    bool SetWorldScale(GameObjectID id, const glm::vec2& scale);
    bool SetWorldPivot(GameObjectID id, const glm::vec2& pivot);
    bool SetWorldRotation(GameObjectID id, float rotationDegrees);
    
    bool SetSiblingOrder(GameObjectID id, int order);
    bool SetGameObjectName(GameObjectID id, const std::string& name);
    bool SetGameObjectActive(GameObjectID id, bool active);
    std::string GetSpriteTexturePath(GameObjectID id) const;
    IntRect GetSpriteSourceRect(GameObjectID id) const;
    bool SpriteUsesSourceRect(GameObjectID id) const;
    glm::vec2 GetSpriteSize(GameObjectID id) const;
    glm::vec4 GetSpriteTint(GameObjectID id) const;
    int GetSpriteLayer(GameObjectID id) const;
    bool IsSpriteVisible(GameObjectID id) const;
    bool IsSpriteFlippedX(GameObjectID id) const;
    bool IsSpriteFlippedY(GameObjectID id) const;
    bool SetSpriteTexturePath(GameObjectID id, const std::string& path);
    bool SetSpriteSourceRect(GameObjectID id, int x, int y, int width, int height);
    bool SetSpriteSourceRectFromGrid(GameObjectID id, int column, int row, int cellWidth, int cellHeight);
    bool ClearSpriteSourceRect(GameObjectID id);
    bool SetSpriteSize(GameObjectID id, const glm::vec2& size);
    bool SetSpriteTint(GameObjectID id, const glm::vec4& tint);
    bool SetSpriteLayer(GameObjectID id, int layer);
    bool SetSpriteVisible(GameObjectID id, bool visible);
    bool SetSpriteFlipX(GameObjectID id, bool flip);
    bool SetSpriteFlipY(GameObjectID id, bool flip);

    bool HasAnimation(GameObjectID id) const;
    bool EnsureAnimation(GameObjectID id);
    bool RemoveAnimation(GameObjectID id);
    std::string GetAnimationSetPath(GameObjectID id) const;
    std::string GetAnimationClipName(GameObjectID id) const;
    bool SetAnimationSetPath(GameObjectID id, const std::string& path);
    bool PlayAnimation(GameObjectID id, const std::string& clipName, bool restartIfSame = false);
    bool StopAnimation(GameObjectID id);
    bool ResetAnimation(GameObjectID id);
    bool IsAnimationPlaying(GameObjectID id) const;
    bool HasAnimationFinished(GameObjectID id) const;
    bool IsPlayingAnimationClip(GameObjectID id, const std::string& clipName) const;
    
    GameObjectHandle GetGameObjectHandle(GameObjectID id);
    GameObjectHandle CreateGameObjectHandle(const std::string& name = "GameObject", GameObjectID parentID = 0);
    

private:
    const SpriteAnimationSet* GetOrLoadAnimationSet(const std::string& path);
    void MarkTransformDirty(GameObjectID id);
    Transform2D ResolveWorldTransform(GameObject& object);
    std::vector<const GameObject*> SortForRendering();
    void DestroyPendingGameObjects();
    bool WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const;
    
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
    Camera2D m_Camera;
    std::unordered_map<std::string, SpriteAnimationSet> m_AnimationSetCache;
    GameObjectID m_NextGameObjectID = 1;
};
