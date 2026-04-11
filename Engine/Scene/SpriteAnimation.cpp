#include "SpriteAnimation.h"

#include "SpriteRenderer.h"

void SpriteAnimation::SetClip(int startColumn, int row, int frameCount, int cellWidth, int cellHeight)
{
    m_StartColumn = startColumn;
    m_Row = row;
    m_FrameCount = frameCount;
    m_CellWidth = cellWidth;
    m_CellHeight = cellHeight;
    m_CurrentFrame = 0;
    m_ElapsedTime = 0.0f;
}

void SpriteAnimation::SetFrameDuration(float seconds)
{
    m_FrameDuration = seconds;
}

void SpriteAnimation::SetLooping(bool looping)
{
    m_Looping = looping;
}

void SpriteAnimation::Play()
{
    m_Playing = true;
}

void SpriteAnimation::Stop()
{
    m_Playing = false;
}

void SpriteAnimation::Reset()
{
    m_CurrentFrame = 0;
    m_ElapsedTime = 0.0f;
}

void SpriteAnimation::Update(float deltaTime, SpriteRenderer& sprite)
{
    if (!m_Playing || m_FrameCount <= 0 || m_CellWidth <= 0 || m_CellHeight <= 0)
        return;

    m_ElapsedTime += deltaTime;

    while (m_ElapsedTime >= m_FrameDuration)
    {
        m_ElapsedTime -= m_FrameDuration;
        ++m_CurrentFrame;

        if (m_CurrentFrame >= m_FrameCount)
        {
            if (m_Looping)
            {
                m_CurrentFrame = 0;
            }
            else
            {
                m_CurrentFrame = m_FrameCount - 1;
                m_Playing = false;
            }
        }
    }

    sprite.SetSourceRectFromGrid(
        m_StartColumn + m_CurrentFrame,
        m_Row,
        m_CellWidth,
        m_CellHeight
    );
}