#pragma once

class SpriteRenderer;

class SpriteAnimation
{
public:
    void SetClip(int startColumn, int row, int frameCount, int cellWidth, int cellHeight);
    void SetFrameDuration(float seconds);
    void SetLooping(bool looping);
    void Play();
    void Stop();
    void Reset();

    void Update(float deltaTime, SpriteRenderer& sprite);

private:
    int m_StartColumn = 0;
    int m_Row = 0;
    int m_FrameCount = 1;
    int m_CellWidth = 0;
    int m_CellHeight = 0;

    float m_FrameDuration = 0.1f;
    float m_ElapsedTime = 0.0f;

    int m_CurrentFrame = 0;
    bool m_Playing = true;
    bool m_Looping = true;
};
