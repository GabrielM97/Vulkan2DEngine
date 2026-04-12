#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <entt/entt.hpp>

#include "Camera2D.h"
#include "GameObjectHandle.h"
#include "SceneComponents.h"
#include "SpriteAnimationSet.h"

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
    GameObjectHandle CreateGameObject(const std::string& name = "GameObject", GameObjectID parentID = 0);
    GameObjectHandle GetGameObject(GameObjectID id);
    GameObjectHandle GetGameObjectHandle(GameObjectID id);
    GameObjectHandle CreateGameObjectHandle(const std::string& name = "GameObject", GameObjectID parentID = 0);

    bool IsValidGameObject(GameObjectID id) const;
    bool IsValidHandle(entt::entity entity, GameObjectID id) const;

    void DestroyGameObject(GameObjectID id);
    size_t GetGameObjectCount() const;

    void Update(float deltaTime);
    void Render(IRenderer2D& renderer);
    void UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight);

    std::string GetGameObjectName(GameObjectID id) const;
    bool SetGameObjectName(GameObjectID id, const std::string& name);

    bool IsGameObjectActive(GameObjectID id) const;
    bool SetGameObjectActive(GameObjectID id, bool active);

    std::vector<GameObjectID> GetGameObjectIDs() const;
    std::vector<GameObjectID> GetRootGameObjects() const;
    std::vector<GameObjectID> GetChildGameObjects(GameObjectID parentID) const;
    bool HasChildren(GameObjectID parentID) const;
    bool HasParent(GameObjectID id) const;
    GameObjectID GetParentID(GameObjectID id) const;
    bool SetParent(GameObjectID childID, GameObjectID parentID);
    bool ClearParent(GameObjectID childID);
    bool SetSiblingOrder(GameObjectID id, int order);

    Transform2D GetWorldTransform(GameObjectID id) const;
    bool SetWorldTransform(GameObjectID id, const Transform2D& transform);

    LocalTransformComponent GetLocalTransform(GameObjectID id) const;
    bool SetLocalTransform(GameObjectID id, const LocalTransformComponent& transform);

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

    Camera2D& GetCamera() { return m_Camera; }
    const Camera2D& GetCamera() const { return m_Camera; }

private:
    entt::entity CreateGameObjectEntity(const std::string& name);
    entt::entity FindEntityByID(GameObjectID id) const;

    const SpriteAnimationSet* GetOrLoadAnimationSet(const std::string& path);
    void MarkTransformDirty(GameObjectID id);
    Transform2D ResolveWorldTransform(entt::entity entity);
    std::vector<entt::entity> SortForRendering() const;
    void DestroyPendingGameObjects();
    bool WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const;

    entt::registry m_Registry;
    std::unordered_map<GameObjectID, entt::entity> m_EntityByID;
    Camera2D m_Camera;
    std::unordered_map<std::string, SpriteAnimationSet> m_AnimationSetCache;
    GameObjectID m_NextGameObjectID = 1;
};
