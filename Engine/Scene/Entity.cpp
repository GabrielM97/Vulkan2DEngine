#include "Entity.h"

#include <algorithm>

#include "SpriteAnimation.h"
#include "Scene.h"

const std::vector<ComponentTypeID>& Entity::GetTrackedComponentIDs() const
{
    static const std::vector<ComponentTypeID> empty;

    if (!IsValid() || !m_Registry->all_of<RequiredComponentsComponent>(m_Entity))
        return empty;

    return m_Registry->get<RequiredComponentsComponent>(m_Entity).componentIDs;
}

void Entity::RegisterTrackedComponent(ComponentTypeID componentID) const
{
    if (!IsValid() || !m_Registry->all_of<RequiredComponentsComponent>(m_Entity))
        return;

    auto& componentIDs = m_Registry->get<RequiredComponentsComponent>(m_Entity).componentIDs;
    if (std::find(componentIDs.begin(), componentIDs.end(), componentID) == componentIDs.end())
        componentIDs.push_back(componentID);
}

void Entity::UnregisterTrackedComponent(ComponentTypeID componentID) const
{
    if (!IsValid() || !m_Registry->all_of<RequiredComponentsComponent>(m_Entity))
        return;

    auto& componentIDs = m_Registry->get<RequiredComponentsComponent>(m_Entity).componentIDs;
    componentIDs.erase(
        std::remove(componentIDs.begin(), componentIDs.end(), componentID),
        componentIDs.end()
    );
}

bool Entity::IsValid() const
{
    return m_Scene != nullptr && m_Scene->IsValidHandle(m_Entity, m_ID);
}

std::string Entity::GetName() const
{
    return IsValid() ? m_Scene->GetGameObjectName(m_ID) : std::string{};
}

void Entity::SetName(const std::string& name) const
{
    if (IsValid())
        m_Scene->SetGameObjectName(m_ID, name);
}

bool Entity::IsActive() const
{
    return IsValid() && m_Scene->IsGameObjectActive(m_ID);
}

void Entity::SetActive(bool active) const
{
    if (IsValid())
        m_Scene->SetGameObjectActive(m_ID, active);
}

void Entity::Destroy() const
{
    if (IsValid())
        m_Scene->DestroyGameObject(m_ID);
}

bool Entity::HasParent() const
{
    return IsValid() && m_Scene->HasParent(m_ID);
}

GameObjectID Entity::GetParentID() const
{
    return IsValid() ? m_Scene->GetParentID(m_ID) : 0;
}

void Entity::SetParent(GameObjectID parentID) const
{
    if (IsValid())
        m_Scene->SetParent(m_ID, parentID);
}

void Entity::ClearParent() const
{
    if (IsValid())
        m_Scene->ClearParent(m_ID);
}

ChildDestroyPolicy Entity::GetChildDestroyPolicy() const
{
    return IsValid() ? m_Scene->GetChildDestroyPolicy(m_ID) : ChildDestroyPolicy::DetachToRoot;
}

void Entity::SetChildDestroyPolicy(ChildDestroyPolicy policy) const
{
    if (IsValid())
        m_Scene->SetChildDestroyPolicy(m_ID, policy);
}

Transform2D Entity::GetTransform() const
{
    return GetWorldTransform();
}

void Entity::SetTransform(const Transform2D& transform) const
{
    SetWorldTransform(transform);   
}

glm::vec2 Entity::GetPosition() const
{
    return GetWorldTransform().position;  
}

void Entity::SetPosition(const glm::vec2& position) const
{
    Transform2D transform = GetWorldTransform();
    transform.position = position;
    SetWorldTransform(transform);
}

float Entity::GetRotation() const
{
    return GetWorldTransform().rotationDegrees;  
}

void Entity::SetRotation(float rotationDegrees) const
{
    Transform2D transform = GetWorldTransform();
    transform.rotationDegrees = rotationDegrees;
    SetWorldTransform(transform);
}

glm::vec2 Entity::GetScale() const
{
    return GetWorldTransform().scale; 
}

void Entity::SetScale(const glm::vec2& scale) const
{
    Transform2D transform = GetWorldTransform();
    transform.scale = scale;
    SetWorldTransform(transform);
}

glm::vec2 Entity::GetLocalPosition() const
{
    return IsValid() ? m_Scene->GetLocalPosition(m_ID) : glm::vec2{};
}

void Entity::SetLocalPosition(const glm::vec2& position) const
{
    if (IsValid())
        m_Scene->SetLocalPosition(m_ID, position);
}

float Entity::GetLocalRotation() const
{
    return IsValid() ? m_Scene->GetLocalRotation(m_ID) : 0;
}

void Entity::SetLocalRotation(float rotationDegrees) const
{
    if (IsValid())
        m_Scene->SetLocalRotation(m_ID, rotationDegrees);
}

glm::vec2 Entity::GetLocalScale() const
{
    return IsValid() ? m_Scene->GetLocalScale(m_ID) : glm::vec2{};
}

void Entity::SetLocalScale(const glm::vec2& scale) const
{
    if (IsValid())
        m_Scene->SetLocalScale(m_ID, scale);
}

Transform2D Entity::GetWorldTransform() const
{
    return IsValid() ? m_Scene->GetWorldTransform(m_ID) : Transform2D{};
}

LocalTransformComponent Entity::GetLocalTransform() const
{
    return IsValid() ? m_Scene->GetLocalTransform(m_ID) : LocalTransformComponent{};
}

void Entity::SetWorldTransform(const Transform2D& transform) const
{
    if (IsValid())
        m_Scene->SetWorldTransform(m_ID, transform);
}

void Entity::SetLocalTransform(const LocalTransformComponent& transform) const
{
    if (IsValid())
        m_Scene->SetLocalTransform(m_ID, transform);
}

std::string Entity::GetSpriteTexturePath() const
{
    return IsValid() ? GetComponent<SpriteComponent>().GetTexture().path : std::string{};
}

IntRect Entity::GetSpriteSourceRect() const
{
    return IsValid() ? GetComponent<SpriteComponent>().GetSourceRect() : IntRect{};
}

bool Entity::SpriteUsesSourceRect() const
{
    return IsValid() && GetComponent<SpriteComponent>().UsesSourceRect();
}

glm::vec2 Entity::GetSpriteSize() const
{
    return IsValid() ? GetComponent<SpriteComponent>().GetSize() : glm::vec2{};
}

glm::vec4 Entity::GetSpriteTint() const
{
    return IsValid() ? GetComponent<SpriteComponent>().GetTint() : glm::vec4{};
}

int Entity::GetSpriteLayer() const
{
    return IsValid() ? GetComponent<SpriteComponent>().GetLayer() : 0;
}

bool Entity::IsSpriteVisible() const
{
    return IsValid() && GetComponent<SpriteComponent>().IsVisible();
}

bool Entity::IsSpriteFlippedX() const
{
    return IsValid() && GetComponent<SpriteComponent>().IsFlippedX();
}

bool Entity::IsSpriteFlippedY() const
{
    return IsValid() && GetComponent<SpriteComponent>().IsFlippedY();
}

void Entity::SetSpriteTexturePath(const std::string& path)
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).SetTexturePath(path);
}

void Entity::SetSpriteSourceRect(int x, int y, int width, int height)
{
    if (IsValid())
    {
        m_Registry->get<SpriteComponent>(m_Entity).SetSourceRect(x, y, width, height);
        m_Scene->MarkTransformDirty(m_ID);
    }
}

void Entity::SetSpriteSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight)
{
    if (IsValid())
        SetSpriteSourceRect(column * cellWidth, row * cellHeight, cellWidth, cellHeight);
}

void Entity::SetSpriteSourceRectFromGrid(int column, int row)
{
    glm::vec2 cellSize = GetSpriteSize();
    
    if (IsValid())
        SetSpriteSourceRect(column * static_cast<int>(cellSize.x), row * static_cast<int>(cellSize.y), 
                        static_cast<int>(cellSize.x), static_cast<int>(cellSize.y));
}

void Entity::ClearSpriteSourceRect()
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).ClearSourceRect();
}

void Entity::SetSpriteSize(const glm::vec2& size)
{
    if (IsValid())
    {
        m_Registry->get<SpriteComponent>(m_Entity).SetSize(size);
        m_Scene->MarkTransformDirty(m_ID);
    }
}

void Entity::SetSpriteTint(const glm::vec4& tint)
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).SetTint(tint);
}

void Entity::SetSpriteLayer(int layer)
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).SetLayer(layer);
}

void Entity::SetSpriteVisible(bool visible)
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).SetVisible(visible);
}

void Entity::SetSpriteFlipX(bool flip)
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).SetFlipX(flip);
}

void Entity::SetSpriteFlipY(bool flip)
{
    if (IsValid())
        m_Registry->get<SpriteComponent>(m_Entity).SetFlipY(flip);
}

bool Entity::HasAnimation() const
{
    return IsValid() && HasComponent<SpriteAnimationComponent>();
}

void Entity::EnsureAnimation() const
{
    if (IsValid())
        AddComponent<SpriteAnimationComponent>();
}

void Entity::RemoveAnimation() const
{
    if (IsValid())
        RemoveComponent<SpriteAnimationComponent>();
}

std::string Entity::GetAnimationSetPath() const
{
    if (!IsValid() || !HasAnimation())
        return {};

    return GetComponent<SpriteAnimationComponent>().GetAnimationSetRef().path;
}

std::string Entity::GetAnimationClipName() const
{
    if (!IsValid() || !HasAnimation())
        return {};

    return GetComponent<SpriteAnimationComponent>().GetRequestedClipName();
}

void Entity::SetAnimationSetPath(const std::string& path) const
{
    if (IsValid())
        AddComponent<SpriteAnimationComponent>().SetAnimationSetPath(path);
}

void Entity::PlayAnimation(const std::string& clipName, bool restartIfSame) const
{
    if (IsValid() && m_Scene->IsPlaying())
        AddComponent<SpriteAnimationComponent>().Play(clipName, restartIfSame);
}

void Entity::StopAnimation()
{
    if (IsValid() && HasAnimation())
        m_Registry->get<SpriteAnimationComponent>(m_Entity).Stop();
}

void Entity::ResetAnimation()
{
    if (IsValid() && HasAnimation())
        m_Registry->get<SpriteAnimationComponent>(m_Entity).Reset();
}

bool Entity::IsAnimationPlaying() const
{
    return IsValid() && HasAnimation() && GetComponent<SpriteAnimationComponent>().IsPlaying();
}

bool Entity::HasAnimationFinished() const
{
    return IsValid() && HasAnimation() && GetComponent<SpriteAnimationComponent>().HasFinished();
}

bool Entity::IsPlayingAnimationClip(const std::string& clipName) const
{
    return IsValid() && HasAnimation() && GetComponent<SpriteAnimationComponent>().IsPlayingClip(clipName);
}

bool Entity::HasTileMap() const
{
    return IsValid() && HasComponent<TileMapComponent>();
}

void Entity::EnsureTileMap() const
{
    if (!IsValid() || HasComponent<TileMapComponent>())
        return;

    m_Registry->emplace<TileMapComponent>(m_Entity);
}

void Entity::RemoveTileMap() const
{
    if (IsValid() && HasComponent<TileMapComponent>())
        m_Registry->remove<TileMapComponent>(m_Entity);
}

uint32_t Entity::GetTileMapWidth() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).width : 0;
}

uint32_t Entity::GetTileMapHeight() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).height : 0;
}

glm::vec2 Entity::GetTileSize() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).tileSize : glm::vec2{32.0f, 32.0f};
}

void Entity::SetTileSize(const glm::vec2& size) const
{
    if (!HasTileMap())
        return;

    m_Registry->get<TileMapComponent>(m_Entity).tileSize = size;
    m_Scene->MarkTransformDirty(m_ID);
}

glm::ivec2 Entity::GetTileAtlasCellSize() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).atlasCellSize : glm::ivec2{32, 32};
}

void Entity::SetTileAtlasCellSize(const glm::ivec2& size) const
{
    if (!HasTileMap())
        return;

    auto& tileMap = m_Registry->get<TileMapComponent>(m_Entity);
    tileMap.atlasCellSize = {
        std::max(1, size.x),
        std::max(1, size.y)
    };
}

std::string Entity::GetTileMapTexturePath() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).tilesetTexturePath : std::string{};
}

void Entity::SetTileMapTexturePath(const std::string& path) const
{
    if (!HasTileMap())
        return;

    m_Registry->get<TileMapComponent>(m_Entity).tilesetTexturePath = path;
}

uint32_t Entity::GetTileMapColumns() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).columns : 1;
}

uint32_t Entity::GetTileMapRows() const
{
    return HasTileMap() ? m_Registry->get<TileMapComponent>(m_Entity).rows : 1;
}

void Entity::SetTileMapGrid(uint32_t columns, uint32_t rows) const
{
    if (!HasTileMap())
        return;

    auto& tileMap = m_Registry->get<TileMapComponent>(m_Entity);
    tileMap.columns = std::max<uint32_t>(1, columns);
    tileMap.rows = std::max<uint32_t>(1, rows);
}

void Entity::ResizeTileMap(uint32_t width, uint32_t height) const
{
    if (!HasTileMap())
        return;

    auto& tileMap = m_Registry->get<TileMapComponent>(m_Entity);

    std::vector<int32_t> resizedTiles(width * height, -1);

    const uint32_t copyWidth = std::min(tileMap.width, width);
    const uint32_t copyHeight = std::min(tileMap.height, height);

    for (uint32_t y = 0; y < copyHeight; ++y)
    {
        for (uint32_t x = 0; x < copyWidth; ++x)
        {
            resizedTiles[y * width + x] = tileMap.tiles[y * tileMap.width + x];
        }
    }

    tileMap.width = width;
    tileMap.height = height;
    tileMap.tiles = std::move(resizedTiles);
}

int32_t Entity::GetTile(int x, int y) const
{
    if (!HasTileMap())
        return -1;

    const auto& tileMap = m_Registry->get<TileMapComponent>(m_Entity);
    if (x < 0 || y < 0)
        return -1;

    const uint32_t tileX = static_cast<uint32_t>(x);
    const uint32_t tileY = static_cast<uint32_t>(y);

    if (tileX >= tileMap.width || tileY >= tileMap.height)
        return -1;

    return tileMap.tiles[tileY * tileMap.width + tileX];
}

void Entity::SetTile(int x, int y, int32_t tileID) const
{
    if (!HasTileMap())
        return;

    auto& tileMap = m_Registry->get<TileMapComponent>(m_Entity);
    if (x < 0 || y < 0)
        return;

    const uint32_t tileX = static_cast<uint32_t>(x);
    const uint32_t tileY = static_cast<uint32_t>(y);

    if (tileX >= tileMap.width || tileY >= tileMap.height)
        return;

    tileMap.tiles[tileY * tileMap.width + tileX] = tileID;
}
