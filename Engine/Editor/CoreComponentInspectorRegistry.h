#pragma once

#include <functional>
#include <string>
#include <vector>

class Entity;
class SceneEditorPanel;

struct CoreComponentInspectorEntry
{
    std::string sectionName;
    std::function<bool(const Entity&)> hasComponent;
    std::function<void(Entity&)> ensureComponent;
    std::function<void(Entity&)> removeComponent;
    std::function<void(Entity&, SceneEditorPanel&)> draw;
};

class CoreComponentInspectorRegistry
{
public:
    static CoreComponentInspectorRegistry& Get();

    void Register(CoreComponentInspectorEntry entry);
    const std::vector<CoreComponentInspectorEntry>& GetEntries() const { return m_Entries; }

private:
    std::vector<CoreComponentInspectorEntry> m_Entries;
};
