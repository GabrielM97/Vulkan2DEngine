# Vulkan2DRenderer Documentation

## Overview
`Vulkan2DRenderer` is a small 2D game engine built around a few clear layers:

- `Application` owns the window, renderer, ImGui frame lifecycle, and editor play/edit transitions.
- `Scene` owns game objects, transforms, serialization, rendering submission, animation updates, and simple AABB collision checks.
- `Entity` is the low-level handle used to manipulate a game object and its components.
- `Object` and `Actor` wrap `Entity` into typed gameplay classes such as `Player`.
- `EditorLayer` provides the in-engine viewport, hierarchy, inspector, tile map tools, and play/stop controls.
- `VulkanRenderer` is the concrete 2D renderer used by both the game and the editor viewport.

The current engine is already set up for simple sprite-based games with:

- a runtime/editor application loop
- entity creation and hierarchy
- sprite rendering with layers, tinting, flips, and source rectangles
- CSV-driven sprite animation
- tile map authoring with multiple layers
- tileset assets with per-tile collision flags
- box collider based movement blocking
- JSON scene save/load
- reflected scene components shown in the editor inspector

## Build And Run
Prerequisites:

- Vulkan SDK installed and `VULKAN_SDK` configured
- `vcpkg` available and integrated for manifest mode
- CMake 3.20+

Typical setup:

```powershell
cmake --preset windows-debug
cmake --build --preset build-debug
```
f
The root `CMakeLists.txt` also:

- builds the `Engine` static libraries
- builds the `Sandbox` sample app
- compiles Vulkan shaders from `Assets/Shaders`
- copies runtime textures and animation CSVs into the build output

## Project Structure
Top-level layout:

- `Engine/Core`: app loop and per-frame input state
- `Engine/Platform`: GLFW window wrapper
- `Engine/Renderer`: Vulkan renderer, buffers, textures, swapchain, ImGui integration
- `Engine/Scene`: ECS-facing scene model, entity handle, camera, sprites, animation, serialization, tile assets
- `Engine/Object`: typed game object wrappers
- `Engine/Component`: custom scene-component registration and editor-facing component metadata
- `Engine/Reflection`: property reflection and JSON property serialization
- `Engine/Editor`: hierarchy, inspector, scene viewport, tile map and tileset tools
- `Sandbox`: sample game/application showing intended usage

## Runtime Flow
The main runtime path is:

1. `main()` creates `SandboxApp`.
2. `Application::Init()` creates the window, initializes Vulkan, then initializes ImGui.
3. `SandboxApp::OnInit()` prepares the initial scene and editor controller state.
4. `Application::Run()` loops until the window closes.
5. Each frame:
   - GLFW events are polled.
   - ImGui begins a frame.
   - resize handling recreates renderer resources when needed.
   - the editor panels are drawn.
   - play/stop requests are consumed.
   - `InputState` snapshots keyboard and mouse state.
   - `OnUpdate(deltaTime)` runs gameplay/editor logic.
   - `VulkanRenderer` records and submits the scene and UI render passes.
6. `Application::Shutdown()` tears down ImGui and Vulkan.

## Core Concepts
### Application
To build a game, derive from `Application` and override the hooks you need:

- `OnInit()`
- `OnUpdate(float deltaTime)`
- `OnRender(VulkanRenderer& renderer)`
- `OnShutdown()`
- `GetEditorScene()`
- optional play-mode hooks:
  - `GetPlayModeSnapshotPath()`
  - `OnEnterPlayMode()`
  - `OnExitPlayMode()`

The editor play button works by saving the current scene to a snapshot file, starting play mode, and then reloading the snapshot when play stops. That gives you Unity-like edit/play separation without a second runtime scene instance.

### Scene And Entity
`Scene` stores game objects in an `entt::registry`, but the main API is exposed through:

- `Scene` for creation, queries, hierarchy, serialization, camera control, rendering, and collision
- `Entity` for manipulating a single game object

Every game object automatically starts with:

- `IDComponent`
- `NameComponent`
- `ActiveComponent`
- `RelationshipComponent`
- `LocalTransformComponent`
- `WorldTransformComponent`
- `RequiredComponentsComponent`
- `ObjectTypeComponent`
- `SpriteComponent`

Optional built-in components include:

- `SpriteAnimationComponent`
- `TileMapComponent`
- `BoxColliderComponent`

### Typed Objects
If you want object-style gameplay code instead of raw entity manipulation:

- derive from `Object` for a general typed wrapper
- derive from `Actor` if you want transform and sprite helpers
- register lifecycle hooks with `REGISTER_SCENE_OBJECT(Type)`

When the scene enters play mode, `Scene::BeginPlay()` looks up object lifecycle hooks in `ObjectRegistry` using the stored `ObjectTypeComponent`.

### Scene Components And Reflection
Custom inspector-visible components derive from `SceneComponent`.

The pattern is:

1. derive from `SceneComponent`
2. use `REGISTER_SCENE_COMPONENT`
3. mark fields with reflection metadata using `PROPERTY_FIELD`
4. register reflected properties with `REGISTER_PROPERTIES`

Those reflected fields can then:

- show up in the editor
- serialize to JSON through `PropertySerialization`
- be automatically ensured on entities during scene loading

## Using The Engine
### Creating A Custom App
Minimal pattern:

```cpp
#include "Core/Application.h"
#include "Scene/Scene.h"

class MyGameApp : public Application
{
protected:
    void OnInit() override
    {
        Entity player = m_Scene.CreateEntity("Player");
        player.SetSpriteTexturePath("Assets/Textures/player.png");
        player.SetSpriteSize({32.0f, 32.0f});
    }

    void OnUpdate(float deltaTime) override
    {
        if (IsEditorPlaying())
            m_Scene.Update(deltaTime);
    }

    void OnRender(VulkanRenderer& renderer) override
    {
        m_Scene.Render(renderer);
    }

    Scene* GetEditorScene() override
    {
        return &m_Scene;
    }

private:
    Scene m_Scene;
};
```

### Creating Entities
Use `Scene::CreateEntity` for raw scene objects:

```cpp
Entity wall = m_Scene.CreateEntity("Wall");
wall.SetPosition({200.0f, 0.0f});
wall.SetSpriteTexturePath("Assets/Textures/texture.jpg");
wall.SetSpriteSize({32.0f, 32.0f});
wall.SetSpriteLayer(1);
```

### Creating Typed Objects
Use `Scene::Place<T>()` or `Scene::Spawn<T>()` for registered object types:

```cpp
Player player = m_Scene.Place<Player>();
Entity playerEntity = m_Scene.GetEntity(player.GetID());
playerEntity.EnsureBoxCollider();
playerEntity.SetColliderBodyType(ColliderBodyType::Dynamic);
playerEntity.SetBoxColliderSize({64.0f, 64.0f});
```

Difference:

- `Place<T>()` creates the object but does not call `BeginPlay()` immediately
- `Spawn<T>()` will call `BeginPlay()` right away if the scene is already playing

### Transform And Hierarchy
You can work in either local or world space:

```cpp
Entity parent = m_Scene.CreateEntity("Parent");
Entity child = m_Scene.CreateEntity("Child");

parent.SetPosition({100.0f, 100.0f});
child.SetPosition({140.0f, 120.0f});
child.SetParent(parent.GetID());
```

Useful notes:

- local transforms are preserved relative to parents
- world transforms are lazily recomputed using dirty propagation
- sibling order affects hierarchy ordering and root sorting

### Sprites
Sprites are always present on an entity.

Common operations:

```cpp
entity.SetSpriteTexturePath("Assets/Textures/character-spritesheet.png");
entity.SetSpriteSize({64.0f, 64.0f});
entity.SetSpriteTint({1.0f, 1.0f, 1.0f, 1.0f});
entity.SetSpriteLayer(0);
entity.SetSpriteFlipX(true);
entity.SetSpriteSourceRectFromGrid(0, 10, 64, 64);
```

Behavior to remember:

- setting a source rect also updates sprite size to the rect size
- render order is sorted by sprite layer
- invisible sprites are skipped during normal scene rendering

### Sprite Animation
Animation is driven by a CSV file loaded into `SpriteAnimationSet`.

Typical setup:

```cpp
entity.EnsureAnimation();
entity.SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
```

Runtime control:

```cpp
entity.PlayAnimation("Walk");
entity.StopAnimation();
entity.ResetAnimation();
```

Animation updates only advance during `Scene::Update(deltaTime)`, so your app should call `Scene::Update()` during play mode.

CSV format is interpreted as:

- `name`
- `row`
- `startColumn`
- `frameCount`
- `cellWidth`
- `cellHeight`
- `frameDuration`
- `looping`

### Tile Maps
Tile maps are stored as a `TileMapComponent` on a normal entity.

Typical setup:

```cpp
Entity map = m_Scene.CreateEntity("Map");
map.EnsureTileMap();
map.ResizeTileMap(64, 32);
map.SetTileSize({32.0f, 32.0f});
map.SetTileMapGrid(8, 8);
map.SetTileMapTexturePath("Assets/Textures/tiles.png");
map.SetTile(0, 0, 3);
map.SetTile(1, 0, 4);
```

Tile map features:

- multiple layers
- active layer editing
- visibility flags per layer
- collision-enabled layers
- optional external tile map asset path
- optional external tileset asset path

### Tileset Assets
`TileSetAsset` stores:

- texture path
- atlas columns and rows
- a `solid` flag per tile

This lets tile map collision be driven per tile ID rather than by the map alone.

### Collision
Collision is intentionally simple:

- axis-aligned box colliders only
- static and dynamic body types
- trigger flag
- solid overlap tests against static colliders and collision-enabled tile layers
- axis-separated movement resolution

Setup example:

```cpp
entity.EnsureBoxCollider();
entity.SetBoxColliderSize({32.0f, 32.0f});
entity.SetBoxColliderOffset({0.0f, 0.0f});
entity.SetColliderBodyType(ColliderBodyType::Dynamic);
```

Move with collision:

```cpp
const glm::vec2 input = {1.0f, 0.0f};
entity.MoveWithCollision(input * 150.0f * deltaTime);
```

Current implementation notes:

- overlap checks block against `Static` colliders
- triggers are ignored for blocking
- tile collisions use layer collision flags and optional tileset solidity data
- no physics impulses, gravity, or continuous collision detection are present yet

### Saving And Loading Scenes
Scenes serialize to JSON with `SceneSerializer`.

Runtime API:

```cpp
m_Scene.SaveToFile("Assets/Scenes/MyScene.json");
m_Scene.LoadFromFile("Assets/Scenes/MyScene.json");
```

Serialized scene data includes:

- object IDs and names
- active state
- hierarchy and destroy policy
- local transform
- sprite state
- object type
- tracked custom scene components
- optional animation, tile map, and box collider data

The serializer restores entities first, then applies parent relationships afterward so IDs remain stable.

### Input
`InputState` snapshots the current and previous keyboard/mouse state once per frame.

Useful checks:

- `IsKeyDown`
- `WasKeyPressed`
- `WasKeyReleased`
- `IsMouseButtonDown`
- `WasMouseButtonPressed`
- `WasMouseButtonReleased`

Viewport-aware helpers:

- `CanUseViewportInput()`
- `CanUseEditorViewportInput()`
- `CanUseRuntimeViewportInput()`
- `CanUseEditorShortcuts()`

This is important because the engine routes gameplay/editor controls through the scene viewport focus state.

## Editor Workflow
The editor is integrated directly into the application loop.

Main panels:

- scene viewport
- hierarchy + inspector
- tile map editor
- tileset editor

### Scene Viewport
The scene viewport is rendered into a dedicated Vulkan render target and then shown as an ImGui image.

Editor controls:

- `W/A/S/D` or arrow keys: move camera in edit mode
- `Q/E`: zoom camera in edit mode
- left click: select an object in the viewport
- `F5`: save the current scene
- `F9`: reload the current scene
- `F10`: toggle collision debug rendering

### Hierarchy And Inspector
`SceneEditorPanel` manages:

- hierarchy tree display
- selection state
- core component editing
- reflected custom component editing

### Tile Map Tooling
`TileMapEditorPanel` and `TileMapEditorController` together handle:

- tile atlas selection
- painting
- erasing
- fill mode
- tile layer management
- tile map asset save/load
- undo/redo for tile painting strokes
- tile-map-only viewport rendering

### Tileset Tooling
`TileSetEditorPanel` edits a `TileSetAsset` directly:

- choose texture
- choose atlas dimensions
- inspect/select tiles
- mark tiles as solid
- save/load tileset JSON assets

## Example: Simple Player Setup
This mirrors the existing sandbox pattern:

```cpp
void SandboxApp::CreateDefaultScene()
{
    m_Player = m_Scene.Place<Player>();

    Entity playerEntity = m_Scene.GetEntity(m_Player.GetID());
    playerEntity.EnsureBoxCollider();
    playerEntity.SetBoxColliderSize({64.f, 64.f});
    playerEntity.SetColliderBodyType(ColliderBodyType::Dynamic);

    Entity wall = m_Scene.CreateEntity("Wall");
    wall.SetPosition({200.0f, 0.0f});
    wall.SetSpriteSize({32.0f, 32.0f});
    wall.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    wall.EnsureBoxCollider();
    wall.SetBoxColliderSize({32.0f, 32.0f});
    wall.SetColliderBodyType(ColliderBodyType::Static);
}
```

Runtime movement:

```cpp
if (IsEditorPlaying() && m_Player.IsValid())
{
    Entity playerEntity = m_Scene.GetEntity(m_Player.GetID());
    const glm::vec2 moveDelta = playerInput * 150.0f * deltaTime;
    playerEntity.MoveWithCollision(moveDelta);
}
```

## Example: Defining A Custom Scene Component
Pattern:

```cpp
class HealthComponent : public SceneComponent
{
public:
    PROPERTY_FIELD(int, maxHealth, EditAnywhere | Save, 100);
    PROPERTY_FIELD(int, currentHealth, EditAnywhere | Save, 100);

    REGISTER_SCENE_COMPONENT(HealthComponent, 10, "HealthComponent")
};

REGISTER_PROPERTIES(
    HealthComponent,
    PROPERTY_INFO(HealthComponent, maxHealth, EditAnywhere | Save),
    PROPERTY_INFO(HealthComponent, currentHealth, EditAnywhere | Save)
);
```

Attach it:

```cpp
Entity enemy = m_Scene.CreateEntity("Enemy");
enemy.AddComponent<HealthComponent>();
```

Because the component is a registered `SceneComponent`, it can participate in the tracked-component serialization/editor pipeline.

## Example: Defining A New Actor Type
Pattern:

```cpp
class Coin : public Actor
{
public:
    using Actor::Actor;

    static const char* StaticName() { return "Coin"; }
    const char* GetTypeName() const override { return StaticName(); }

    void Initialize() override
    {
        ConfigureSprite("Assets/Textures/coin.png", {16.0f, 16.0f});
    }

    void BeginPlay() override
    {
    }

    REGISTER_SCENE_OBJECT(Coin)
};
```

Create it with:

```cpp
Coin coin = m_Scene.Place<Coin>();
```

## Serialization Formats
### Scene JSON
Scene files store a versioned list of entities. Each entry can contain:

- identity and object type
- activity and relationship state
- local transform
- sprite data
- custom scene components keyed by component ID
- optional animation block
- optional tile map block
- optional box collider block

### Tile Map JSON
Tile map asset files are separate JSON documents with:

- version
- `assetType = "TileMap"`
- tile map dimensions and tile size
- atlas metadata
- texture and tileset asset paths
- layered tile data

### Tileset JSON
Tileset asset files are separate JSON documents with:

- version
- `assetType = "TileSet"`
- texture path
- atlas dimensions
- per-tile flags such as `solid`

## Notes And Current Limits
The engine is in a strong "simple 2D game" state, but a few boundaries are worth documenting:

- rendering is 2D-focused and sprite/tile based
- collision is AABB-based, not full rigid-body physics
- scene save/load is JSON and currently version `1`
- animation CSV loading is intentionally minimal and expects a clean fixed-column format
- editor play mode restores from a snapshot file instead of running a fully separate world clone
- custom scene component editing depends on reflected property types currently supported by `PropertyTypeTraits`

## Suggested Starting Pattern For New Games
For a new project using this engine:

1. derive your own app from `Application`
2. keep a single `Scene` member
3. create typed actors with `Place<T>()`
4. use `Entity` helpers for built-in sprite/tile/collider state
5. call `Scene::Update()` only while playing
6. call `Scene::Render()` from `OnRender`
7. use custom `SceneComponent` types for editor-visible gameplay data
8. use the sandbox scene/editor workflow as the reference implementation
