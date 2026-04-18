#include "Editor/CoreComponentInspectorRegistry.h"

CoreComponentInspectorRegistry& CoreComponentInspectorRegistry::Get()
{
    static CoreComponentInspectorRegistry instance;
    return instance;
}

void CoreComponentInspectorRegistry::Register(CoreComponentInspectorEntry entry)
{
    m_Entries.push_back(std::move(entry));
}
