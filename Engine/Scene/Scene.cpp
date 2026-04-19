#include "Scene.h"

#include <algorithm>
#include <iostream>

#include "Math/TransformMath2D.h"
#include "Object/ObjectRegistry.h"
#include "Renderer/IRenderer2D.h"
#include "SceneSerializer.h"
#include "SpriteAnimation.h"
#include "Component/SceneComponentRegistry.h"

namespace
{
    enum class OverlapEventPhase
    {
        Enter,
        Stay,
        Exit
    };

    bool OverlapEquals(const OverlapResult& a, const OverlapResult& b)
    {
        return
            a.type == b.type &&
            a.objectID == b.objectID &&
            a.tileLayerIndex == b.tileLayerIndex &&
            a.tileCoordinates == b.tileCoordinates;
    }

    bool ContainsOverlap(const std::vector<OverlapResult>& overlaps, const OverlapResult& target)
    {
        return std::any_of(
            overlaps.begin(),
            overlaps.end(),
            [&](const OverlapResult& overlap) { return OverlapEquals(overlap, target); }
        );
    }
}

Scene::Scene()
{
    ConnectRegistrySignals();
}

void Scene::BeginPlay()
{
    if (m_IsPlaying)
        return;

    m_IsPlaying = true;
    m_PreviousCollisionOverlaps.clear();
    m_PreviousTriggerOverlaps.clear();

    auto view = m_Registry.view<IDComponent, ObjectTypeComponent>();
    for (entt::entity entity : view)
    {
        const auto& idComponent = view.get<IDComponent>(entity);
        const auto& objectType = view.get<ObjectTypeComponent>(entity);

        if (objectType.typeName.empty())
            continue;

        const ObjectLifecycleHooks* hooks = ObjectRegistry::Get().Find(objectType.typeName);
        if (hooks == nullptr || !hooks->beginPlay)
            continue;

        hooks->beginPlay(Entity(this, &m_Registry, entity, idComponent.id));
    }
}

void Scene::EndPlay()
{
    if (!m_IsPlaying)
        return;

    auto view = m_Registry.view<IDComponent, ObjectTypeComponent>();
    for (entt::entity entity : view)
    {
        const auto& idComponent = view.get<IDComponent>(entity);
        const auto& objectType = view.get<ObjectTypeComponent>(entity);

        if (objectType.typeName.empty())
            continue;

        const ObjectLifecycleHooks* hooks = ObjectRegistry::Get().Find(objectType.typeName);
        if (hooks == nullptr || !hooks->endPlay)
            continue;

        hooks->endPlay(Entity(this, &m_Registry, entity, idComponent.id));
    }

    m_IsPlaying = false;
    m_PreviousCollisionOverlaps.clear();
    m_PreviousTriggerOverlaps.clear();
}

void Scene::ConnectRegistrySignals()
{
    m_Registry.on_update<LocalTransformComponent>().connect<&Scene::OnLocalTransformUpdated>(this);
}

void Scene::OnLocalTransformUpdated(entt::registry& registry, entt::entity entity)
{
    const IDComponent* idComponent = registry.try_get<IDComponent>(entity);
    if (idComponent == nullptr)
        return;

    MarkTransformDirty(idComponent->id);
}

void Scene::ResolveRequiredComponents(entt::entity entity)
{
    if (entity == entt::null || !m_Registry.valid(entity))
        return;

    const auto& idComponent = m_Registry.get<IDComponent>(entity);
    const auto componentIDs = m_Registry.get<RequiredComponentsComponent>(entity).componentIDs;

    Entity wrapped(this, &m_Registry, entity, idComponent.id);

    for (ComponentTypeID componentID : componentIDs)
    {
        const SceneComponent* component = SceneComponentRegistry::Get().Find(componentID);
        if (component == nullptr)
            continue;

        component->Ensure(wrapped);
    }
}

entt::entity Scene::FindEntityByID(GameObjectID id) const
{
    auto it = m_EntityByID.find(id);
    return it != m_EntityByID.end() ? it->second : entt::null;
}

Entity Scene::CreateEntity(const std::string& name, GameObjectID parentID)
{
    const entt::entity entity = CreateEntityInternal(name);
    const GameObjectID id = m_Registry.get<IDComponent>(entity).id;

    if (parentID != 0)
        SetParent(id, parentID);

    return Entity(this, &m_Registry, entity, id);
}

bool Scene::SaveToFile(const std::string& path) const
{
    return SceneSerializer::SaveToFile(*this, path);
}

bool Scene::LoadFromFile(const std::string& path)
{
    return SceneSerializer::LoadFromFile(*this, path);
}

Entity Scene::GetEntity(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return {};

    return Entity(this, &m_Registry, entity, id);
}

Entity Scene::GetEntity(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return {};

    return Entity(const_cast<Scene*>(this), const_cast<entt::registry*>(&m_Registry), entity, id);
}

entt::entity Scene::CreateEntityInternal(const std::string& name)
{
    const entt::entity entity = m_Registry.create();
    const GameObjectID id = m_NextGameObjectID++;

    m_Registry.emplace<IDComponent>(entity, id);
    m_Registry.emplace<NameComponent>(entity, name);
    m_Registry.emplace<ActiveComponent>(entity);
    m_Registry.emplace<RelationshipComponent>(entity,RelationshipComponent{GameObjectID{0},static_cast<int>(m_EntityByID.size()),ChildDestroyPolicy::DetachToRoot});
    m_Registry.emplace<LocalTransformComponent>(entity);
    m_Registry.emplace<WorldTransformComponent>(entity);
    m_Registry.emplace<RequiredComponentsComponent>(entity);
    m_Registry.emplace<ObjectTypeComponent>(entity);
    m_Registry.emplace<SpriteComponent>(entity);

    m_EntityByID.emplace(id, entity);
    return entity;
}

entt::entity Scene::CreateEntityWithID(const std::string& name, GameObjectID id)
{
    const entt::entity entity = m_Registry.create();

    m_Registry.emplace<IDComponent>(entity, id);
    m_Registry.emplace<NameComponent>(entity, name);
    m_Registry.emplace<ActiveComponent>(entity);
    m_Registry.emplace<RelationshipComponent>(
        entity,
        RelationshipComponent{
            GameObjectID{0},
            static_cast<int>(m_EntityByID.size()),
            ChildDestroyPolicy::DetachToRoot
        }
    );
    m_Registry.emplace<LocalTransformComponent>(entity);
    m_Registry.emplace<WorldTransformComponent>(entity);
    m_Registry.emplace<RequiredComponentsComponent>(entity);
    m_Registry.emplace<ObjectTypeComponent>(entity);
    m_Registry.emplace<SpriteComponent>(entity);

    m_EntityByID.emplace(id, entity);
    m_NextGameObjectID = std::max(m_NextGameObjectID, id + 1);
    return entity;
}

void Scene::Clear()
{
    EndPlay();
    m_Registry.clear();
    m_EntityByID.clear();
    m_AnimationSetCache.clear();
    m_TileSetAssetCache.clear();
    m_NextGameObjectID = 1;
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
    DestroyGameObjectRecursive(id);
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

    if (m_IsPlaying)
        UpdateCollisionEvents();

    DestroyPendingGameObjects();
}

void Scene::Render(IRenderer2D& renderer)
{
    renderer.SetCamera(m_Camera);
    
    for (entt::entity entity : SortForRendering())
        RenderEntity(renderer, entity);
}

void Scene::RenderTileMapOnly(IRenderer2D& renderer, GameObjectID tileMapID)
{
    renderer.SetCamera(m_Camera);

    const entt::entity entity = FindEntityByID(tileMapID);
    if (entity == entt::null || !m_Registry.valid(entity))
        return;

    if (!m_Registry.all_of<ActiveComponent, TileMapComponent>(entity))
        return;

    if (!m_Registry.get<ActiveComponent>(entity).active)
        return;

    RenderEntity(renderer, entity);
}

void Scene::RenderEntity(IRenderer2D& renderer, entt::entity entity)
{
    const auto& id = m_Registry.get<IDComponent>(entity);

    if (m_Registry.all_of<TileMapComponent>(entity))
    {
        const auto& tileMap = m_Registry.get<TileMapComponent>(entity);
        const TileSetAsset* tileSetAsset = GetOrLoadTileSetAsset(tileMap.tileSetAssetPath);
        const std::string texturePath =
            (tileSetAsset != nullptr && !tileSetAsset->GetTexturePath().empty())
            ? tileSetAsset->GetTexturePath()
            : tileMap.tilesetTexturePath;
        const uint32_t atlasColumns = tileSetAsset != nullptr ? tileSetAsset->GetColumns() : tileMap.columns;

        if (tileMap.width == 0 || tileMap.height == 0 || texturePath.empty() || tileMap.layers.empty())
            return;

        const int cellWidth = std::max(1, tileMap.atlasCellSize.x);
        const int cellHeight = std::max(1, tileMap.atlasCellSize.y);
        const int maxTileCount = static_cast<int>(atlasColumns * (tileSetAsset != nullptr ? tileSetAsset->GetRows() : tileMap.rows));
        const Transform2D mapTransform = GetWorldTransform(id.id);

        for (const auto& layer : tileMap.layers)
        {
            if (!layer.visible)
                continue;

            for (uint32_t y = 0; y < tileMap.height; ++y)
            {
                for (uint32_t x = 0; x < tileMap.width; ++x)
                {
                    const int32_t tileID = layer.tiles[y * tileMap.width + x];
                    if (tileID < 0 || tileID >= maxTileCount)
                        continue;

                    SpriteRenderer tileSprite;
                    tileSprite.SetTexturePath(texturePath);
                    tileSprite.SetSourceRectFromGrid(
                        tileID % static_cast<int>(atlasColumns),
                        tileID / static_cast<int>(atlasColumns),
                        cellWidth,
                        cellHeight
                    );
                    tileSprite.SetSize(tileMap.tileSize);

                    Transform2D tileTransform = mapTransform;
                    tileTransform.position += glm::vec2{
                        static_cast<float>(x) * tileMap.tileSize.x,
                        static_cast<float>(y) * tileMap.tileSize.y
                    };

                    renderer.DrawSprite(tileTransform, tileSprite);
                }
            }
        }

        return;
    }

    const auto& sprite = m_Registry.get<SpriteComponent>(entity);
    renderer.DrawSprite(GetWorldTransform(id.id), sprite);
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

bool Scene::OverlapsSolidBox(const glm::vec2& center, const glm::vec2& size, GameObjectID ignoredID) const
{
    return !QuerySolidOverlaps(center, size, ignoredID).empty();
}

bool Scene::OverlapsSolidBox(const AABB2D& box, GameObjectID ignoredID) const
{
    return !QuerySolidOverlaps(box, ignoredID).empty();
}

std::vector<OverlapResult> Scene::QuerySolidOverlaps(const glm::vec2& center, const glm::vec2& size, GameObjectID ignoredID) const
{
    const glm::vec2 halfSize = size * 0.5f;
    return QuerySolidOverlaps({center - halfSize, center + halfSize}, ignoredID);
}

std::vector<OverlapResult> Scene::QuerySolidOverlaps(const AABB2D& box, GameObjectID ignoredID) const
{
    std::vector<OverlapResult> results;

    auto entityView = m_Registry.view<IDComponent, ActiveComponent, BoxColliderComponent, Transform2D>();
    for (entt::entity entity : entityView)
    {
        const auto& id = entityView.get<IDComponent>(entity);
        const auto& active = entityView.get<ActiveComponent>(entity);   
        const auto& collider = entityView.get<BoxColliderComponent>(entity);
        const auto& colliderTransform = entityView.get<Transform2D>(entity);
        
        if (!active.active || !collider.enabled || collider.isTrigger || !collider.blocksMovement)
            continue;

        if (id.id == ignoredID)
            continue;

        if (collider.type != ColliderBodyType::Static)
            continue;
        
        const AABB2D colliderBounds = BuildColliderAABB(id.id);
        if (Intersects(box, colliderBounds))
        {
            results.push_back({
                OverlapResult::Type::Entity,
                id.id,
                0,
                {-1, -1},
                {colliderTransform.position}
            });
        }
    }
    
    auto tileMapView = m_Registry.view<IDComponent, ActiveComponent, TileMapComponent, Transform2D>();
    for (entt::entity entity : tileMapView)
    {
        const auto& id = tileMapView.get<IDComponent>(entity);
        const auto& active = tileMapView.get<ActiveComponent>(entity);
        const auto& tileMap = tileMapView.get<TileMapComponent>(entity);
        const auto& tileMapTransform = tileMapView.get<Transform2D>(entity);
        const TileSetAsset* tileSetAsset = GetOrLoadTileSetAsset(tileMap.tileSetAssetPath);

        if (!active.active || id.id == ignoredID)
            continue;

        for (uint32_t layerIndex = 0; layerIndex < tileMap.layers.size(); ++layerIndex)
        {
            const auto& layer = tileMap.layers[layerIndex];
            if (!layer.collisionEnabled || !layer.blocksMovement)
                continue;

            for (uint32_t y = 0; y < tileMap.height; ++y)
            {
                for (uint32_t x = 0; x < tileMap.width; ++x)
                {
                    const int32_t tileID = layer.tiles[y * tileMap.width + x];
                    if (tileID < 0)
                        continue;

                    if (tileSetAsset != nullptr &&
                        !tileSetAsset->IsTileSolid(static_cast<uint32_t>(tileID)))
                    {
                        continue;
                    }

                    const AABB2D tileBounds = BuildTileAABB(
                        id.id,
                        static_cast<int>(x),
                        static_cast<int>(y)
                    );

                    if (Intersects(box, tileBounds))
                    {
                        results.push_back({
                            OverlapResult::Type::Tile,
                            id.id,
                            layerIndex,
                            {static_cast<int>(x), static_cast<int>(y)}
                        });
                    }
                }
            }
        }
    }

    return results;
}

std::vector<OverlapResult> Scene::QueryCollisionOverlaps(GameObjectID id) const
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid() || !entity.HasBoxCollider() || !entity.IsBoxColliderEnabled() || entity.IsColliderTrigger())
        return {};

    const AABB2D box = BuildColliderAABB(id);
    std::vector<OverlapResult> results;

    auto entityView = m_Registry.view<IDComponent, ActiveComponent, BoxColliderComponent>();
    for (entt::entity other : entityView)
    {
        const auto& otherID = entityView.get<IDComponent>(other);
        const auto& otherActive = entityView.get<ActiveComponent>(other);
        const auto& otherCollider = entityView.get<BoxColliderComponent>(other);

        if (otherID.id == id)
            continue;

        if (!otherActive.active || !otherCollider.enabled || otherCollider.isTrigger)
            continue;

        if (Intersects(box, BuildColliderAABB(otherID.id)))
        {
            results.push_back({
                OverlapResult::Type::Entity,
                otherID.id,
                0,
                {-1, -1}
            });
        }
    }

    auto tileMapView = m_Registry.view<IDComponent, ActiveComponent, TileMapComponent>();
    for (entt::entity tileMapEntity : tileMapView)
    {
        const auto& tileMapID = tileMapView.get<IDComponent>(tileMapEntity);
        const auto& tileMapActive = tileMapView.get<ActiveComponent>(tileMapEntity);
        const auto& tileMap = tileMapView.get<TileMapComponent>(tileMapEntity);
        const TileSetAsset* tileSetAsset = GetOrLoadTileSetAsset(tileMap.tileSetAssetPath);

        if (!tileMapActive.active || tileMapID.id == id)
            continue;

        for (uint32_t layerIndex = 0; layerIndex < tileMap.layers.size(); ++layerIndex)
        {
            const auto& layer = tileMap.layers[layerIndex];
            if (!layer.collisionEnabled)
                continue;

            for (uint32_t y = 0; y < tileMap.height; ++y)
            {
                for (uint32_t x = 0; x < tileMap.width; ++x)
                {
                    const int32_t tileID = layer.tiles[y * tileMap.width + x];
                    if (tileID < 0)
                        continue;

                    if (tileSetAsset != nullptr &&
                        !tileSetAsset->IsTileSolid(static_cast<uint32_t>(tileID)))
                    {
                        continue;
                    }

                    if (Intersects(box, BuildTileAABB(tileMapID.id, static_cast<int>(x), static_cast<int>(y))))
                    {
                        results.push_back({
                            OverlapResult::Type::Tile,
                            tileMapID.id,
                            layerIndex,
                            {static_cast<int>(x), static_cast<int>(y)}
                        });
                    }
                }
            }
        }
    }

    return results;
}

std::vector<OverlapResult> Scene::QueryTriggerOverlaps(GameObjectID id) const
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid() || !entity.HasBoxCollider() || !entity.IsBoxColliderEnabled())
        return {};

    const AABB2D box = BuildColliderAABB(id);
    const bool selfIsTrigger = entity.IsColliderTrigger();
    std::vector<OverlapResult> results;

    auto entityView = m_Registry.view<IDComponent, ActiveComponent, BoxColliderComponent>();
    for (entt::entity other : entityView)
    {
        const auto& otherID = entityView.get<IDComponent>(other);
        const auto& otherActive = entityView.get<ActiveComponent>(other);
        const auto& otherCollider = entityView.get<BoxColliderComponent>(other);

        if (otherID.id == id)
            continue;

        if (!otherActive.active || !otherCollider.enabled)
            continue;

        if (!selfIsTrigger && !otherCollider.isTrigger)
            continue;

        if (Intersects(box, BuildColliderAABB(otherID.id)))
        {
            results.push_back({
                OverlapResult::Type::Entity,
                otherID.id,
                0,
                {-1, -1}
            });
        }
    }

    return results;
}

std::vector<OverlapResult> Scene::QueryBlockingHits(GameObjectID id, const glm::vec2& delta) const
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid() || !entity.HasBoxCollider() || !entity.IsBoxColliderEnabled())
        return {};

    const glm::vec2 currentPosition = entity.GetPosition();
    glm::vec2 resolved = currentPosition;
    std::vector<OverlapResult> results;

    if (delta.x != 0.0f)
    {
        const glm::vec2 proposed = resolved + glm::vec2{delta.x, 0.0f};
        const std::vector<OverlapResult> overlaps = QuerySolidOverlaps(BuildColliderAABB(id, proposed), id);
        for (const OverlapResult& overlap : overlaps)
        {
            if (!ContainsOverlap(results, overlap))
                results.push_back(overlap);
        }

        if (overlaps.empty())
            resolved.x = proposed.x;
    }

    if (delta.y != 0.0f)
    {
        const glm::vec2 proposed = resolved + glm::vec2{0.0f, delta.y};
        const std::vector<OverlapResult> overlaps = QuerySolidOverlaps(BuildColliderAABB(id, proposed), id);
        for (const OverlapResult& overlap : overlaps)
        {
            if (!ContainsOverlap(results, overlap))
                results.push_back(overlap);
        }
    }

    return results;
}

AABB2D Scene::BuildWorldAABB(GameObjectID id) const
{
    const Entity entity = GetEntity(id);
    
    if (!entity.IsValid() || !entity.HasBoxCollider())
    {
        return {};
    }
    
    const Transform2D transform = GetWorldTransform(id);
    const glm::vec2 colliderSize = entity.GetBoxColliderSize();
    const glm::vec2 colliderOffset = entity.GetBoxColliderOffset();
    const glm::vec2 center = transform.position + colliderOffset;
    const glm::vec2 halfSize = colliderSize * 0.5f;
    
    return {
        center - halfSize,
        center + halfSize
    };
}

AABB2D Scene::BuildColliderAABB(GameObjectID id) const
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid() || !entity.HasBoxCollider())
        return {};

    return BuildColliderAABB(id, entity.GetPosition());
}

AABB2D Scene::BuildColliderAABB(GameObjectID id, const glm::vec2& overridePosition) const
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid() || !entity.HasBoxCollider())
        return {};

    const Transform2D transform = GetWorldTransform(id);
    const glm::vec2 colliderSize = entity.GetBoxColliderSize();
    const glm::vec2 colliderOffset = entity.GetBoxColliderOffset();
    const glm::vec2 spriteSize = entity.GetSpriteSize() * transform.scale;

    const glm::vec2 actorTopLeft =
        overridePosition - glm::vec2{
            spriteSize.x * transform.pivot.x,
            spriteSize.y * transform.pivot.y
        };

    const glm::vec2 colliderMin = actorTopLeft + colliderOffset;

    return {
        colliderMin,
        colliderMin + colliderSize
    };
}

glm::vec2 Scene::GetTileWorldPosition(GameObjectID tileMapID, int tileX, int tileY) const
{
    const Entity tileMap = GetEntity(tileMapID);
    if (!tileMap.IsValid() || !tileMap.HasTileMap())
        return {};

    const Transform2D transform = GetWorldTransform(tileMapID);
    const glm::vec2 tileSize = tileMap.GetTileSize();
    return transform.position + glm::vec2{
        static_cast<float>(tileX) * tileSize.x,
        static_cast<float>(tileY) * tileSize.y
    };
}

AABB2D Scene::BuildTileAABB(GameObjectID tileMapID, int tileX, int tileY) const
{
    const Entity tileMap = GetEntity(tileMapID);
    if (!tileMap.IsValid() || !tileMap.HasTileMap())
        return {};

    const glm::vec2 tileSize = tileMap.GetTileSize();
    const glm::vec2 min = GetTileWorldPosition(tileMapID, tileX, tileY);

    return {
        min,
        min + tileSize
    };
}

bool Scene::Intersects(const AABB2D& a, const AABB2D& b) const
{
    return
        a.min.x < b.max.x &&
        a.max.x > b.min.x &&
        a.min.y < b.max.y &&
        a.max.y > b.min.y;
}

glm::vec2 Scene::ResolveMovement(GameObjectID id, const glm::vec2& currentPosition, const glm::vec2& delta) const
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid() || !entity.HasBoxCollider())
        return currentPosition + delta;

    glm::vec2 resolved = currentPosition;

    if (delta.x != 0.0f)
    {
        const glm::vec2 proposed = resolved + glm::vec2{delta.x, 0.0f};
        const AABB2D bounds = BuildColliderAABB(id, proposed);
        const std::vector<OverlapResult> overlaps = QuerySolidOverlaps(bounds, id);
        if (overlaps.empty())
            resolved.x = proposed.x;
        else
        {
            for (const OverlapResult& overlap : overlaps)
                DispatchCollisionBlockedEvent(id, overlap);
        }
    }

    if (delta.y != 0.0f)
    {
        const glm::vec2 proposed = resolved + glm::vec2{0.0f, delta.y};
        const AABB2D bounds = BuildColliderAABB(id, proposed);
        const std::vector<OverlapResult> overlaps = QuerySolidOverlaps(bounds, id);
        if (overlaps.empty())
            resolved.y = proposed.y;
        else
        {
            for (const OverlapResult& overlap : overlaps)
                DispatchCollisionBlockedEvent(id, overlap);
        }
    }

    return resolved;
}

bool Scene::MoveWithCollision(GameObjectID id, const glm::vec2& delta)
{
    const Entity entity = GetEntity(id);
    if (!entity.IsValid())
        return false;

    const glm::vec2 currentPosition = entity.GetPosition();
    const glm::vec2 resolvedPosition = ResolveMovement(id, currentPosition, delta);

    if (resolvedPosition == currentPosition)
        return false;

    entity.SetPosition(resolvedPosition);
    return true;
}

void Scene::UpdateCollisionEvents()
{
    std::unordered_map<GameObjectID, std::vector<OverlapResult>> currentCollisionOverlaps;
    std::unordered_map<GameObjectID, std::vector<OverlapResult>> currentTriggerOverlaps;

    auto colliderView = m_Registry.view<IDComponent, ActiveComponent, BoxColliderComponent>();
    for (entt::entity entity : colliderView)
    {
        const auto& id = colliderView.get<IDComponent>(entity);
        const auto& active = colliderView.get<ActiveComponent>(entity);
        const auto& collider = colliderView.get<BoxColliderComponent>(entity);

        if (!active.active || !collider.enabled)
            continue;

        if (!collider.isTrigger)
            currentCollisionOverlaps[id.id] = QueryCollisionOverlaps(id.id);

        currentTriggerOverlaps[id.id] = QueryTriggerOverlaps(id.id);
    }

    for (const auto& [id, current] : currentCollisionOverlaps)
    {
        const std::vector<OverlapResult>& previous = m_PreviousCollisionOverlaps[id];

        for (const OverlapResult& overlap : current)
        {
            DispatchCollisionEvent(
                id,
                overlap,
                false,
                ContainsOverlap(previous, overlap) ? static_cast<int>(OverlapEventPhase::Stay) : static_cast<int>(OverlapEventPhase::Enter)
            );
        }

        for (const OverlapResult& overlap : previous)
        {
            if (!ContainsOverlap(current, overlap))
                DispatchCollisionEvent(id, overlap, false, static_cast<int>(OverlapEventPhase::Exit));
        }
    }

    for (const auto& [id, previous] : m_PreviousCollisionOverlaps)
    {
        if (currentCollisionOverlaps.contains(id))
            continue;

        for (const OverlapResult& overlap : previous)
            DispatchCollisionEvent(id, overlap, false, static_cast<int>(OverlapEventPhase::Exit));
    }

    for (const auto& [id, current] : currentTriggerOverlaps)
    {
        const std::vector<OverlapResult>& previous = m_PreviousTriggerOverlaps[id];

        for (const OverlapResult& overlap : current)
        {
            DispatchCollisionEvent(
                id,
                overlap,
                true,
                ContainsOverlap(previous, overlap) ? static_cast<int>(OverlapEventPhase::Stay) : static_cast<int>(OverlapEventPhase::Enter)
            );
        }

        for (const OverlapResult& overlap : previous)
        {
            if (!ContainsOverlap(current, overlap))
                DispatchCollisionEvent(id, overlap, true, static_cast<int>(OverlapEventPhase::Exit));
        }
    }

    for (const auto& [id, previous] : m_PreviousTriggerOverlaps)
    {
        if (currentTriggerOverlaps.contains(id))
            continue;

        for (const OverlapResult& overlap : previous)
            DispatchCollisionEvent(id, overlap, true, static_cast<int>(OverlapEventPhase::Exit));
    }

    m_PreviousCollisionOverlaps = std::move(currentCollisionOverlaps);
    m_PreviousTriggerOverlaps = std::move(currentTriggerOverlaps);
}

void Scene::DispatchCollisionEvent(GameObjectID id, const OverlapResult& overlap, bool isTrigger, int phase) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return;

    const auto& objectType = m_Registry.get<ObjectTypeComponent>(entity);
    if (objectType.typeName.empty())
        return;

    const ObjectLifecycleHooks* hooks = ObjectRegistry::Get().Find(objectType.typeName);
    if (hooks == nullptr)
        return;

    Entity wrapped(const_cast<Scene*>(this), const_cast<entt::registry*>(&m_Registry), entity, id);
    const OverlapEventPhase eventPhase = static_cast<OverlapEventPhase>(phase);

    if (isTrigger)
    {
        if (eventPhase == OverlapEventPhase::Enter && hooks->triggerEnter)
            hooks->triggerEnter(wrapped, overlap);
        else if (eventPhase == OverlapEventPhase::Stay && hooks->triggerStay)
            hooks->triggerStay(wrapped, overlap);
        else if (eventPhase == OverlapEventPhase::Exit && hooks->triggerExit)
            hooks->triggerExit(wrapped, overlap);
        return;
    }

    if (eventPhase == OverlapEventPhase::Enter && hooks->collisionEnter)
        hooks->collisionEnter(wrapped, overlap);
    else if (eventPhase == OverlapEventPhase::Stay && hooks->collisionStay)
        hooks->collisionStay(wrapped, overlap);
    else if (eventPhase == OverlapEventPhase::Exit && hooks->collisionExit)
        hooks->collisionExit(wrapped, overlap);
}

void Scene::DispatchCollisionBlockedEvent(GameObjectID id, const OverlapResult& overlap) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return;

    const auto& objectType = m_Registry.get<ObjectTypeComponent>(entity);
    if (objectType.typeName.empty())
        return;

    const ObjectLifecycleHooks* hooks = ObjectRegistry::Get().Find(objectType.typeName);
    if (hooks == nullptr || !hooks->collisionBlocked)
        return;

    Entity wrapped(const_cast<Scene*>(this), const_cast<entt::registry*>(&m_Registry), entity, id);
    hooks->collisionBlocked(wrapped, overlap);
}

void Scene::RenderCollisionDebug(IRenderer2D& renderer,
    GameObjectID focusedTileMapID,
    bool tileMapOnly)
{
    renderer.SetCamera(m_Camera);

    auto colliderView = m_Registry.view<IDComponent, ActiveComponent, BoxColliderComponent>();
    for (entt::entity entity : colliderView)
    {
        const auto& id = colliderView.get<IDComponent>(entity);
        const auto& active = colliderView.get<ActiveComponent>(entity);
        const auto& collider = colliderView.get<BoxColliderComponent>(entity);

        if (!active.active || !collider.enabled)
            continue;

        if (tileMapOnly && focusedTileMapID != 0 && id.id != focusedTileMapID)
            continue;

        const AABB2D bounds = BuildColliderAABB(id.id);

        glm::vec4 color{1.0f, 0.2f, 0.2f, 1.0f};
        if (collider.isTrigger)
            color = {1.0f, 0.9f, 0.2f, 1.0f};
        else if (collider.type == ColliderBodyType::Dynamic)
            color = {0.2f, 1.0f, 0.2f, 1.0f};

        renderer.DrawRectOutline(bounds.min, bounds.max, color, 2.0f);
    }

    auto tileMapView = m_Registry.view<IDComponent, ActiveComponent, TileMapComponent>();
    for (entt::entity entity : tileMapView)
    {
        const auto& id = tileMapView.get<IDComponent>(entity);
        const auto& active = tileMapView.get<ActiveComponent>(entity);
        const auto& tileMap = tileMapView.get<TileMapComponent>(entity);
        const TileSetAsset* tileSetAsset = GetOrLoadTileSetAsset(tileMap.tileSetAssetPath);
        
        if (!active.active)
            continue;

        if (tileMapOnly && focusedTileMapID != 0 && id.id != focusedTileMapID)
            continue;

        for (const auto& layer : tileMap.layers)
        {
            if (!layer.collisionEnabled)
                continue;

            for (uint32_t y = 0; y < tileMap.height; ++y)
            {
                for (uint32_t x = 0; x < tileMap.width; ++x)
                {
                    const int32_t tileID = layer.tiles[y * tileMap.width + x];
                    if (tileID < 0)
                        continue;
                    
                    if (tileSetAsset != nullptr &&
                      !tileSetAsset->IsTileSolid(static_cast<uint32_t>(tileID)))
                    {
                        continue;
                    }


                    const AABB2D bounds = BuildTileAABB(
                        id.id,
                        static_cast<int>(x),
                        static_cast<int>(y)
                    );

                    renderer.DrawRectOutline(
                        bounds.min,
                        bounds.max,
                        {0.0f, 0.0f, 1.0f, 1.0f},
                        1.0f
                    );
                }
            }
        }
    }
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
    const auto& childSprite = m_Registry.get<SpriteComponent>(childEntity);
    const auto& parentSprite = m_Registry.get<SpriteComponent>(parentEntity);

    relationship.parentID = parentID;
    m_Registry.replace<LocalTransformComponent>(
        childEntity,
        TransformMath2D::ToLocalTransform(
        childWorld,
        childSprite.GetSize(),
        parentWorld,
        parentSprite.GetSize()
        )
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
    m_Registry.replace<LocalTransformComponent>(childEntity, GetWorldTransform(childID));
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

    const auto& relationship = m_Registry.get<RelationshipComponent>(entity);

    if (relationship.parentID == 0)
    {
        m_Registry.replace<LocalTransformComponent>(entity, transform);
        return true;
    }

    const entt::entity parentEntity = FindEntityByID(relationship.parentID);
    if (parentEntity == entt::null)
    {
        m_Registry.replace<LocalTransformComponent>(entity, transform);
        return true;
    }

    const auto& sprite = m_Registry.get<SpriteComponent>(entity);
    const auto& parentSprite = m_Registry.get<SpriteComponent>(parentEntity);
    const Transform2D parentWorld = GetWorldTransform(relationship.parentID);

    m_Registry.replace<LocalTransformComponent>(
        entity,
        TransformMath2D::ToLocalTransform(
        transform,
        sprite.GetSize(),
        parentWorld,
        parentSprite.GetSize()
        )
    );
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

    m_Registry.replace<LocalTransformComponent>(entity, transform);
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

ChildDestroyPolicy Scene::GetChildDestroyPolicy(GameObjectID id) const
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return ChildDestroyPolicy::DetachToRoot;

    return m_Registry.get<RelationshipComponent>(entity).childDestroyPolicy;
}

bool Scene::SetChildDestroyPolicy(GameObjectID id, ChildDestroyPolicy policy)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return false;

    m_Registry.get<RelationshipComponent>(entity).childDestroyPolicy = policy;
    return true;
}

void Scene::HandleChildrenOnDestroy(GameObjectID parentID)
{
    const std::vector<GameObjectID> children = GetChildGameObjects(parentID);

    for (GameObjectID childID : children)
    {
        const entt::entity childEntity = FindEntityByID(childID);
        if (childEntity == entt::null)
            continue;

        const ChildDestroyPolicy policy =
            m_Registry.get<RelationshipComponent>(childEntity).childDestroyPolicy;

        if (policy == ChildDestroyPolicy::DestroyWithParent)
        {
            DestroyGameObjectRecursive(childID);
            continue;
        }

        ClearParent(childID);
    }
}

void Scene::DestroyGameObjectRecursive(GameObjectID id)
{
    const entt::entity entity = FindEntityByID(id);
    if (entity == entt::null)
        return;

    HandleChildrenOnDestroy(id);

    auto& active = m_Registry.get<ActiveComponent>(entity);
    active.pendingDestroy = true;
    active.active = false;
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

const TileSetAsset* Scene::GetOrLoadTileSetAsset(const std::string& path) const
{
    if (path.empty())
        return nullptr;

    auto it = m_TileSetAssetCache.find(path);
    if (it != m_TileSetAssetCache.end())
        return &it->second;

    TileSetAsset asset;
    if (!asset.LoadFromFile(path))
        return nullptr;

    auto [insertedIt, inserted] = m_TileSetAssetCache.emplace(path, std::move(asset));
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
        const bool hasTileMap = m_Registry.all_of<TileMapComponent>(entity);

        if (!active.active)
            continue;

        if (!hasTileMap && !sprite.IsVisible())
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
