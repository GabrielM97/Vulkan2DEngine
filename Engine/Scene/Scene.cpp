#include "Scene.h"

#include "Math/TransformMath2D.h"
#include "Renderer/IRenderer2D.h"
#include "Camera2D.h"

#include <algorithm>
#include <iostream>

GameObject& Scene::CreateGameObject(const std::string& name, GameObjectID parentID)
{
    auto object = std::make_unique<GameObject>();
    object->SetID(m_NextGameObjectID++);
    object->SetName(name);
    object->SetSiblingOrder(static_cast<int>(m_GameObjects.size()));
    
    GameObject& reference = *object;
    m_GameObjects.push_back(std::move(object));
    
    if (parentID != 0 && FindGameObjectByID(parentID) != nullptr)
        SetParent(reference.GetID(), parentID);
    
    return reference;
}

void Scene::DestroyGameObject(GameObject& object)
{
    object.Destroy();
}

void Scene::DestroyGameObject(GameObjectID id)
{
    if (GameObject* object = FindGameObjectByID(id))
        DestroyGameObject(*object);
}

size_t Scene::GetGameObjectCount() const
{
    return m_GameObjects.size();
}

GameObject* Scene::GetGameObject(size_t index)
{
    if (index >= m_GameObjects.size())
        return nullptr;

    return m_GameObjects[index].get();
}

const GameObject* Scene::GetGameObject(size_t index) const
{
    if (index >= m_GameObjects.size())
        return nullptr;

    return m_GameObjects[index].get();
}

GameObject* Scene::FindGameObjectByName(const std::string& name)
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetName() == name)
            return object.get();
    }

    return nullptr;
}

GameObject* Scene::FindGameObjectByID(GameObjectID id)
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetID() == id)
            return object.get();
    }

    return nullptr;
}

const GameObject* Scene::FindGameObjectByID(GameObjectID id) const
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetID() == id)
            return object.get();
    }

    return nullptr;
}

const GameObject* Scene::FindGameObjectByName(const std::string& name) const
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetName() == name)
            return object.get();
    }

    return nullptr;
}

void Scene::DestroyPendingGameObjects()
{
    std::erase_if(
      m_GameObjects,
      [](const std::unique_ptr<GameObject>& object)
      {
          return object->isPendingDestroy();
      }
  );
}

bool Scene::WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const
{
    if (parentID == 0)
        return false;

    if (parentID == childID)
        return true;

    const GameObject* parent = FindGameObjectByID(parentID);
    if (parent == nullptr)
        return false;

    return WouldCreateCycle(childID, parent->GetParentID());
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
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return;

    object->transformDirty = true;

    for (const auto& candidate : m_GameObjects)
    {
        if (candidate->GetParentID() == id)
            MarkTransformDirty(candidate->GetID());
    }
}

Transform2D Scene::ResolveWorldTransform(GameObject& object)
{
    if (!object.transformDirty)
        return object.cachedWorldTransform;

    if (!object.HasParent())
    {
        object.cachedWorldTransform = object.transform;
        object.transformDirty = false;
        return object.cachedWorldTransform;
    }

    GameObject* parent = FindGameObjectByID(object.GetParentID());
    if (parent == nullptr)
    {
        object.cachedWorldTransform = object.transform;
        object.transformDirty = false;
        return object.cachedWorldTransform;
    }

    const Transform2D parentWorld = ResolveWorldTransform(*parent);
    object.cachedWorldTransform = TransformMath2D::CombineTransforms(
        parentWorld,
        parent->sprite.GetSize(),
        object.transform,
        object.sprite.GetSize()
    );
    object.transformDirty = false;
    return object.cachedWorldTransform;
}

void Scene::Render(IRenderer2D& renderer)
{
    renderer.SetCamera(m_Camera);
    const std::vector<const GameObject*> renderQueue = SortForRendering();
    
    for (const GameObject* object : renderQueue)
    {
        if (!object->isActive() || !object->sprite.IsVisible())
            continue;

        Transform2D worldTransform = GetWorldTransform(object->GetID());
        
        renderer.DrawSprite(
            worldTransform,
            object->sprite
        );
    }
}

void Scene::Update(float deltaTime)
{
    for (const std::unique_ptr<GameObject>& object : m_GameObjects)
    {
        if (!object->isActive())
            continue;

        if (object->animation.has_value())
        {
            const SpriteAnimationSet* animationSet =
                GetOrLoadAnimationSet(object->animation->GetAnimationSetRef().path);

            object->animation->Update(deltaTime, object->sprite, animationSet);
        }
    }

    DestroyPendingGameObjects();
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

std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects()
{
    return m_GameObjects;
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
    return m_GameObjects;
}

std::vector<const GameObject*> Scene::GetRootGameObjects() const
{
    std::vector<const GameObject*> roots;
    roots.reserve(m_GameObjects.size());

    for (const auto& object : m_GameObjects)
    {
        if (!object->HasParent())
            roots.push_back(object.get());
    }
    
    std::stable_sort(
    roots.begin(),
    roots.end(),
    [](const GameObject* a, const GameObject* b)
    {
        return a->GetSiblingOrder() < b->GetSiblingOrder();
    });

    return roots;
}

std::vector<const GameObject*> Scene::GetChildGameObjects(GameObjectID parentID) const
{
    std::vector<const GameObject*> children;

    for (const auto& object : m_GameObjects)
    {
        if (object->GetParentID() == parentID)
            children.push_back(object.get());
    }
    
    std::stable_sort(
    children.begin(),
    children.end(),
    [](const GameObject* a, const GameObject* b)
    {
        return a->GetSiblingOrder() < b->GetSiblingOrder();
    });

    return children;
}

bool Scene::HasChildren(GameObjectID parentID) const
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetParentID() == parentID)
            return true;
    }

    return false;
}

bool Scene::SetParent(GameObjectID childID, GameObjectID parentID)
{
    if (childID == 0 || parentID == 0 || childID == parentID)
        return false;

    if (WouldCreateCycle(childID, parentID))
        return false;
    
    GameObject* child = FindGameObjectByID(childID);
    GameObject* parent = FindGameObjectByID(parentID);

    if (child == nullptr || parent == nullptr)
        return false;

    const Transform2D childWorld = GetWorldTransform(childID);
    const Transform2D parentWorld = GetWorldTransform(parentID);

    child->SetParentID(parentID);
    child->transform = TransformMath2D::ToLocalTransform(
        childWorld,
        child->sprite.GetSize(),
        parentWorld,
        parent->sprite.GetSize()
    );
    MarkTransformDirty(childID);
    return true;
}

bool Scene::ClearParent(GameObjectID childID)
{
    GameObject* child = FindGameObjectByID(childID);
    
    if (child == nullptr)
        return false;

    const Transform2D world = GetWorldTransform(childID);

    child->ClearParent();
    child->transform = world;
    MarkTransformDirty(childID);

    return true;
}

Transform2D Scene::GetWorldTransform(GameObjectID id) const
{
    GameObject* object = const_cast<Scene*>(this)->FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    return const_cast<Scene*>(this)->ResolveWorldTransform(*object);
}

bool Scene::SetWorldTransform(GameObjectID id, const Transform2D& transform)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;
    
    if (!object->HasParent())
    {
        object->transform = transform;
        MarkTransformDirty(id);
        return true;
    }
    
    const GameObject* parent = FindGameObjectByID(object->GetParentID());
    if (parent == nullptr)
    {
        object->transform = transform;
        MarkTransformDirty(id);
        return true;
    }

    const Transform2D parentWorld = GetWorldTransform(parent->GetID());

    object->transform = TransformMath2D::ToLocalTransform(
        transform,
        object->sprite.GetSize(),
        parentWorld,
        parent->sprite.GetSize()
    );
    MarkTransformDirty(id);

    return true;   
}

Transform2D Scene::GetLocalTransform(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    return object->transform;
}

glm::vec2 Scene::GetLocalPosition(GameObjectID id) const
{
    return GetLocalTransform(id).position;
}

glm::vec2 Scene::GetLocalScale(GameObjectID id) const
{
    return GetLocalTransform(id).scale;
}

glm::vec2 Scene::GetLocalPivot(GameObjectID id) const
{
    return GetLocalTransform(id).pivot;
}

float Scene::GetLocalRotation(GameObjectID id) const
{
    return GetLocalTransform(id).rotationDegrees;
}

glm::vec2 Scene::GetWorldPosition(GameObjectID id) const
{
    return GetWorldTransform(id).position;
}

glm::vec2 Scene::GetWorldScale(GameObjectID id) const
{
    return GetWorldTransform(id).scale;
}

glm::vec2 Scene::GetWorldPivot(GameObjectID id) const
{
    return GetWorldTransform(id).pivot;
}

float Scene::GetWorldRotation(GameObjectID id) const
{
    return GetWorldTransform(id).rotationDegrees;
}

bool Scene::SetLocalTransform(GameObjectID id, const Transform2D& transform)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->transform = transform;
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetLocalPosition(GameObjectID id, const glm::vec2& position)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->transform.position = position;
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetLocalScale(GameObjectID id, const glm::vec2& scale)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->transform.scale = scale;
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetLocalPivot(GameObjectID id, const glm::vec2& pivot)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->transform.pivot = pivot;
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetLocalRotation(GameObjectID id, float rotationDegrees)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->transform.rotationDegrees = rotationDegrees;
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetWorldPosition(GameObjectID id, const glm::vec2& position)
{
    Transform2D worldTransform = GetWorldTransform(id);
    worldTransform.position = position;
    return SetWorldTransform(id, worldTransform);
}

bool Scene::SetWorldScale(GameObjectID id, const glm::vec2& scale)
{
    Transform2D worldTransform = GetWorldTransform(id);
    worldTransform.scale = scale;
    return SetWorldTransform(id, worldTransform);
}

bool Scene::SetWorldPivot(GameObjectID id, const glm::vec2& pivot)
{
    Transform2D worldTransform = GetWorldTransform(id);
    worldTransform.pivot = pivot;
    return SetWorldTransform(id, worldTransform);
}

bool Scene::SetWorldRotation(GameObjectID id, float rotationDegrees)
{
    Transform2D worldTransform = GetWorldTransform(id);
    worldTransform.rotationDegrees = rotationDegrees;
    return SetWorldTransform(id, worldTransform);
}

bool Scene::SetSiblingOrder(GameObjectID id, int order)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->SetSiblingOrder(order);
    return true;
}

bool Scene::SetGameObjectName(GameObjectID id, const std::string& name)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->SetName(name);
    return true;
}

bool Scene::SetGameObjectActive(GameObjectID id, bool active)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->setActive(active);
    return true;
}

std::string Scene::GetSpriteTexturePath(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    return object->sprite.GetTexture().path;
}

IntRect Scene::GetSpriteSourceRect(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    return object->sprite.GetSourceRect();
}

bool Scene::SpriteUsesSourceRect(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    return object->sprite.UsesSourceRect();
}

glm::vec2 Scene::GetSpriteSize(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    return object->sprite.GetSize();
}

glm::vec4 Scene::GetSpriteTint(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    return object->sprite.GetTint();
}

int Scene::GetSpriteLayer(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return 0;

    return object->sprite.GetLayer();
}

bool Scene::IsSpriteVisible(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    return object->sprite.IsVisible();
}

bool Scene::IsSpriteFlippedX(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    return object->sprite.IsFlippedX();
}

bool Scene::IsSpriteFlippedY(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    return object->sprite.IsFlippedY();
}

bool Scene::SetSpriteTexturePath(GameObjectID id, const std::string& path)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetTexturePath(path);
    return true;
}

bool Scene::SetSpriteSourceRect(GameObjectID id, int x, int y, int width, int height)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetSourceRect(x, y, width, height);
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetSpriteSourceRectFromGrid(GameObjectID id, int column, int row, int cellWidth, int cellHeight)
{
    return SetSpriteSourceRect(
        id,
        column * cellWidth,
        row * cellHeight,
        cellWidth,
        cellHeight
    );
}

bool Scene::ClearSpriteSourceRect(GameObjectID id)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.ClearSourceRect();
    return true;
}

bool Scene::SetSpriteSize(GameObjectID id, const glm::vec2& size)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetSize(size);
    MarkTransformDirty(id);
    return true;
}

bool Scene::SetSpriteTint(GameObjectID id, const glm::vec4& tint)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetTint(tint);
    return true;
}

bool Scene::SetSpriteLayer(GameObjectID id, int layer)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetLayer(layer);
    return true;
}

bool Scene::SetSpriteVisible(GameObjectID id, bool visible)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetVisible(visible);
    return true;
}

bool Scene::SetSpriteFlipX(GameObjectID id, bool flip)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetFlipX(flip);
    return true;
}

bool Scene::SetSpriteFlipY(GameObjectID id, bool flip)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->sprite.SetFlipY(flip);
    return true;
}

bool Scene::HasAnimation(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    return object->animation.has_value();
}

bool Scene::EnsureAnimation(GameObjectID id)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    if (!object->animation.has_value())
        object->animation.emplace();

    return true;
}

bool Scene::RemoveAnimation(GameObjectID id)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    object->animation.reset();
    return true;
}

std::string Scene::GetAnimationSetPath(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return {};

    return object->animation->GetAnimationSetRef().path;
}

std::string Scene::GetAnimationClipName(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return {};

    return object->animation->GetRequestedClipName();
}

bool Scene::SetAnimationSetPath(GameObjectID id, const std::string& path)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    if (!object->animation.has_value())
        object->animation.emplace();

    object->animation->SetAnimationSetPath(path);
    return true;
}

bool Scene::PlayAnimation(GameObjectID id, const std::string& clipName, bool restartIfSame)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return false;

    if (!object->animation.has_value())
        object->animation.emplace();

    object->animation->Play(clipName, restartIfSame);
    return true;
}

bool Scene::StopAnimation(GameObjectID id)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return false;

    object->animation->Stop();
    return true;
}

bool Scene::ResetAnimation(GameObjectID id)
{
    GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return false;

    object->animation->Reset();
    return true;
}

bool Scene::IsAnimationPlaying(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return false;

    return object->animation->IsPlaying();
}

bool Scene::HasAnimationFinished(GameObjectID id) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return false;

    return object->animation->HasFinished();
}

bool Scene::IsPlayingAnimationClip(GameObjectID id, const std::string& clipName) const
{
    const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr || !object->animation.has_value())
        return false;

    return object->animation->IsPlayingClip(clipName);
}

GameObjectHandle Scene::GetGameObjectHandle(GameObjectID id)
{
    if (FindGameObjectByID(id) == nullptr)
        return {};

    return GameObjectHandle(this, id);
}

GameObjectHandle Scene::CreateGameObjectHandle(const std::string& name, GameObjectID parentID)
{
    GameObject& object = CreateGameObject(name, parentID);
    return GameObjectHandle(this, object.GetID());
}

std::vector<const GameObject*> Scene::SortForRendering()
{
    std::vector<const GameObject*> renderQueue;
    renderQueue.reserve(m_GameObjects.size());

    for (const auto& object : m_GameObjects)
    {
        if (!object->isActive() || !object->sprite.IsVisible())
            continue;

        renderQueue.push_back(object.get());
    }

    std::stable_sort(
        renderQueue.begin(),
        renderQueue.end(),
        [](const GameObject* a, const GameObject* b)
        {
            return a->sprite.GetLayer() < b->sprite.GetLayer();
        }
    );

    return renderQueue;
}
