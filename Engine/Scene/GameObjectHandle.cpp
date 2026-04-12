#include "GameObjectHandle.h"

#include "Scene.h"

bool GameObjectHandle::IsValid() const
{
    return m_Scene != nullptr && m_Scene->FindGameObjectByID(m_ID) != nullptr;
}

bool GameObjectHandle::IsActive() const
{
    if (const GameObject* object = IsValid() ? m_Scene->FindGameObjectByID(m_ID) : nullptr)
        return object->isActive();

    return false;
}

void GameObjectHandle::SetActive(bool active) const
{
    if (IsValid())
        m_Scene->SetGameObjectActive(m_ID, active);
}

void GameObjectHandle::Destroy() const
{
    if (IsValid())
        m_Scene->DestroyGameObject(m_ID);
}

bool GameObjectHandle::HasParent() const
{
    if (const GameObject* object = IsValid() ? m_Scene->FindGameObjectByID(m_ID) : nullptr)
        return object->HasParent();

    return false;
}

GameObjectID GameObjectHandle::GetParentID() const
{
    if (const GameObject* object = IsValid() ? m_Scene->FindGameObjectByID(m_ID) : nullptr)
        return object->GetParentID();

    return 0;
}

void GameObjectHandle::SetParent(GameObjectID parentID) const
{
    if (IsValid())
        m_Scene->SetParent(m_ID, parentID);
}

void GameObjectHandle::ClearParent() const
{
    if (IsValid())
        m_Scene->ClearParent(m_ID);
}

Transform2D GameObjectHandle::GetLocalTransform() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetLocalTransform(m_ID);
}

Transform2D GameObjectHandle::GetWorldTransform() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetWorldTransform(m_ID);
}

glm::vec2 GameObjectHandle::GetLocalPosition() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetLocalPosition(m_ID);
}

glm::vec2 GameObjectHandle::GetWorldPosition() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetWorldPosition(m_ID);
}

glm::vec2 GameObjectHandle::GetLocalScale() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetLocalScale(m_ID);
}

glm::vec2 GameObjectHandle::GetLocalPivot() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetLocalPivot(m_ID);
}

float GameObjectHandle::GetLocalRotation() const
{
    if (!IsValid())
        return 0.0f;

    return m_Scene->GetLocalRotation(m_ID);
}

glm::vec2 GameObjectHandle::GetWorldScale() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetWorldScale(m_ID);
}

glm::vec2 GameObjectHandle::GetWorldPivot() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetWorldPivot(m_ID);
}

float GameObjectHandle::GetWorldRotation() const
{
    if (!IsValid())
        return 0.0f;

    return m_Scene->GetWorldRotation(m_ID);
}

void GameObjectHandle::SetLocalTransform(const Transform2D& transform) const
{
    if (IsValid())
        m_Scene->SetLocalTransform(m_ID, transform);
}

void GameObjectHandle::SetWorldTransform(const Transform2D& transform) const
{
    if (IsValid())
        m_Scene->SetWorldTransform(m_ID, transform);
}

void GameObjectHandle::SetLocalPosition(const glm::vec2& position) const
{
    if (IsValid())
        m_Scene->SetLocalPosition(m_ID, position);
}

void GameObjectHandle::SetLocalScale(const glm::vec2& scale) const
{
    if (IsValid())
        m_Scene->SetLocalScale(m_ID, scale);
}

void GameObjectHandle::SetLocalPivot(const glm::vec2& pivot) const
{
    if (IsValid())
        m_Scene->SetLocalPivot(m_ID, pivot);
}

void GameObjectHandle::SetLocalRotation(float rotationDegrees) const
{
    if (IsValid())
        m_Scene->SetLocalRotation(m_ID, rotationDegrees);
}

void GameObjectHandle::SetWorldPosition(const glm::vec2& position) const
{
    if (IsValid())
        m_Scene->SetWorldPosition(m_ID, position);
}

void GameObjectHandle::SetWorldScale(const glm::vec2& scale) const
{
    if (IsValid())
        m_Scene->SetWorldScale(m_ID, scale);
}

void GameObjectHandle::SetWorldPivot(const glm::vec2& pivot) const
{
    if (IsValid())
        m_Scene->SetWorldPivot(m_ID, pivot);
}

void GameObjectHandle::SetWorldRotation(float rotationDegrees) const
{
    if (IsValid())
        m_Scene->SetWorldRotation(m_ID, rotationDegrees);
}

void GameObjectHandle::TranslateLocal(const glm::vec2& delta) const
{
    if (!IsValid())
        return;

    m_Scene->SetLocalPosition(m_ID, m_Scene->GetLocalPosition(m_ID) + delta);
}

std::string GameObjectHandle::GetSpriteTexturePath() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetSpriteTexturePath(m_ID);
}

IntRect GameObjectHandle::GetSpriteSourceRect() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetSpriteSourceRect(m_ID);
}

bool GameObjectHandle::SpriteUsesSourceRect() const
{
    if (!IsValid())
        return false;

    return m_Scene->SpriteUsesSourceRect(m_ID);
}

glm::vec2 GameObjectHandle::GetSpriteSize() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetSpriteSize(m_ID);
}

glm::vec4 GameObjectHandle::GetSpriteTint() const
{
    if (!IsValid())
        return {};

    return m_Scene->GetSpriteTint(m_ID);
}

int GameObjectHandle::GetSpriteLayer() const
{
    if (!IsValid())
        return 0;

    return m_Scene->GetSpriteLayer(m_ID);
}

bool GameObjectHandle::IsSpriteVisible() const
{
    if (!IsValid())
        return false;

    return m_Scene->IsSpriteVisible(m_ID);
}

bool GameObjectHandle::IsSpriteFlippedX() const
{
    if (!IsValid())
        return false;

    return m_Scene->IsSpriteFlippedX(m_ID);
}

bool GameObjectHandle::IsSpriteFlippedY() const
{
    if (!IsValid())
        return false;

    return m_Scene->IsSpriteFlippedY(m_ID);
}

void GameObjectHandle::SetSpriteTexturePath(const std::string& path) const
{
    if (IsValid())
        m_Scene->SetSpriteTexturePath(m_ID, path);
}

void GameObjectHandle::SetSpriteSourceRect(int x, int y, int width, int height) const
{
    if (IsValid())
        m_Scene->SetSpriteSourceRect(m_ID, x, y, width, height);
}

void GameObjectHandle::SetSpriteSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight) const
{
    if (IsValid())
        m_Scene->SetSpriteSourceRectFromGrid(m_ID, column, row, cellWidth, cellHeight);
}

void GameObjectHandle::ClearSpriteSourceRect() const
{
    if (IsValid())
        m_Scene->ClearSpriteSourceRect(m_ID);
}

void GameObjectHandle::SetSpriteSize(const glm::vec2& size) const
{
    if (IsValid())
        m_Scene->SetSpriteSize(m_ID, size);
}

void GameObjectHandle::SetSpriteFlipX(bool flip) const
{
    if (IsValid())
        m_Scene->SetSpriteFlipX(m_ID, flip);
}

void GameObjectHandle::SetSpriteFlipY(bool flip) const
{
    if (IsValid())
        m_Scene->SetSpriteFlipY(m_ID, flip);
}

void GameObjectHandle::SetSpriteLayer(int layer) const
{
    if (IsValid())
        m_Scene->SetSpriteLayer(m_ID, layer);
}

void GameObjectHandle::SetSpriteTint(const glm::vec4& tint) const
{
    if (IsValid())
        m_Scene->SetSpriteTint(m_ID, tint);
}

void GameObjectHandle::SetSpriteVisible(bool visible) const
{
    if (IsValid())
        m_Scene->SetSpriteVisible(m_ID, visible);
}

std::string GameObjectHandle::GetName() const
{
    if (!IsValid())
        return {};

    if (const GameObject* object = m_Scene->FindGameObjectByID(m_ID))
        return object->GetName();

    return {};
}

void GameObjectHandle::SetName(const std::string& name) const
{
    if (IsValid())
        m_Scene->SetGameObjectName(m_ID, name);
}
