#include "Scene.h"

#include <algorithm>
#include <iostream>

#include "Math/TransformMath2D.h"
#include "Renderer/IRenderer2D.h"

entt::entity Scene::CreateGameObjectEntity(const std::string& name)
{
    const entt::entity entity = m_Registry.create();
    const GameObjectID id = m_NextGameObjectID++;

    auto& idComponent = m_Registry.emplace<IDComponent>(entity);
    idComponent.id = id;

    auto& nameComponent = m_Registry.emplace<NameComponent>(entity);
    nameComponent.name = name;

    m_Registry.emplace<ActiveComponent>(entity);
    auto& relationship = m_Registry.emplace<RelationshipComponent>(entity);
    relationship.parentID = 0;
    relationship.siblingOrder = static_cast<int>(m_EntityByID.size());

    m_Registry.emplace<LocalTransformComponent>(entity);
    m_Registry.emplace<WorldTransformComponent>(entity);
    m_Registry.emplace<SpriteComponent>(entity);

    m_EntityByID.emplace(id, entity);
    return entity;
}

entt::entity Scene::FindEntityByID(GameObjectID id) const
{
    auto it = m_EntityByID.find(id);
    return it != m_EntityByID.end() ? it->second : entt::null;
}

GameObjectHandle Scene::CreateGameObject(const std::string& name, GameObjectID parentID)
{
    const entt::entity entity = CreateGameObjectEntity(name);
    const GameObjectID id = m_Registry.get<IDComponent>(entity).id;

    if (parentID != 0)
        SetParent(id, parentID);

    return GameObjectHandle(this, entity, id);
}

GameObjectHandle Scene::GetGameObject(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return {};

    return GameObjectHandle(this, entity, id);
}

GameObjectHandle Scene::GetGameObjectHandle(GameObjectID id)
{
    return GetGameObject(id);
}

GameObjectHandle Scene::CreateGameObjectHandle(const std::string& name, GameObjectID parentID)
{
    return CreateGameObject(name, parentID);
}

bool Scene::IsValidGameObject(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null && m_Registry.valid(entity);
}

bool Scene::IsValidHandle(entt::entity entity, GameObjectID id) const
{
    if (entity == entt::null || !m_Registry.valid(entity))
        return false;

    const IDComponent* idComponent = m_Registry.try_get<IDComponent>(entity);
    return idComponent != nullptr && idComponent->id == id;
}

void Scene::DestroyGameObject(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return;

    auto& active = m_Registry.get<ActiveComponent>(entity);
    active.pendingDestroy = true;
    active.active = false;
}

size_t Scene::GetGameObjectCount() const
{
    return m_EntityByID.size();
}

void Scene::Update(float deltaTime)
{
    auto view = m_Registry.view<ActiveComponent, SpriteComponent, SpriteAnimationComponent>();
    for (entt::entity entity : view)
    {
        auto& active = view.get<ActiveComponent>(entity);
        auto& sprite = view.get<SpriteComponent>(entity);
        auto& animation = view.get<SpriteAnimationComponent>(entity);

        if (!active.active)
            continue;

        const SpriteAnimationSet* animationSet =
            GetOrLoadAnimationSet(animation.GetAnimationSetRef().path);

        animation.Update(deltaTime, sprite, animationSet);
    }

    DestroyPendingGameObjects();
}

void Scene::Render(IRenderer2D& renderer)
{
    renderer.SetCamera(m_Camera);

    for (entt::entity entity : SortForRendering())
    {
        const auto& id = m_Registry.get<IDComponent>(entity);
        const auto& sprite = m_Registry.get<SpriteComponent>(entity);
        renderer.DrawSprite(GetWorldTransform(id.id), sprite);
    }
}

void Scene::UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight)
{
    m_Camera.Update(
        command.moveX,
        command.moveY,
        command.zoomDelta,
        deltaTime,
        viewportWidth,
        viewportHeight
    );
}

std::string Scene::GetGameObjectName(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return {};

    return m_Registry.get<NameComponent>(entity).name;
}

bool Scene::SetGameObjectName(GameObjectID id, const std::string& name)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<NameComponent>(entity).name = name;
    return true;
}

bool Scene::IsGameObjectActive(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    return m_Registry.get<ActiveComponent>(entity).active;
}

bool Scene::SetGameObjectActive(GameObjectID id, bool active)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<ActiveComponent>(entity).active = active;
    return true;
}

std::vector<GameObjectID> Scene::GetGameObjectIDs() const
{
    std::vector<GameObjectID> ids;
    ids.reserve(m_EntityByID.size());

    auto view = m_Registry.view<IDComponent>();
    for (entt::entity entity : view)
        ids.push_back(view.get<IDComponent>(entity).id);

    return ids;
}

std::vector<GameObjectID> Scene::GetRootGameObjects() const
{
    std::vector<GameObjectID> roots;

    auto view = m_Registry.view<IDComponent, RelationshipComponent>();
    for (entt::entity entity : view)
    {
        const auto& id = view.get<IDComponent>(entity);
        const auto& relationship = view.get<RelationshipComponent>(entity);
        if (relationship.parentID == 0)
            roots.push_back(id.id);
    }

    std::stable_sort(
        roots.begin(),
        roots.end(),
        [this](GameObjectID a, GameObjectID b)
        {
            return m_Registry.get<RelationshipComponent>(FindEntityByID(a)).siblingOrder <
                   m_Registry.get<RelationshipComponent>(FindEntityByID(b)).siblingOrder;
        }
    );

    return roots;
}

std::vector<GameObjectID> Scene::GetChildGameObjects(GameObjectID parentID) const
{
    std::vector<GameObjectID> children;

    auto view = m_Registry.view<IDComponent, RelationshipComponent>();
    for (entt::entity entity : view)
    {
        const auto& id = view.get<IDComponent>(entity);
        const auto& relationship = view.get<RelationshipComponent>(entity);
        if (relationship.parentID == parentID)
            children.push_back(id.id);
    }

    std::stable_sort(
        children.begin(),
        children.end(),
        [this](GameObjectID a, GameObjectID b)
        {
            return m_Registry.get<RelationshipComponent>(FindEntityByID(a)).siblingOrder <
                   m_Registry.get<RelationshipComponent>(FindEntityByID(b)).siblingOrder;
        }
    );

    return children;
}

bool Scene::HasChildren(GameObjectID parentID) const
{
    auto view = m_Registry.view<RelationshipComponent>();
    for (entt::entity entity : view)
    {
        if (view.get<RelationshipComponent>(entity).parentID == parentID)
            return true;
    }

    return false;
}

bool Scene::HasParent(GameObjectID id) const
{
    return GetParentID(id) != 0;
}

GameObjectID Scene::GetParentID(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return 0;

    return m_Registry.get<RelationshipComponent>(entity).parentID;
}

bool Scene::SetParent(GameObjectID childID, GameObjectID parentID)
{
    if (childID == 0 || parentID == 0 || childID == parentID)
        return false;

    if (WouldCreateCycle(childID, parentID))
        return false;

    const entt::entity childEntity = FindEntityByID(childID);
    const entt::entity parentEntity = FindEntityByID(parentID);
    if (childEntity == entt::null || parentEntity == entt::null)
        return false;

    const Transform2D childWorld = GetWorldTransform(childID);
    const Transform2D parentWorld = GetWorldTransform(parentID);

    auto& relationship = m_Registry.get<RelationshipComponent>(childEntity);
    auto& local = m_Registry.get<LocalTransformComponent>(childEntity);
    const auto& childSprite = m_Registry.get<SpriteComponent>(childEntity);
    const auto& parentSprite = m_Registry.get<SpriteComponent>(parentEntity);

    relationship.parentID = parentID;
    local = TransformMath2D::ToLocalTransform(
        childWorld,
        childSprite.GetSize(),
        parentWorld,
        parentSprite.GetSize()
    );

    MarkTransformDirty(childID);
    return true;
}

bool Scene::ClearParent(GameObjectID childID)
{
    const entt::entity childEntity = FindEntityByID(childID);
    if (childEntity == entt::null)
        return false;

    auto& relationship = m_Registry.get<RelationshipComponent>(childEntity);
    auto& local = m_Registry.get<LocalTransformComponent>(childEntity);

    local = GetWorldTransform(childID);
    relationship.parentID = 0;

    MarkTransformDirty(childID);
    return true;
}

bool Scene::SetSiblingOrder(GameObjectID id, int order)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<RelationshipComponent>(entity).siblingOrder = order;
    return true;
}

Transform2D Scene::GetWorldTransform(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return {};

    return const_cast<Scene*>(this)->ResolveWorldTransform(entity);
}

bool Scene::SetWorldTransform(GameObjectID id, const Transform2D& transform)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    auto& local = m_Registry.get<LocalTransformComponent>(entity);
    const auto& relationship = m_Registry.get<RelationshipComponent>(entity);

    if (relationship.parentID == 0)
    {
        local = transform;
        MarkTransformDirty(id);
        return true;
    }

    const entt::entity parentEntity = FindEntityByID(relationship.parentID);
    if (parentEntity == entt::null)
    {
        local = transform;
        MarkTransformDirty(id);
        return true;
    }

    const auto& sprite = m_Registry.get<SpriteComponent>(entity);
    const auto& parentSprite = m_Registry.get<SpriteComponent>(parentEntity);
    const Transform2D parentWorld = GetWorldTransform(relationship.parentID);

    local = TransformMath2D::ToLocalTransform(
        transform,
        sprite.GetSize(),
        parentWorld,
        parentSprite.GetSize()
    );

    MarkTransformDirty(id);
    return true;
}

LocalTransformComponent Scene::GetLocalTransform(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return {};

    return m_Registry.get<LocalTransformComponent>(entity);
}

bool Scene::SetLocalTransform(GameObjectID id, const LocalTransformComponent& transform)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<LocalTransformComponent>(entity) = transform;
    MarkTransformDirty(id);
    return true;
}

glm::vec2 Scene::GetLocalPosition(GameObjectID id) const { return GetLocalTransform(id).position; }
glm::vec2 Scene::GetLocalScale(GameObjectID id) const { return GetLocalTransform(id).scale; }
glm::vec2 Scene::GetLocalPivot(GameObjectID id) const { return GetLocalTransform(id).pivot; }
float Scene::GetLocalRotation(GameObjectID id) const { return GetLocalTransform(id).rotationDegrees; }
glm::vec2 Scene::GetWorldPosition(GameObjectID id) const { return GetWorldTransform(id).position; }
glm::vec2 Scene::GetWorldScale(GameObjectID id) const { return GetWorldTransform(id).scale; }
glm::vec2 Scene::GetWorldPivot(GameObjectID id) const { return GetWorldTransform(id).pivot; }
float Scene::GetWorldRotation(GameObjectID id) const { return GetWorldTransform(id).rotationDegrees; }

bool Scene::SetLocalPosition(GameObjectID id, const glm::vec2& position)
{
    auto transform = GetLocalTransform(id);
    transform.position = position;
    return SetLocalTransform(id, transform);
}

bool Scene::SetLocalScale(GameObjectID id, const glm::vec2& scale)
{
    auto transform = GetLocalTransform(id);
    transform.scale = scale;
    return SetLocalTransform(id, transform);
}

bool Scene::SetLocalPivot(GameObjectID id, const glm::vec2& pivot)
{
    auto transform = GetLocalTransform(id);
    transform.pivot = pivot;
    return SetLocalTransform(id, transform);
}

bool Scene::SetLocalRotation(GameObjectID id, float rotationDegrees)
{
    auto transform = GetLocalTransform(id);
    transform.rotationDegrees = rotationDegrees;
    return SetLocalTransform(id, transform);
}

bool Scene::SetWorldPosition(GameObjectID id, const glm::vec2& position)
{
    auto transform = GetWorldTransform(id);
    transform.position = position;
    return SetWorldTransform(id, transform);
}

bool Scene::SetWorldScale(GameObjectID id, const glm::vec2& scale)
{
    auto transform = GetWorldTransform(id);
    transform.scale = scale;
    return SetWorldTransform(id, transform);
}

bool Scene::SetWorldPivot(GameObjectID id, const glm::vec2& pivot)
{
    auto transform = GetWorldTransform(id);
    transform.pivot = pivot;
    return SetWorldTransform(id, transform);
}

bool Scene::SetWorldRotation(GameObjectID id, float rotationDegrees)
{
    auto transform = GetWorldTransform(id);
    transform.rotationDegrees = rotationDegrees;
    return SetWorldTransform(id, transform);
}

std::string Scene::GetSpriteTexturePath(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null ? m_Registry.get<SpriteComponent>(entity).GetTexture().path : std::string{};
}

IntRect Scene::GetSpriteSourceRect(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null ? m_Registry.get<SpriteComponent>(entity).GetSourceRect() : IntRect{};
}

bool Scene::SpriteUsesSourceRect(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null && m_Registry.get<SpriteComponent>(entity).UsesSourceRect();
}

glm::vec2 Scene::GetSpriteSize(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null ? m_Registry.get<SpriteComponent>(entity).GetSize() : glm::vec2{};
}

glm::vec4 Scene::GetSpriteTint(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null ? m_Registry.get<SpriteComponent>(entity).GetTint() : glm::vec4{};
}

int Scene::GetSpriteLayer(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null ? m_Registry.get<SpriteComponent>(entity).GetLayer() : 0;
}

bool Scene::IsSpriteVisible(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null && m_Registry.get<SpriteComponent>(entity).IsVisible();
}

bool Scene::IsSpriteFlippedX(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null && m_Registry.get<SpriteComponent>(entity).IsFlippedX();
}

bool Scene::IsSpriteFlippedY(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null && m_Registry.get<SpriteComponent>(entity).IsFlippedY();
}

bool Scene::SetSpriteTexturePath(GameObjectID id, const std::string& path)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetTexturePath(path);
    return true;
}

bool Scene::SetSpriteSourceRect(GameObjectID id, int x, int y, int width, int height)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetSourceRect(x, y, width, height);
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetSpriteSourceRectFromGrid(GameObjectID id, int column, int row, int cellWidth, int cellHeight)
{
    return SetSpriteSourceRect(id, column * cellWidth, row * cellHeight, cellWidth, cellHeight);
}

bool Scene::ClearSpriteSourceRect(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).ClearSourceRect();
    return true;
}

bool Scene::SetSpriteSize(GameObjectID id, const glm::vec2& size)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetSize(size);
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetSpriteTint(GameObjectID id, const glm::vec4& tint)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetTint(tint);
    return true;
}

bool Scene::SetSpriteLayer(GameObjectID id, int layer)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetLayer(layer);
    return true;
}

bool Scene::SetSpriteVisible(GameObjectID id, bool visible)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetVisible(visible);
    return true;
}

bool Scene::SetSpriteFlipX(GameObjectID id, bool flip)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetFlipX(flip);
    return true;
}

bool Scene::SetSpriteFlipY(GameObjectID id, bool flip)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<SpriteComponent>(entity).SetFlipY(flip);
    return true;
}

bool Scene::HasAnimation(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    return entity != entt::null && m_Registry.all_of<SpriteAnimationComponent>(entity);
}

bool Scene::EnsureAnimation(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    if (!m_Registry.all_of<SpriteAnimationComponent>(entity))
        m_Registry.emplace<SpriteAnimationComponent>(entity);

    return true;
}

bool Scene::RemoveAnimation(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.remove<SpriteAnimationComponent>(entity);
    return true;
}

std::string Scene::GetAnimationSetPath(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return {};

    return m_Registry.get<SpriteAnimationComponent>(entity).GetAnimationSetRef().path;
}

std::string Scene::GetAnimationClipName(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return {};

    return m_Registry.get<SpriteAnimationComponent>(entity).GetRequestedClipName();
}

bool Scene::SetAnimationSetPath(GameObjectID id, const std::string& path)
{
    if (!EnsureAnimation(id))
        return false;

    m_Registry.get<SpriteAnimationComponent>(FindEntityByID(id)).SetAnimationSetPath(path);
    return true;
}

bool Scene::PlayAnimation(GameObjectID id, const std::string& clipName, bool restartIfSame)
{
    if (!EnsureAnimation(id))
        return false;

    m_Registry.get<SpriteAnimationComponent>(FindEntityByID(id)).Play(clipName, restartIfSame);
    return true;
}

bool Scene::StopAnimation(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return false;

    m_Registry.get<SpriteAnimationComponent>(entity).Stop();
    return true;
}

bool Scene::ResetAnimation(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return false;

    m_Registry.get<SpriteAnimationComponent>(entity).Reset();
    return true;
}

bool Scene::IsAnimationPlaying(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return false;

    return m_Registry.get<SpriteAnimationComponent>(entity).IsPlaying();
}

bool Scene::HasAnimationFinished(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return false;

    return m_Registry.get<SpriteAnimationComponent>(entity).HasFinished();
}

bool Scene::IsPlayingAnimationClip(GameObjectID id, const std::string& clipName) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null || !m_Registry.all_of<SpriteAnimationComponent>(entity))
        return false;

    return m_Registry.get<SpriteAnimationComponent>(entity).IsPlayingClip(clipName);
}

const SpriteAnimationSet* Scene::GetOrLoadAnimationSet(const std::string& path)
{
    if (path.empty())
        return nullptr;

    auto it = m_AnimationSetCache.find(path);
    if (it != m_AnimationSetCache.end())
        return &it->second;

    SpriteAnimationSet animationSet;
    if (!animationSet.LoadFromCSV(path))
    {
        std::cerr << "Failed to load animation set '" << path << "'\n";
        return nullptr;
    }

    auto [insertedIt, inserted] = m_AnimationSetCache.emplace(path, std::move(animationSet));
    return &insertedIt->second;
}

void Scene::MarkTransformDirty(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return;

    m_Registry.get<WorldTransformComponent>(entity).dirty = true;

    auto view = m_Registry.view<IDComponent, RelationshipComponent>();
    for (entt::entity candidate : view)
    {
        const auto& candidateId = view.get<IDComponent>(candidate);
        const auto& relationship = view.get<RelationshipComponent>(candidate);

        if (relationship.parentID == id)
            MarkTransformDirty(candidateId.id);
    }
}

Transform2D Scene::ResolveWorldTransform(entt::entity entity)
{
    auto& local = m_Registry.get<LocalTransformComponent>(entity);
    auto& world = m_Registry.get<WorldTransformComponent>(entity);
    const auto& relationship = m_Registry.get<RelationshipComponent>(entity);
    const auto& sprite = m_Registry.get<SpriteComponent>(entity);

    if (!world.dirty)
        return world.world;

    if (relationship.parentID == 0)
    {
        world.world = local;
        world.dirty = false;
        return world.world;
    }

    const entt::entity parentEntity = FindEntityByID(relationship.parentID);
    if (parentEntity == entt::null)
    {
        world.world = local;
        world.dirty = false;
        return world.world;
    }

    const Transform2D parentWorld = ResolveWorldTransform(parentEntity);
    const auto& parentSprite = m_Registry.get<SpriteComponent>(parentEntity);

    world.world = TransformMath2D::CombineTransforms(
        parentWorld,
        parentSprite.GetSize(),
        local,
        sprite.GetSize()
    );

    world.dirty = false;
    return world.world;
}

std::vector<entt::entity> Scene::SortForRendering() const
{
    std::vector<entt::entity> renderQueue;

    auto view = m_Registry.view<ActiveComponent, SpriteComponent>();
    for (entt::entity entity : view)
    {
        const auto& active = view.get<ActiveComponent>(entity);
        const auto& sprite = view.get<SpriteComponent>(entity);

        if (!active.active || !sprite.IsVisible())
            continue;

        renderQueue.push_back(entity);
    }

    std::stable_sort(
        renderQueue.begin(),
        renderQueue.end(),
        [this](entt::entity a, entt::entity b)
        {
            return m_Registry.get<SpriteComponent>(a).GetLayer() <
                   m_Registry.get<SpriteComponent>(b).GetLayer();
        }
    );

    return renderQueue;
}

void Scene::DestroyPendingGameObjects()
{
    std::vector<GameObjectID> idsToDestroy;

    auto view = m_Registry.view<IDComponent, ActiveComponent>();
    for (entt::entity entity : view)
    {
        const auto& id = view.get<IDComponent>(entity);
        const auto& active = view.get<ActiveComponent>(entity);

        if (active.pendingDestroy)
            idsToDestroy.push_back(id.id);
    }

    for (GameObjectID id : idsToDestroy)
    {
        const entt::entity entity = FindEntityByID(id);
        if (entity == entt::null)
            continue;

        m_EntityByID.erase(id);
        m_Registry.destroy(entity);
    }
}

bool Scene::WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const
{
    if (parentID == 0)
        return false;
    if (childID == parentID)
        return true;

    const entt::entity parentEntity = FindEntityByID(parentID);
    if (parentEntity == entt::null)
        return false;

    return WouldCreateCycle(childID, m_Registry.get<RelationshipComponent>(parentEntity).parentID);
}
