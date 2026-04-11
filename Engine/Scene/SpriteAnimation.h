#pragma once

#include <string>

struct AnimationSetRef
{
    std::string path;
};

struct AnimationClip;
class SpriteRenderer;
class SpriteAnimationSet;

class SpriteAnimation
{
public:
    void SetAnimationSetPath(const std::string& path);

    void Play(const std::string& clipName, bool restartIfSame = false);
    void Stop();
    void Reset();

    bool IsPlaying() const { return m_Playing; }
    bool IsPlayingClip(const std::string& clipName) const;
    const std::string& GetRequestedClipName() const { return m_RequestedClipName; }
    const AnimationSetRef& GetAnimationSetRef() const { return m_AnimationSetRef; }
    const AnimationClip* GetCurrentClip() const { return m_CurrentClip; }

    void Update(float deltaTime, SpriteRenderer& sprite, const SpriteAnimationSet* animationSet);

private:
    AnimationSetRef m_AnimationSetRef;
    std::string m_RequestedClipName;
    const AnimationClip* m_CurrentClip = nullptr;

    float m_ElapsedTime = 0.0f;
    int m_CurrentFrame = 0;
    bool m_Playing = false;
};
