#include "GameObjectHandle.h"

#include "Scene.h"

bool GameObjectHandle::IsValid() const
{
    return m_Scene != nullptr && m_Scene->IsValidHandle(m_Entity, m_ID);
}

bool GameObjectHandle::IsActive() const
{
    return IsValid() && m_Scene->IsGameObjectActive(m_ID);
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
    return IsValid() && m_Scene->HasParent(m_ID);
}

GameObjectID GameObjectHandle::GetParentID() const
{
    return IsValid() ? m_Scene->GetParentID(m_ID) : 0;
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

LocalTransformComponent GameObjectHandle::GetLocalTransform() const
{
    return IsValid() ? m_Scene->GetLocalTransform(m_ID) : LocalTransformComponent{};
}

Transform2D GameObjectHandle::GetWorldTransform() const
{
    return IsValid() ? m_Scene->GetWorldTransform(m_ID) : Transform2D{};
}

glm::vec2 GameObjectHandle::GetLocalPosition() const
{
    return IsValid() ? m_Scene->GetLocalPosition(m_ID) : glm::vec2{};
}

glm::vec2 GameObjectHandle::GetLocalScale() const
{
    return IsValid() ? m_Scene->GetLocalScale(m_ID) : glm::vec2{};
}

glm::vec2 GameObjectHandle::GetLocalPivot() const
{
    return IsValid() ? m_Scene->GetLocalPivot(m_ID) : glm::vec2{};
}

float GameObjectHandle::GetLocalRotation() const
{
    return IsValid() ? m_Scene->GetLocalRotation(m_ID) : 0.0f;
}

glm::vec2 GameObjectHandle::GetWorldPosition() const
{
    return IsValid() ? m_Scene->GetWorldPosition(m_ID) : glm::vec2{};
}

glm::vec2 GameObjectHandle::GetWorldScale() const
{
    return IsValid() ? m_Scene->GetWorldScale(m_ID) : glm::vec2{};
}

glm::vec2 GameObjectHandle::GetWorldPivot() const
{
    return IsValid() ? m_Scene->GetWorldPivot(m_ID) : glm::vec2{};
}

float GameObjectHandle::GetWorldRotation() const
{
    return IsValid() ? m_Scene->GetWorldRotation(m_ID) : 0.0f;
}

void GameObjectHandle::SetLocalTransform(const LocalTransformComponent& transform) const
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
    return IsValid() ? m_Scene->GetSpriteTexturePath(m_ID) : std::string{};
}

IntRect GameObjectHandle::GetSpriteSourceRect() const
{
    return IsValid() ? m_Scene->GetSpriteSourceRect(m_ID) : IntRect{};
}

bool GameObjectHandle::SpriteUsesSourceRect() const
{
    return IsValid() && m_Scene->SpriteUsesSourceRect(m_ID);
}

glm::vec2 GameObjectHandle::GetSpriteSize() const
{
    return IsValid() ? m_Scene->GetSpriteSize(m_ID) : glm::vec2{};
}

glm::vec4 GameObjectHandle::GetSpriteTint() const
{
    return IsValid() ? m_Scene->GetSpriteTint(m_ID) : glm::vec4{};
}

int GameObjectHandle::GetSpriteLayer() const
{
    return IsValid() ? m_Scene->GetSpriteLayer(m_ID) : 0;
}

bool GameObjectHandle::IsSpriteVisible() const
{
    return IsValid() && m_Scene->IsSpriteVisible(m_ID);
}

bool GameObjectHandle::IsSpriteFlippedX() const
{
    return IsValid() && m_Scene->IsSpriteFlippedX(m_ID);
}

bool GameObjectHandle::IsSpriteFlippedY() const
{
    return IsValid() && m_Scene->IsSpriteFlippedY(m_ID);
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

void GameObjectHandle::SetSpriteTint(const glm::vec4& tint) const
{
    if (IsValid())
        m_Scene->SetSpriteTint(m_ID, tint);
}

void GameObjectHandle::SetSpriteLayer(int layer) const
{
    if (IsValid())
        m_Scene->SetSpriteLayer(m_ID, layer);
}

void GameObjectHandle::SetSpriteVisible(bool visible) const
{
    if (IsValid())
        m_Scene->SetSpriteVisible(m_ID, visible);
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

bool GameObjectHandle::HasAnimation() const
{
    return IsValid() && m_Scene->HasAnimation(m_ID);
}

void GameObjectHandle::EnsureAnimation() const
{
    if (IsValid())
        m_Scene->EnsureAnimation(m_ID);
}

void GameObjectHandle::RemoveAnimation() const
{
    if (IsValid())
        m_Scene->RemoveAnimation(m_ID);
}

std::string GameObjectHandle::GetAnimationSetPath() const
{
    return IsValid() ? m_Scene->GetAnimationSetPath(m_ID) : std::string{};
}

std::string GameObjectHandle::GetAnimationClipName() const
{
    return IsValid() ? m_Scene->GetAnimationClipName(m_ID) : std::string{};
}

void GameObjectHandle::SetAnimationSetPath(const std::string& path) const
{
    if (IsValid())
        m_Scene->SetAnimationSetPath(m_ID, path);
}

void GameObjectHandle::PlayAnimation(const std::string& clipName, bool restartIfSame) const
{
    if (IsValid())
        m_Scene->PlayAnimation(m_ID, clipName, restartIfSame);
}

void GameObjectHandle::StopAnimation() const
{
    if (IsValid())
        m_Scene->StopAnimation(m_ID);
}

void GameObjectHandle::ResetAnimation() const
{
    if (IsValid())
        m_Scene->ResetAnimation(m_ID);
}

bool GameObjectHandle::IsAnimationPlaying() const
{
    return IsValid() && m_Scene->IsAnimationPlaying(m_ID);
}

bool GameObjectHandle::HasAnimationFinished() const
{
    return IsValid() && m_Scene->HasAnimationFinished(m_ID);
}

bool GameObjectHandle::IsPlayingAnimationClip(const std::string& clipName) const
{
    return IsValid() && m_Scene->IsPlayingAnimationClip(m_ID, clipName);
}

std::string GameObjectHandle::GetName() const
{
    return IsValid() ? m_Scene->GetGameObjectName(m_ID) : std::string{};
}

void GameObjectHandle::SetName(const std::string& name) const
{
    if (IsValid())
        m_Scene->SetGameObjectName(m_ID, name);
}
