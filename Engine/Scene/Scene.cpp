#include "Scene.h"

#include <algorithm>
#include <iostream>

#include "Math/TransformMath2D.h"
#include "Object/ObjectRegistry.h"
#include "Renderer/IRenderer2D.h"
#include "SceneSerializer.h"
#include "SpriteAnimation.h"
#include "Component/SceneComponentRegistry.h"

Scene::Scene()
{
    ConnectRegistrySignals();
}

void Scene::BeginPlay()
{
    if (m_IsPlaying)
        return;

    m_IsPlaying = true;

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
