# Header Reference

This file is a code-oriented reference for the engine headers. It is organized by module and focuses on what each header declares, when to include it, and how it fits into the rest of the engine.

## Core
### `Engine/Core/Application.h`
Declares the base application class.

- owns the main engine loop
- owns `Window`, `VulkanRenderer`, `ImGuiLayer`, `InputState`, and `EditorLayer`
- exposes virtual hooks for init, update, render, shutdown, and play-mode transitions
- requires derived apps to return the editor scene through `GetEditorScene()` if they want editor integration

Use this when creating a game executable or sample app.

### `Engine/Core/InputState.h`
Declares the per-frame keyboard and mouse snapshot used by both gameplay and editor tools.

- tracks current and previous keyboard state
- tracks current and previous mouse button state
- stores viewport visibility/focus and text-input state
- exposes helpers for edit-mode vs play-mode input routing

Use this in controllers and gameplay code that need debounced input.

## Platform
### `Engine/Platform/Window.h`
Declares the GLFW window wrapper.

- initializes GLFW in Vulkan mode
- exposes framebuffer size, resize state, and native `GLFWwindow*`
- is the only platform abstraction layer currently present

Use this only when working on the engine bootstrap or renderer integration.

## Math
### `Engine/Math/Transform2D.h`
Declares the engine's basic 2D transform struct.

Fields:

- `position`
- `scale`
- `pivot`
- `rotationDegrees`

This is used as both local and world transform storage.

### `Engine/Math/TransformMath2D.h`
Declares helper math for transform composition and picking.

Key functions:

- vector rotation and safe division
- world/local point conversion
- point-in-rotated-rect checks
- corner extraction for overlays and picking
- parent/child transform combination
- world-to-local conversion for reparenting

Use this when adding transform-aware editor tools or hierarchy logic.

## Reflection
### `Engine/Reflection/Property.h`
Declares the reflection metadata model for custom scene components.

Includes:

- `PropertyType`
- `PropertyFlags`
- `Property`
- `PropertyRegistry<T>`
- reflection macros such as `PROPERTY_FIELD`, `PROPERTY_INFO`, and `REGISTER_PROPERTIES`

Use this when defining reflected component fields.

### `Engine/Reflection/PropertySerialization.h`
Declares templated JSON serialization helpers for reflected component properties.

- saves only fields marked with `Save`
- deserializes fields by reflected type
- supports `float`, `bool`, `int`, `uint32_t`, `std::string`, `glm::vec2`, and `glm::vec4`

Use this through the scene component registry pipeline rather than calling it everywhere manually.

### `Engine/Reflection/PropertyTypeTraits.h`
Declares the mapping from C++ field types to engine `PropertyType`.

Use this when adding support for new reflected field types.

## Component System
### `Engine/Component/EntityComponent.h`
Declares the small base interface for component metadata.

- every component type exposes a numeric `ComponentTypeID`
- every component type exposes a human-readable type name

This is the common base for `SceneComponent`.

### `Engine/Component/SceneComponent.h`
Declares the editor/serialization aware component interface.

A `SceneComponent` can:

- ensure itself on an entity
- remove itself
- report whether an entity has it
- serialize and deserialize itself
- expose its reflected properties
- provide mutable and immutable data access

Use this when creating a gameplay data component meant to appear in the editor.

### `Engine/Component/SceneComponentRegistry.h`
Declares the global registry for `SceneComponent` types and the helper macro used to register them.

Important responsibilities:

- stores all registered component metadata objects
- bridges reflected component data with the entity registry
- implements generic `Ensure`, `Remove`, `Serialize`, and `Deserialize` behavior

Key macro:

- `REGISTER_SCENE_COMPONENT(Type, IdValue, NameValue)`

### `Engine/Component/Gameplay/PlayerMovementComponent.h`
Declares a sample gameplay component storing player movement tuning.

Fields:

- `moveSpeed` as reflected/saved data
- `runtimeVelocity` as runtime-only state

This is a good template for lightweight gameplay components.

### `Engine/Component/Gameplay/DebugSettingsComponent.h`
Declares another sample gameplay component used to demonstrate reflection and saved editor data.

Fields:

- `showBounds`
- `moveSpeedMultiplier`
- `spawnOffset`
- `runtimeAccumulator` as runtime-only state

## Scene
### `Engine/Scene/SceneComponents.h`
Declares the built-in ECS component structs and aliases used throughout the scene system.

Includes:

- identity, naming, activation, hierarchy, and transform components
- `TileMapComponent`
- `BoxColliderComponent`
- aliases tying `Transform2D`, `SpriteRenderer`, and `SpriteAnimation` into component storage

This header is the core schema of the scene registry.

### `Engine/Scene/Entity.h`
Declares the high-level handle for a single game object.

Key responsibilities:

- wraps a scene pointer, registry pointer, raw `entt::entity`, and stable object ID
- exposes world and local transform helpers
- exposes sprite, animation, tile map, and collider helpers
- exposes generic templated component add/get/remove methods
- tracks custom component IDs for serialization and editor support

This is the main "gameplay-facing" low-level API.

### `Engine/Scene/Scene.h`
Declares the main scene container.

Major responsibilities:

- create and destroy game objects
- manage hierarchy
- update animations
- render sprites and tile maps
- manage the camera
- save and load scenes
- manage custom object creation and lifecycle
- perform simple collision queries and collision-aware movement

Notable public APIs:

- `CreateEntity`
- `Place<T>()` and `Spawn<T>()`
- `Update`
- `Render`
- `SaveToFile` and `LoadFromFile`
- hierarchy helpers
- transform helpers
- collision helpers

### `Engine/Scene/SceneSerializer.h`
Declares the scene JSON serializer entry points.

- `SaveToFile`
- `LoadFromFile`

This keeps scene file IO separate from `Scene` itself.

### `Engine/Scene/Camera2D.h`
Declares the 2D camera used by scene rendering and editor viewport conversion.

Features:

- top-left world origin
- zoom control with limits
- movement speed and zoom speed settings
- world/screen conversion helpers
- cached view-projection matrix

### `Engine/Scene/SpriteRenderer.h`
Declares the built-in sprite render component data.

Stores:

- texture path
- source rectangle
- tint
- visibility
- render layer
- X/Y flip flags
- sprite size

Despite the name, this is data, not a renderer implementation.

### `Engine/Scene/SpriteAnimation.h`
Declares the runtime animation state machine for a sprite.

Stores:

- animation set path reference
- requested clip name
- current clip pointer
- playback timing and frame state

The actual clip definitions live in `SpriteAnimationSet`.

### `Engine/Scene/SpriteAnimationSet.h`
Declares the CSV-loaded clip library used by `SpriteAnimation`.

Contains:

- `AnimationClip`
- `LoadFromCSV`
- `FindClip`

Use this when changing animation asset format behavior.

### `Engine/Scene/TileMapAssetSerializer.h`
Declares save/load helpers for tile map assets stored separately from whole scenes.

- saves tile map data from an entity to a JSON asset
- loads tile map data into an entity

### `Engine/Scene/TileSetAsset.h`
Declares the tileset asset type used by tile maps.

Stores:

- source texture path
- atlas dimensions
- per-tile collision data

Use this when tile collision needs to come from atlas metadata.

## Object Layer
### `Engine/Object/Object.h`
Declares the base typed wrapper around an `Entity`.

- gives object classes an identity and name/active helpers
- exposes direct access to the wrapped entity
- requires a virtual `GetTypeName`

### `Engine/Object/Actor.h`
Declares a gameplay-oriented object base class with convenience methods.

Adds:

- transform and hierarchy helpers
- sprite helpers
- animation helpers
- required `Initialize()` hook
- optional `BeginPlay()` and `EndPlay()` hooks

Use this when creating game actors that should feel higher level than raw entities.

### `Engine/Object/ObjectRegistry.h`
Declares the registry that maps object type names to lifecycle hooks.

Purpose:

- object type auto-registration
- `BeginPlay` / `EndPlay` invocation from scene play transitions

Key macro:

- `REGISTER_SCENE_OBJECT(Type)`

### `Engine/Object/Player.h`
Declares the example player actor.

It demonstrates:

- deriving from `Actor`
- static type naming
- object registration
- `Initialize`, `BeginPlay`, and custom gameplay methods

## Renderer
### `Engine/Renderer/IRenderer2D.h`
Declares the minimal rendering interface consumed by the scene system.

Methods:

- `SetCamera`
- `DrawSprite`
- `DrawRectOutline`

This allows scene code to stay mostly renderer-agnostic.

### `Engine/Renderer/RenderTypes.h`
Declares renderer-side data structures used during batching and GPU upload.

Includes:

- `PreparedBatch`
- `QuadCommand`
- `GlobalUBO`
- `QuadInstanceData`

Use this when changing the instance layout or draw batching path.

### `Engine/Renderer/ImGuiLayer.h`
Declares the engine's ImGui integration wrapper.

Responsibilities include:

- ImGui initialization against Vulkan/GLFW
- per-frame begin/end
- render submission into Vulkan command buffers
- reporting input capture state

### `Engine/Renderer/VulkanRenderer.h`
Declares the main Vulkan-based 2D renderer.

Responsibilities:

- renderer startup and cleanup
- swapchain lifecycle
- texture loading and caching
- sprite submission and batching
- scene viewport render target management
- ImGui texture handle management
- frame begin/end and command recording

This is the main renderer implementation entry point used by `Application`.

### `Engine/Renderer/VulkanContext.h`
Declares Vulkan instance and surface setup.

### `Engine/Renderer/VulkanDevice.h`
Declares physical/logical device selection and queue retrieval.

### `Engine/Renderer/VulkanSwapChain.h`
Declares swapchain creation, image views, extent, and presentation resources.

### `Engine/Renderer/VulkanRenderPass.h`
Declares Vulkan render pass creation and ownership.

### `Engine/Renderer/VulkanFramebuffer.h`
Declares framebuffer creation for swapchain images.

### `Engine/Renderer/VulkanPipeline.h`
Declares graphics pipeline creation and pipeline layout ownership.

### `Engine/Renderer/VulkanCommandBuffer.h`
Declares allocation and lifetime management for render command buffers.

### `Engine/Renderer/VulkanSync.h`
Declares fences and semaphores used for multi-frame rendering synchronization.

### `Engine/Renderer/VulkanTexture.h`
Declares image upload, image view, sampler, and texture metadata handling.

### `Engine/Renderer/VulkanUniformBuffer.h`
Declares uniform buffer creation and updates for global shader data.

### `Engine/Renderer/VulkanVertexBuffer.h`
Declares the vertex buffer used by the quad renderer.

### `Engine/Renderer/VulkanIndexBuffer.h`
Declares the index buffer used by the quad renderer.

### `Engine/Renderer/VulkanQuadInstanceBuffer.h`
Declares the GPU buffer storing per-instance quad data.

### `Engine/Renderer/VulkanViewportRenderTarget.h`
Declares the offscreen render target used by the editor scene viewport.

For all Vulkan helper headers above, include them only when working on renderer internals; most gameplay code should only need `VulkanRenderer.h` or `IRenderer2D.h`.

## Editor
### `Engine/Editor/SceneViewportPanel.h`
Declares the ImGui scene viewport panel and its tracked state.

`SceneViewportState` stores:

- viewport pixel size
- hover/focus/visibility state
- content bounds in screen space

This state is used by input routing, picking, and world/screen conversion.

### `Engine/Editor/EditorLayer.h`
Declares the high-level editor orchestration class.

Responsibilities:

- draw the dockspace and top bar
- draw scene, tile map, and tileset panels
- track selected object
- expose play and stop button requests
- draw viewport overlays

### `Engine/Editor/EditorSceneController.h`
Declares the scene-specific editor controller.

Responsibilities:

- camera movement in edit mode
- save/reload shortcuts
- object picking
- collision debug toggling
- choosing whether to render the whole scene or tile-map-only view

### `Engine/Editor/SceneEditorPanel.h`
Declares the hierarchy and inspector UI.

Responsibilities:

- hierarchy tree rendering
- inspector synchronization with selection
- built-in component editors
- custom component inspector sections

### `Engine/Editor/CoreComponentInspectorRegistry.h`
Declares the registry of core inspector sections.

This is used to extend the inspector UI for built-in engine-managed component sections.

### `Engine/Editor/EditorViewportMath.h`
Declares viewport conversion helpers used by editor picking and tile editing.

### `Engine/Editor/TileMapEditorPanel.h`
Declares the tile map editing UI state and controls.

Capabilities:

- atlas tile selection
- paint/erase/fill mode toggles
- tile map asset load/save controls
- layer controls
- tile-map-only view toggling

### `Engine/Editor/TileMapEditorController.h`
Declares the runtime logic for tile painting interactions.

Responsibilities:

- hovered tile detection
- tile stroke tracking
- undo/redo
- stamp and fill application

### `Engine/Editor/TileSetEditorPanel.h`
Declares the UI used to edit and save tileset assets.

Responsibilities:

- texture path editing
- atlas dimension editing
- tile selection
- solidity flag editing

## Sandbox
### `Sandbox/SandboxApp.h`
Declares the sample application showing how to wire the engine together.

Shows:

- deriving from `Application`
- storing a scene and typed object handles
- editor scene integration
- play-mode snapshot path override
- editor controllers for scene and tile map editing

### `Sandbox/Main.cpp`
Provides the simplest possible executable entry point:

- construct app
- initialize
- run
- return error on failed startup

## Recommended Include Surface
If you are building gameplay code on top of the engine, the most common headers to include are:

- `Core/Application.h`
- `Scene/Scene.h`
- `Scene/Entity.h`
- `Scene/SceneComponents.h`
- `Object/Actor.h`
- `Component/SceneComponentRegistry.h`
- `Reflection/Property.h`

If you are extending tools/editor behavior, add:

- `Editor/EditorLayer.h`
- `Editor/EditorSceneController.h`
- `Editor/TileMapEditorController.h`

If you are extending rendering internals, start with:

- `Renderer/VulkanRenderer.h`
- `Renderer/IRenderer2D.h`
- `Renderer/RenderTypes.h`
