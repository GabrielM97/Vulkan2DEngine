#include "SpriteAnimation.h"
#include "SpriteAnimationSet.h"
#include "SpriteRenderer.h"

void SpriteAnimation::SetAnimationSetPath(const std::string& path)
{
    m_AnimationSetRef.path = path;
    m_CurrentClip = nullptr;
    m_RequestedClipName.clear();
    Reset();
    m_Playing = false;
    m_Finished = false;
}

void SpriteAnimation::Play(const std::string& clipName, bool restartIfSame)
{
    if (!restartIfSame && m_Playing && m_RequestedClipName == clipName)
        return;

    m_RequestedClipName = clipName;
    m_CurrentClip = nullptr;
    m_CurrentFrame = 0;
    m_ElapsedTime = 0.0f;
    m_Playing = true;
    m_Finished = false;
}

void SpriteAnimation::Stop()
{
    m_Playing = false;
}

void SpriteAnimation::Reset()
{
    m_CurrentFrame = 0;
    m_ElapsedTime = 0.0f;
    m_Finished = false;
}

bool SpriteAnimation::IsPlayingClip(const std::string& clipName) const
{
    return m_Playing && m_RequestedClipName == clipName;
}

void SpriteAnimation::Update(float deltaTime, SpriteRenderer& sprite, const SpriteAnimationSet* animationSet)
{
    if (!m_Playing || animationSet == nullptr)
        return;

    if (m_CurrentClip == nullptr ||
        !m_RequestedClipName.empty() && m_CurrentClip->name != m_RequestedClipName)
    {
        m_CurrentClip = animationSet->FindClip(m_RequestedClipName);
        m_CurrentFrame = 0;
        m_ElapsedTime = 0.0f;
        m_Finished = false;
    }

    if (m_CurrentClip == nullptr)
        return;

    if (m_CurrentClip->frameCount <= 0 ||
        m_CurrentClip->cellWidth <= 0 ||
        m_CurrentClip->cellHeight <= 0 ||
        m_CurrentClip->frameDuration <= 0.0f)
    {
        return;
    }

    m_ElapsedTime += deltaTime;

    while (m_ElapsedTime >= m_CurrentClip->frameDuration)
    {
        m_ElapsedTime -= m_CurrentClip->frameDuration;
        ++m_CurrentFrame;

        if (m_CurrentFrame >= m_CurrentClip->frameCount)
        {
            if (m_CurrentClip->looping)
            {
                m_CurrentFrame = 0;
            }
            else
            {
                m_CurrentFrame = m_CurrentClip->frameCount - 1;
                m_Playing = false;
                m_Finished = true;
            }
        }
    }

    sprite.SetSourceRectFromGrid(
        m_CurrentClip->startColumn + m_CurrentFrame,
        m_CurrentClip->row,
        m_CurrentClip->cellWidth,
        m_CurrentClip->cellHeight
    );
}
