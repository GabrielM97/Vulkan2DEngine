#pragma once

#include <glm/glm.hpp>

#include "Component/SceneComponentRegistry.h"
#include "Reflection/Property.h"
#include "Reflection/PropertyTypeTraits.h"

struct DebugSettingsComponent : public SceneComponent
{
    PROPERTY_FIELD(bool, showBounds, EditAnywhere | Save, true);
    PROPERTY_FIELD(float, moveSpeedMultiplier, EditAnywhere | Save, 1.0f);
    PROPERTY_FIELD(glm::vec2, spawnOffset, EditAnywhere | Save, glm::vec2{0.0f, 0.0f});

    float runtimeAccumulator = 0.0f;

    REGISTER_SCENE_COMPONENT(DebugSettingsComponent, 2, "DebugSettingsComponent")
};

REGISTER_PROPERTIES(
    DebugSettingsComponent,
    PROPERTY_INFO(DebugSettingsComponent, showBounds, EditAnywhere | Save),
    PROPERTY_INFO(DebugSettingsComponent, moveSpeedMultiplier, EditAnywhere | Save),
    PROPERTY_INFO(DebugSettingsComponent, spawnOffset, EditAnywhere | Save)
);
