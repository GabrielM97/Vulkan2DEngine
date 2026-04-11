#include "SpriteAnimationSet.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace
{
    std::vector<std::string> SplitCSVLine(const std::string& line)
    {
        std::vector<std::string> tokens;
        std::stringstream stream(line);
        std::string token;

        while (std::getline(stream, token, ','))
            tokens.push_back(token);

        return tokens;
    }
}

bool SpriteAnimationSet::LoadFromCSV(const std::string& path)
{
    m_Clips.clear();

    std::ifstream file(path);
    if (!file.is_open())
        return false;

    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        std::vector<std::string> tokens = SplitCSVLine(line);
        if (tokens.size() < 8)
            continue;

        AnimationClip clip{};
        clip.name = tokens[0];
        clip.row = std::stoi(tokens[1]);
        clip.startColumn = std::stoi(tokens[2]);
        clip.frameCount = std::stoi(tokens[3]);
        clip.cellWidth = std::stoi(tokens[4]);
        clip.cellHeight = std::stoi(tokens[5]);
        clip.frameDuration = std::stof(tokens[6]);
        clip.looping = std::stoi(tokens[7]) != 0;

        m_Clips.push_back(clip);
    }

    return !m_Clips.empty();
}

const AnimationClip* SpriteAnimationSet::FindClip(const std::string& name) const
{
    for (const AnimationClip& clip : m_Clips)
    {
        if (clip.name == name)
            return &clip;
    }

    return nullptr;
}