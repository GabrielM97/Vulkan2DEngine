#pragma once

#include <string>
#include <vector>

struct AnimationClip
{
    std::string name;

    int row = 0;
    int startColumn = 0;
    int frameCount = 1;

    int cellWidth = 0;
    int cellHeight = 0;

    float frameDuration = 0.1f;
    bool looping = true;
};

class SpriteAnimationSet
{
public:
    bool LoadFromCSV(const std::string& path);

    const AnimationClip* FindClip(const std::string& name) const;
    const std::vector<AnimationClip>& GetClips() const { return m_Clips; }

private:
    std::vector<AnimationClip> m_Clips;
};