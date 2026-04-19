# Engine Function Reference

## Format
This reference is organized by subsystem and then by declared type. Each type uses the same pattern: a short summary, a practical example use case, a few highlights, and then a function-by-function breakdown with notes and example calls.

## Core

### `Application`

**Summary**
Supports the engine lifecycle by handling application flow, platform integration, or per-frame input capture.

**Example Use Case**
Use `Application` as the base class for a game or sandbox app that owns startup, per-frame updates, rendering, and shutdown.
```cpp
class MyGameApp : public Application
{
protected:
    void OnInit() override
    {
        // Create startup content here.
    }
};
```

**Highlights**
- Declared in `Engine/Core/Application.h`.
- Type kind: class.
- Documented functions: 20.

`virtual ~Application()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`bool Init()`
- What it does: Participates in lifecycle flow for `Application`.
- Notes: This is a public member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
application.Init();
```

`void Run()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
application.Run();
```

`void Shutdown()`
- What it does: Participates in lifecycle flow for `Application`.
- Notes: This is a public member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
application.Shutdown();
```

`const SceneViewportState& GetSceneViewportState() const`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
auto value = application.GetSceneViewportState();
```

`virtual void OnInit()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual void OnUpdate(float deltaTime)`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual void OnRender(VulkanRenderer& renderer)`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual void OnShutdown()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual Scene* GetEditorScene()`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`bool IsEditorPlaying() const`
- What it does: Answers a yes or no question about the current state of `Application`.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual std::string GetPlayModeSnapshotPath() const`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual void OnEnterPlayMode()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`virtual void OnExitPlayMode()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`VulkanRenderer& GetRenderer()`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`const InputState& GetInputState() const`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`EditorLayer& GetEditorLayer()`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`const EditorLayer& GetEditorLayer() const`
- What it does: Returns data from `Application` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// Call this from Application or a derived type when extending behavior.
```

`void EnterPlayMode()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Application itself.
```

`void ExitPlayMode()`
- What it does: Exposes a focused operation on `Application` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Core/Application.h`. Use it when `Application` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Application itself.
```

### `InputState`

**Summary**
Supports the engine lifecycle by handling application flow, platform integration, or per-frame input capture.

**Example Use Case**
Use `InputState` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for InputState
InputState inputState;
```

**Highlights**
- Declared in `Engine/Core/InputState.h`.
- Type kind: class.
- Documented functions: 18.

`void BeginFrame( GLFWwindow* window, bool viewportVisible, bool viewportFocused, bool textInputActive, bool editorPlaying )`
- What it does: Participates in lifecycle flow for `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
inputState.BeginFrame(/* ... */, true, true, true, true);
```

`bool IsKeyDown(int key) const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.IsKeyDown(0)) {     // React to the returned state here. }
```

`bool WasKeyPressed(int key) const`
- What it does: Exposes a focused operation on `InputState` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.WasKeyPressed(0)) {     // React to the returned state here. }
```

`bool WasKeyReleased(int key) const`
- What it does: Exposes a focused operation on `InputState` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.WasKeyReleased(0)) {     // React to the returned state here. }
```

`bool CanUseViewportInput() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.CanUseViewportInput()) {     // React to the returned state here. }
```

`bool CanUseEditorViewportInput() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.CanUseEditorViewportInput()) {     // React to the returned state here. }
```

`bool CanUseRuntimeViewportInput() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.CanUseRuntimeViewportInput()) {     // React to the returned state here. }
```

`bool CanUseEditorShortcuts() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.CanUseEditorShortcuts()) {     // React to the returned state here. }
```

`bool IsTextInputActive() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.IsTextInputActive()) {     // React to the returned state here. }
```

`bool IsEditorPlaying() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.IsEditorPlaying()) {     // React to the returned state here. }
```

`bool IsViewportVisible() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.IsViewportVisible()) {     // React to the returned state here. }
```

`bool IsViewportFocused() const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.IsViewportFocused()) {     // React to the returned state here. }
```

`bool IsMouseButtonDown(int button) const`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.IsMouseButtonDown(0)) {     // React to the returned state here. }
```

`bool WasMouseButtonPressed(int button) const`
- What it does: Exposes a focused operation on `InputState` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.WasMouseButtonPressed(0)) {     // React to the returned state here. }
```

`bool WasMouseButtonReleased(int button) const`
- What it does: Exposes a focused operation on `InputState` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (inputState.WasMouseButtonReleased(0)) {     // React to the returned state here. }
```

`static bool IsValidMouseButton(int button)`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
if (InputState.IsValidMouseButton(0)) {     // React to the returned state here. }
```

`glm::vec2 GetMouseScreenPosition() const`
- What it does: Returns data from `InputState` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
auto value = inputState.GetMouseScreenPosition();
```

`static bool IsValidKey(int key)`
- What it does: Answers a yes or no question about the current state of `InputState`.
- Notes: This is a private member in `Engine/Core/InputState.h`. Use it when `InputState` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside InputState itself.
```

## Platform

### `Window`

**Summary**
Supports the engine lifecycle by handling application flow, platform integration, or per-frame input capture.

**Example Use Case**
Use `Window` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Window
Window window;
```

**Highlights**
- Declared in `Engine/Platform/Window.h`.
- Type kind: class.
- Documented functions: 9.

`~Window()`
- What it does: Exposes a focused operation on `Window` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`bool Init()`
- What it does: Participates in lifecycle flow for `Window`.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
window.Init();
```

`bool ShouldClose() const`
- What it does: Exposes a focused operation on `Window` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
window.ShouldClose();
```

`void PollEvents() const`
- What it does: Exposes a focused operation on `Window` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
window.PollEvents();
```

`GLFWwindow* GetNativeWindow() const`
- What it does: Returns data from `Window` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
auto value = window.GetNativeWindow();
```

`std::pair<int, int> GetFramebufferSize() const`
- What it does: Returns data from `Window` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
auto value = window.GetFramebufferSize();
```

`bool WasResized() const`
- What it does: Exposes a focused operation on `Window` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
if (window.WasResized()) {     // React to the returned state here. }
```

`void ResetResizeFlag()`
- What it does: Controls runtime playback or reset behavior exposed by `Window`.
- Notes: This is a public member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
window.ResetResizeFlag();
```

`static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)`
- What it does: Exposes a focused operation on `Window` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Platform/Window.h`. Use it when `Window` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Window itself.
```

## Math

### `Transform2D`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `Transform2D` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Transform2D
Transform2D transform2D;
```

**Highlights**
- Declared in `Engine/Math/Transform2D.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TransformMath2D`

**Summary**
Provides grouped helper functions for the TransformMath2D subsystem so related math or utility calls stay in one discoverable place.

**Example Use Case**
Use `TransformMath2D` when you want stateless helpers instead of creating an object just to perform a calculation.
```cpp
glm::vec2 rotated = TransformMath2D::RotateVector({1.0f, 0.0f}, 90.0f);
```

**Highlights**
- Declared in `Engine/Math/TransformMath2D.h`.
- Type kind: namespace.
- Documented functions: 9.

`glm::vec2 RotateVector(const glm::vec2& value, float degrees)`
- What it does: Exposes a focused operation on `TransformMath2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
TransformMath2D.RotateVector({0.0f, 0.0f}, 1.0f);
```

`glm::vec2 SafeDivide(const glm::vec2& value, const glm::vec2& divisor)`
- What it does: Exposes a focused operation on `TransformMath2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
TransformMath2D.SafeDivide({0.0f, 0.0f}, {0.0f, 0.0f});
```

`glm::vec2 GetPivotLocalPosition(const Transform2D& transform, const glm::vec2& size)`
- What it does: Returns data from `TransformMath2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
auto value = TransformMath2D.GetPivotLocalPosition({}, {0.0f, 0.0f});
```

`glm::vec2 GetPivotWorldPosition(const Transform2D& transform, const glm::vec2& size)`
- What it does: Returns data from `TransformMath2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
auto value = TransformMath2D.GetPivotWorldPosition({}, {0.0f, 0.0f});
```

`glm::vec2 WorldToLocalRectPoint(const glm::vec2& worldPoint, const Transform2D& transform, const glm::vec2& size)`
- What it does: Exposes a focused operation on `TransformMath2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
TransformMath2D.WorldToLocalRectPoint({0.0f, 0.0f}, {}, {0.0f, 0.0f});
```

`bool ContainsWorldPoint(const glm::vec2& worldPoint, const Transform2D& transform, const glm::vec2& size)`
- What it does: Exposes a focused operation on `TransformMath2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
TransformMath2D.ContainsWorldPoint({0.0f, 0.0f}, {}, {0.0f, 0.0f});
```

`std::array<glm::vec2, 4> GetWorldCorners(const Transform2D& transform, const glm::vec2& size)`
- What it does: Returns data from `TransformMath2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
auto value = TransformMath2D.GetWorldCorners({}, {0.0f, 0.0f});
```

`Transform2D CombineTransforms( const Transform2D& parentWorld, const glm::vec2& parentSize, const Transform2D& childLocal, const glm::vec2& childSize )`
- What it does: Exposes a focused operation on `TransformMath2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
TransformMath2D.CombineTransforms({}, {0.0f, 0.0f}, {}, {0.0f, 0.0f});
```

`Transform2D ToLocalTransform( const Transform2D& worldTransform, const glm::vec2& worldSize, const Transform2D& parentWorldTransform, const glm::vec2& parentSize )`
- What it does: Exposes a focused operation on `TransformMath2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Math/TransformMath2D.h`. Use it when `TransformMath2D` is the right owner for this operation.
- Example use:
```cpp
TransformMath2D.ToLocalTransform({}, {0.0f, 0.0f}, {}, {0.0f, 0.0f});
```

## Reflection

### `Property`

**Summary**
Supplies reflection and serialization metadata so editor and save systems can discover values automatically.

**Example Use Case**
Use `Property` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Property
Property property;
```

**Highlights**
- Declared in `Engine/Reflection/Property.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `PropertyRegistry`

**Summary**
Supplies reflection and serialization metadata so editor and save systems can discover values automatically.

**Example Use Case**
Use `PropertyRegistry` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for PropertyRegistry
PropertyRegistry propertyRegistry;
```

**Highlights**
- Declared in `Engine/Reflection/Property.h`.
- Type kind: struct.
- Documented functions: 1.

`static constexpr std::span<const Property> Get()`
- What it does: Returns data from `PropertyRegistry` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Reflection/Property.h`. Use it when `PropertyRegistry` is the right owner for this operation.
- Example use:
```cpp
auto value = PropertyRegistry.Get();
```

### `PropertySerialization`

**Summary**
Supplies reflection and serialization metadata so editor and save systems can discover values automatically.

**Example Use Case**
Use `PropertySerialization` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for PropertySerialization
PropertySerialization propertySerialization;
```

**Highlights**
- Declared in `Engine/Reflection/PropertySerialization.h`.
- Type kind: header.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `PropertyTypeTraits`

**Summary**
Supplies reflection and serialization metadata so editor and save systems can discover values automatically.

**Example Use Case**
Use `PropertyTypeTraits` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for PropertyTypeTraits
PropertyTypeTraits propertyTypeTraits;
```

**Highlights**
- Declared in `Engine/Reflection/PropertyTypeTraits.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

## Component System

### `EntityComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `EntityComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for EntityComponent
EntityComponent entityComponent;
```

**Highlights**
- Declared in `Engine/Component/EntityComponent.h`.
- Type kind: class.
- Documented functions: 3.

`virtual ~EntityComponent()`
- What it does: Exposes a focused operation on `EntityComponent` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Component/EntityComponent.h`. Use it when `EntityComponent` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`virtual ComponentTypeID GetID() const = 0`
- What it does: Returns data from `EntityComponent` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/EntityComponent.h`. Use it when `EntityComponent` is the right owner for this operation.
- Example use:
```cpp
auto value = entityComponent.GetID();
```

`virtual const char* GetTypeName() const = 0`
- What it does: Returns data from `EntityComponent` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/EntityComponent.h`. Use it when `EntityComponent` is the right owner for this operation.
- Example use:
```cpp
auto value = entityComponent.GetTypeName();
```

### `DebugSettingsComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `DebugSettingsComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for DebugSettingsComponent
DebugSettingsComponent debugSettingsComponent;
```

**Highlights**
- Declared in `Engine/Component/Gameplay/DebugSettingsComponent.h`.
- Type kind: class.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `PlayerMovementComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `PlayerMovementComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for PlayerMovementComponent
PlayerMovementComponent playerMovementComponent;
```

**Highlights**
- Declared in `Engine/Component/Gameplay/PlayerMovementComponent.h`.
- Type kind: class.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `SceneComponent`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for SceneComponent
SceneComponent sceneComponent;
```

**Highlights**
- Declared in `Engine/Component/SceneComponent.h`.
- Type kind: class.
- Documented functions: 9.

`~SceneComponent() override`
- What it does: Exposes a focused operation on `SceneComponent` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`virtual void Ensure(Entity& entity) const = 0`
- What it does: Exposes a focused operation on `SceneComponent` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
sceneComponent.Ensure(entity);
```

`virtual void Remove(Entity& entity) const = 0`
- What it does: Removes, clears, or tears down data owned by `SceneComponent`.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
sceneComponent.Remove(entity);
```

`virtual bool Has(const Entity& entity) const = 0`
- What it does: Answers a yes or no question about the current state of `SceneComponent`.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
if (sceneComponent.Has(entity)) {     // React to the returned state here. }
```

`virtual void Serialize(const Entity& entity, nlohmann::json& out) const = 0`
- What it does: Moves data into or out of `SceneComponent` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
sceneComponent.Serialize(entity, /* ... */);
```

`virtual void Deserialize(Entity& entity, const nlohmann::json& in) const = 0`
- What it does: Moves data into or out of `SceneComponent` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
sceneComponent.Deserialize(entity, {});
```

`virtual std::span<const Property> GetProperties() const = 0`
- What it does: Returns data from `SceneComponent` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
auto value = sceneComponent.GetProperties();
```

`virtual void* GetMutableData(Entity& entity) const = 0`
- What it does: Returns data from `SceneComponent` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
auto value = sceneComponent.GetMutableData(entity);
```

`virtual const void* GetData(const Entity& entity) const = 0`
- What it does: Returns data from `SceneComponent` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponent.h`. Use it when `SceneComponent` is the right owner for this operation.
- Example use:
```cpp
auto value = sceneComponent.GetData(entity);
```

### `SceneComponentAutoRegistrar`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneComponentAutoRegistrar` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for SceneComponentAutoRegistrar
SceneComponentAutoRegistrar sceneComponentAutoRegistrar;
```

**Highlights**
- Declared in `Engine/Component/SceneComponentRegistry.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `SceneComponentDetail`

**Summary**
Provides grouped helper functions for the SceneComponentDetail subsystem so related math or utility calls stay in one discoverable place.

**Example Use Case**
Use `SceneComponentDetail` when you want stateless helpers instead of creating an object just to perform a calculation.
```cpp
// Example helper call
auto result = SceneComponentDetail::/* function */;
```

**Highlights**
- Declared in `Engine/Component/SceneComponentRegistry.h`.
- Type kind: namespace.
- Documented functions: 8.

`template<typename TComponent> void Ensure(Entity& entity)`
- What it does: Exposes a focused operation on `SceneComponentDetail` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
SceneComponentDetail.Ensure(entity);
```

`template<typename TComponent> void Remove(Entity& entity)`
- What it does: Removes, clears, or tears down data owned by `SceneComponentDetail`.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
SceneComponentDetail.Remove(entity);
```

`template<typename TComponent> bool Has(const Entity& entity)`
- What it does: Answers a yes or no question about the current state of `SceneComponentDetail`.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
if (SceneComponentDetail.Has(entity)) {     // React to the returned state here. }
```

`template<typename TComponent> void Serialize(const Entity& entity, nlohmann::json& out)`
- What it does: Moves data into or out of `SceneComponentDetail` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
SceneComponentDetail.Serialize(entity, /* ... */);
```

`template<typename TComponent> void Deserialize(Entity& entity, const nlohmann::json& in)`
- What it does: Moves data into or out of `SceneComponentDetail` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
SceneComponentDetail.Deserialize(entity, {});
```

`template<typename TComponent> std::span<const Property> GetProperties()`
- What it does: Returns data from `SceneComponentDetail` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
auto value = SceneComponentDetail.GetProperties();
```

`template<typename TComponent> void* GetMutableData(Entity& entity)`
- What it does: Returns data from `SceneComponentDetail` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
auto value = SceneComponentDetail.GetMutableData(entity);
```

`template<typename TComponent> const void* GetData(const Entity& entity)`
- What it does: Returns data from `SceneComponentDetail` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentDetail` is the right owner for this operation.
- Example use:
```cpp
auto value = SceneComponentDetail.GetData(entity);
```

### `SceneComponentRegistry`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneComponentRegistry` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for SceneComponentRegistry
SceneComponentRegistry sceneComponentRegistry;
```

**Highlights**
- Declared in `Engine/Component/SceneComponentRegistry.h`.
- Type kind: class.
- Documented functions: 4.

`static SceneComponentRegistry& Get()`
- What it does: Returns data from `SceneComponentRegistry` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentRegistry` is the right owner for this operation.
- Example use:
```cpp
auto value = SceneComponentRegistry.Get();
```

`void Register(std::unique_ptr<SceneComponent> component)`
- What it does: Exposes a focused operation on `SceneComponentRegistry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentRegistry` is the right owner for this operation.
- Example use:
```cpp
sceneComponentRegistry.Register(/* ... */);
```

`const SceneComponent* Find(ComponentTypeID id) const`
- What it does: Exposes a focused operation on `SceneComponentRegistry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentRegistry` is the right owner for this operation.
- Example use:
```cpp
sceneComponentRegistry.Find(/* ... */);
```

`std::vector<const SceneComponent*> GetAll() const`
- What it does: Returns data from `SceneComponentRegistry` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Component/SceneComponentRegistry.h`. Use it when `SceneComponentRegistry` is the right owner for this operation.
- Example use:
```cpp
auto value = sceneComponentRegistry.GetAll();
```

## Scene

### `Camera2D`

**Summary**
Stores camera state and exposes the operations needed to move between world space and viewport space.

**Example Use Case**
Use `Camera2D` when you need to pan, zoom, or convert coordinates between screen space and world space.
```cpp
Camera2D camera;
camera.SetTopLeftPosition({0.0f, 0.0f});
camera.SetZoom(1.25f);
```

**Highlights**
- Declared in `Engine/Scene/Camera2D.h`.
- Type kind: class.
- Documented functions: 18.

`const glm::vec2& GetTopLeftPosition() const`
- What it does: Returns data from `Camera2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
auto value = camera2D.GetTopLeftPosition();
```

`float GetZoom() const`
- What it does: Returns data from `Camera2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
auto value = camera2D.GetZoom();
```

`void SetTopLeftPosition(const glm::vec2& position)`
- What it does: Updates engine state through the supported API for `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.SetTopLeftPosition({0.0f, 0.0f});
```

`glm::vec2 GetCenter(float viewWidth, float viewHeight) const`
- What it does: Returns data from `Camera2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
auto value = camera2D.GetCenter(1.0f, 1.0f);
```

`void SetZoom(float zoom)`
- What it does: Updates engine state through the supported API for `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.SetZoom(1.0f);
```

`void Move(const glm::vec2& delta)`
- What it does: Applies movement or spatial resolution logic associated with `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.Move({0.0f, 0.0f});
```

`void Move(float deltaX, float deltaY)`
- What it does: Applies movement or spatial resolution logic associated with `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.Move(1.0f, 1.0f);
```

`void AddZoom(float deltaZoom, float viewportWidth, float viewportHeight)`
- What it does: Creates or attaches something through `Camera2D` and returns or applies the new result.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.AddZoom(1.0f, 1.0f, 1.0f);
```

`void SetMoveSpeed(float speed)`
- What it does: Updates engine state through the supported API for `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.SetMoveSpeed(1.0f);
```

`void SetZoomSpeed(float speed)`
- What it does: Updates engine state through the supported API for `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.SetZoomSpeed(1.0f);
```

`float GetMoveSpeed() const`
- What it does: Returns data from `Camera2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
auto value = camera2D.GetMoveSpeed();
```

`float GetZoomSpeed() const`
- What it does: Returns data from `Camera2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
auto value = camera2D.GetZoomSpeed();
```

`void SetZoomLimits(float minZoom, float maxZoom)`
- What it does: Updates engine state through the supported API for `Camera2D`.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.SetZoomLimits(1.0f, 1.0f);
```

`void Update(float moveX, float moveY, float zoomDelta, float deltaTime, float viewportWidth, float viewportHeight)`
- What it does: Advances or displays `Camera2D` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.Update(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
```

`const glm::mat4& GetViewProjectionMatrix() const`
- What it does: Returns data from `Camera2D` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
auto value = camera2D.GetViewProjectionMatrix();
```

`glm::vec2 ScreenToWorld(const glm::vec2& screenPosition, float viewportWidth, float viewportHeight) const`
- What it does: Exposes a focused operation on `Camera2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.ScreenToWorld({0.0f, 0.0f}, 1.0f, 1.0f);
```

`glm::vec2 WorldToScreen(const glm::vec2& worldPosition, float viewportWidth, float viewportHeight) const`
- What it does: Exposes a focused operation on `Camera2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
camera2D.WorldToScreen({0.0f, 0.0f}, 1.0f, 1.0f);
```

`void RebuildMatrices(float viewportWidth, float viewportHeight)`
- What it does: Exposes a focused operation on `Camera2D` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Camera2D.h`. Use it when `Camera2D` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Camera2D itself.
```

### `Entity`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `Entity` when gameplay code needs to query or mutate a single game object through a convenient handle.
```cpp
Entity entity = scene.CreateEntity("Crate");
entity.SetPosition({128.0f, 64.0f});
entity.EnsureBoxCollider();
```

**Highlights**
- Declared in `Engine/Scene/Entity.h`.
- Type kind: class.
- Documented functions: 125.

`: m_Scene(scene), m_Registry(registry), m_Entity(entity), m_ID(id)`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.m_Scene(scene, /* ... */, entity, /* ... */);
```

`bool IsValid() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsValid()) {     // React to the returned state here. }
```

`explicit operator bool() const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.bool();
```

`GameObjectID GetID() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetID();
```

`std::string GetName() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetName();
```

`void SetName(const std::string& name) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetName("example");
```

`bool IsActive() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsActive()) {     // React to the returned state here. }
```

`void SetActive(bool active) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetActive(true);
```

`void Destroy() const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.Destroy();
```

`bool HasParent() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.HasParent()) {     // React to the returned state here. }
```

`GameObjectID GetParentID() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetParentID();
```

`void SetParent(GameObjectID parentID) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetParent(0);
```

`void ClearParent() const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.ClearParent();
```

`ChildDestroyPolicy GetChildDestroyPolicy() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetChildDestroyPolicy();
```

`void SetChildDestroyPolicy(ChildDestroyPolicy policy) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetChildDestroyPolicy(/* ... */);
```

`Transform2D GetTransform() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTransform();
```

`void SetTransform(const Transform2D& transform) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTransform({});
```

`glm::vec2 GetPosition() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetPosition();
```

`void SetPosition(const glm::vec2& position) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetPosition({0.0f, 0.0f});
```

`float GetRotation() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetRotation();
```

`void SetRotation(float rotationDegrees) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetRotation(1.0f);
```

`glm::vec2 GetScale() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetScale();
```

`void SetScale(const glm::vec2& scale) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetScale({0.0f, 0.0f});
```

`glm::vec2 GetLocalPosition() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetLocalPosition();
```

`void SetLocalPosition(const glm::vec2& position) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetLocalPosition({0.0f, 0.0f});
```

`float GetLocalRotation() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetLocalRotation();
```

`void SetLocalRotation(float rotationDegrees) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetLocalRotation(1.0f);
```

`glm::vec2 GetLocalScale() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetLocalScale();
```

`void SetLocalScale(const glm::vec2& scale) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetLocalScale({0.0f, 0.0f});
```

`Transform2D GetWorldTransform() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetWorldTransform();
```

`void SetWorldTransform(const Transform2D& transform) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetWorldTransform({});
```

`glm::vec2 ResolveMovement(const glm::vec2& delta) const`
- What it does: Applies movement or spatial resolution logic associated with `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.ResolveMovement({0.0f, 0.0f});
```

`bool MoveWithCollision(const glm::vec2& delta) const`
- What it does: Applies movement or spatial resolution logic associated with `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.MoveWithCollision({0.0f, 0.0f});
```

`LocalTransformComponent GetLocalTransform() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetLocalTransform();
```

`void SetLocalTransform(const LocalTransformComponent& transform) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetLocalTransform({});
```

`std::string GetSpriteTexturePath() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetSpriteTexturePath();
```

`IntRect GetSpriteSourceRect() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetSpriteSourceRect();
```

`bool SpriteUsesSourceRect() const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SpriteUsesSourceRect();
```

`glm::vec2 GetSpriteSize() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetSpriteSize();
```

`glm::vec4 GetSpriteTint() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetSpriteTint();
```

`int GetSpriteLayer() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetSpriteLayer();
```

`bool IsSpriteVisible() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsSpriteVisible()) {     // React to the returned state here. }
```

`bool IsSpriteFlippedX() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsSpriteFlippedX()) {     // React to the returned state here. }
```

`bool IsSpriteFlippedY() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsSpriteFlippedY()) {     // React to the returned state here. }
```

`void SetSpriteTexturePath(const std::string& path)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteTexturePath("example");
```

`void SetSpriteSourceRect(int x, int y, int width, int height)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteSourceRect(0, 0, 0, 0);
```

`void SetSpriteSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteSourceRectFromGrid(0, 0, 0, 0);
```

`void SetSpriteSourceRectFromGrid(int column, int row)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteSourceRectFromGrid(0, 0);
```

`void ClearSpriteSourceRect()`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.ClearSpriteSourceRect();
```

`void SetSpriteSize(const glm::vec2& size)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteSize({0.0f, 0.0f});
```

`void SetSpriteTint(const glm::vec4& tint)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteTint({1.0f, 1.0f, 1.0f, 1.0f});
```

`void SetSpriteLayer(int layer)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteLayer(0);
```

`void SetSpriteVisible(bool visible)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteVisible(true);
```

`void SetSpriteFlipX(bool flip)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteFlipX(true);
```

`void SetSpriteFlipY(bool flip)`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetSpriteFlipY(true);
```

`bool HasAnimation() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.HasAnimation()) {     // React to the returned state here. }
```

`void EnsureAnimation() const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.EnsureAnimation();
```

`void RemoveAnimation() const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.RemoveAnimation();
```

`std::string GetAnimationSetPath() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetAnimationSetPath();
```

`std::string GetAnimationClipName() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetAnimationClipName();
```

`void SetAnimationSetPath(const std::string& path) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetAnimationSetPath("example");
```

`void PlayAnimation(const std::string& clipName, bool restartIfSame = false) const`
- What it does: Controls runtime playback or reset behavior exposed by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.PlayAnimation("example", true);
```

`void StopAnimation()`
- What it does: Controls runtime playback or reset behavior exposed by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.StopAnimation();
```

`void ResetAnimation()`
- What it does: Controls runtime playback or reset behavior exposed by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.ResetAnimation();
```

`bool IsAnimationPlaying() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsAnimationPlaying()) {     // React to the returned state here. }
```

`bool HasAnimationFinished() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.HasAnimationFinished()) {     // React to the returned state here. }
```

`bool IsPlayingAnimationClip(const std::string& clipName) const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsPlayingAnimationClip("example")) {     // React to the returned state here. }
```

`bool HasTileMap() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.HasTileMap()) {     // React to the returned state here. }
```

`void EnsureTileMap() const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.EnsureTileMap();
```

`void RemoveTileMap() const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.RemoveTileMap();
```

`uint32_t GetTileMapWidth() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapWidth();
```

`uint32_t GetTileMapHeight() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapHeight();
```

`glm::vec2 GetTileSize() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileSize();
```

`void SetTileSize(const glm::vec2& size) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileSize({0.0f, 0.0f});
```

`glm::ivec2 GetTileAtlasCellSize() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileAtlasCellSize();
```

`void SetTileAtlasCellSize(const glm::ivec2& size) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileAtlasCellSize({0, 0});
```

`std::string GetTileMapTexturePath() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapTexturePath();
```

`void SetTileMapTexturePath(const std::string& path) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileMapTexturePath("example");
```

`std::string GetTileSetAssetPath() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileSetAssetPath();
```

`void SetTileSetAssetPath(const std::string& path) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileSetAssetPath("example");
```

`std::string GetTileMapAssetPath() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapAssetPath();
```

`void SetTileMapAssetPath(const std::string& path) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileMapAssetPath("example");
```

`bool IsTileMapAssetBacked() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsTileMapAssetBacked()) {     // React to the returned state here. }
```

`uint32_t GetTileMapColumns() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapColumns();
```

`uint32_t GetTileMapRows() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapRows();
```

`void SetTileMapGrid(uint32_t columns, uint32_t rows) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileMapGrid(0, 0);
```

`TileMapComponent GetTileMapData() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileMapData();
```

`void SetTileMapData(const TileMapComponent& tileMap) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileMapData({});
```

`void ResizeTileMap(uint32_t width, uint32_t height) const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.ResizeTileMap(0, 0);
```

`int32_t GetTile(int x, int y) const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTile(0, 0);
```

`void SetTile(int x, int y, int32_t tileID) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTile(0, 0, 0);
```

`uint32_t GetTileLayerCount() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileLayerCount();
```

`uint32_t GetActiveTileLayerIndex() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetActiveTileLayerIndex();
```

`void SetActiveTileLayerIndex(uint32_t index) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetActiveTileLayerIndex(0);
```

`std::string GetTileLayerName(uint32_t index) const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTileLayerName(0);
```

`void SetTileLayerName(uint32_t index, const std::string& name) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileLayerName(0, "example");
```

`bool IsTileLayerVisible(uint32_t index) const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsTileLayerVisible(0)) {     // React to the returned state here. }
```

`void SetTileLayerVisible(uint32_t index, bool visible) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileLayerVisible(0, true);
```

`void AddTileLayer(const std::string& name = "Layer") const`
- What it does: Creates or attaches something through `Entity` and returns or applies the new result.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.AddTileLayer("example");
```

`void RemoveTileLayer(uint32_t index) const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.RemoveTileLayer(0);
```

`int32_t GetTile(uint32_t layerIndex, int x, int y) const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTile(0, 0, 0);
```

`void SetTile(uint32_t layerIndex, int x, int y, int32_t tileID) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTile(0, 0, 0, 0);
```

`bool IsTileLayerCollisionEnabled(uint32_t index) const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsTileLayerCollisionEnabled(0)) {     // React to the returned state here. }
```

`void SetTileLayerCollisionEnabled(uint32_t index, bool enabled) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetTileLayerCollisionEnabled(0, true);
```

`bool HasBoxCollider() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.HasBoxCollider()) {     // React to the returned state here. }
```

`void EnsureBoxCollider() const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.EnsureBoxCollider();
```

`void RemoveBoxCollider() const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.RemoveBoxCollider();
```

`glm::vec2 GetBoxColliderSize() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetBoxColliderSize();
```

`void SetBoxColliderSize(const glm::vec2& size) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetBoxColliderSize({0.0f, 0.0f});
```

`glm::vec2 GetBoxColliderOffset() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetBoxColliderOffset();
```

`void SetBoxColliderOffset(const glm::vec2& offset) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetBoxColliderOffset({0.0f, 0.0f});
```

`ColliderBodyType GetColliderBodyType() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetColliderBodyType();
```

`void SetColliderBodyType(ColliderBodyType bodyType) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetColliderBodyType(/* ... */);
```

`bool IsColliderTrigger() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsColliderTrigger()) {     // React to the returned state here. }
```

`void SetColliderTrigger(bool isTrigger) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetColliderTrigger(true);
```

`bool IsBoxColliderEnabled() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.IsBoxColliderEnabled()) {     // React to the returned state here. }
```

`void SetBoxColliderEnabled(bool enabled) const`
- What it does: Updates engine state through the supported API for `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.SetBoxColliderEnabled(true);
```

`template<typename T, typename... Args> T& AddComponent(Args&&... args) const`
- What it does: Creates or attaches something through `Entity` and returns or applies the new result.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.AddComponent(/* ... */);
```

`template<typename T> bool HasComponent() const`
- What it does: Answers a yes or no question about the current state of `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
if (entity.HasComponent()) {     // React to the returned state here. }
```

`template<typename T> T& GetComponent()`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetComponent();
```

`template<typename T> const T& GetComponent() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetComponent();
```

`template<typename T> void RemoveComponent() const`
- What it does: Removes, clears, or tears down data owned by `Entity`.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
entity.RemoveComponent();
```

`const std::vector<ComponentTypeID>& GetTrackedComponentIDs() const`
- What it does: Returns data from `Entity` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
auto value = entity.GetTrackedComponentIDs();
```

`void RegisterTrackedComponent(ComponentTypeID componentID) const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Entity itself.
```

`void UnregisterTrackedComponent(ComponentTypeID componentID) const`
- What it does: Exposes a focused operation on `Entity` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Entity.h`. Use it when `Entity` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Entity itself.
```

### `HasStaticSceneComponentID`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `HasStaticSceneComponentID` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for HasStaticSceneComponentID
HasStaticSceneComponentID hasStaticSceneComponentID;
```

**Highlights**
- Declared in `Engine/Scene/Entity.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `AABB2D`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `AABB2D` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for AABB2D
AABB2D aABB2D;
```

**Highlights**
- Declared in `Engine/Scene/Scene.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `CameraCommand`

**Summary**
Stores camera state and exposes the operations needed to move between world space and viewport space.

**Example Use Case**
Use `CameraCommand` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for CameraCommand
CameraCommand cameraCommand;
```

**Highlights**
- Declared in `Engine/Scene/Scene.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `Scene`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `Scene` when you need to create entities, step gameplay state, and render or serialize the current level.
```cpp
Scene scene;
Entity player = scene.CreateEntity("Player");
scene.Update(deltaTime);
scene.Render(renderer);
```

**Highlights**
- Declared in `Engine/Scene/Scene.h`.
- Type kind: class.
- Documented functions: 83.

`Entity CreateEntity(const std::string& name = "Entity", GameObjectID parentID = 0)`
- What it does: Creates or attaches something through `Scene` and returns or applies the new result.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.CreateEntity("example", 0);
```

`Entity GetEntity(GameObjectID id)`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetEntity(0);
```

`Entity GetEntity(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetEntity(0);
```

`template<typename TObject, typename... Args> TObject Place(Args&&... args)`
- What it does: Creates or attaches something through `Scene` and returns or applies the new result.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.Place(/* ... */);
```

`template<typename TObject, typename... Args> TObject Spawn(Args&&... args)`
- What it does: Creates or attaches something through `Scene` and returns or applies the new result.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.Spawn(/* ... */);
```

`bool IsValidGameObject(GameObjectID id) const`
- What it does: Answers a yes or no question about the current state of `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
if (scene.IsValidGameObject(0)) {     // React to the returned state here. }
```

`bool IsValidHandle(entt::entity entity, GameObjectID id) const`
- What it does: Answers a yes or no question about the current state of `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
if (scene.IsValidHandle(entity, 0)) {     // React to the returned state here. }
```

`void DestroyGameObject(GameObjectID id)`
- What it does: Removes, clears, or tears down data owned by `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.DestroyGameObject(0);
```

`size_t GetGameObjectCount() const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetGameObjectCount();
```

`void Update(float deltaTime)`
- What it does: Advances or displays `Scene` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.Update(1.0f);
```

`void Render(IRenderer2D& renderer)`
- What it does: Advances or displays `Scene` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.Render(renderer);
```

`void RenderTileMapOnly(IRenderer2D& renderer, GameObjectID tileMapID)`
- What it does: Advances or displays `Scene` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.RenderTileMapOnly(renderer, 0);
```

`void UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight)`
- What it does: Advances or displays `Scene` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.UpdateCamera({}, 1.0f, 1.0f, 1.0f);
```

`bool SaveToFile(const std::string& path) const`
- What it does: Moves data into or out of `Scene` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SaveToFile("example");
```

`bool LoadFromFile(const std::string& path)`
- What it does: Moves data into or out of `Scene` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.LoadFromFile("example");
```

`bool IsPlaying() const`
- What it does: Answers a yes or no question about the current state of `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
if (scene.IsPlaying()) {     // React to the returned state here. }
```

`void BeginPlay()`
- What it does: Participates in lifecycle flow for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.BeginPlay();
```

`void EndPlay()`
- What it does: Participates in lifecycle flow for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.EndPlay();
```

`std::string GetGameObjectName(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetGameObjectName(0);
```

`bool SetGameObjectName(GameObjectID id, const std::string& name)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetGameObjectName(0, "example");
```

`bool IsGameObjectActive(GameObjectID id) const`
- What it does: Answers a yes or no question about the current state of `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
if (scene.IsGameObjectActive(0)) {     // React to the returned state here. }
```

`bool SetGameObjectActive(GameObjectID id, bool active)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetGameObjectActive(0, true);
```

`std::vector<GameObjectID> GetGameObjectIDs() const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetGameObjectIDs();
```

`std::vector<GameObjectID> GetRootGameObjects() const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetRootGameObjects();
```

`std::vector<GameObjectID> GetChildGameObjects(GameObjectID parentID) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetChildGameObjects(0);
```

`bool HasChildren(GameObjectID parentID) const`
- What it does: Answers a yes or no question about the current state of `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
if (scene.HasChildren(0)) {     // React to the returned state here. }
```

`bool HasParent(GameObjectID id) const`
- What it does: Answers a yes or no question about the current state of `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
if (scene.HasParent(0)) {     // React to the returned state here. }
```

`GameObjectID GetParentID(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetParentID(0);
```

`bool SetParent(GameObjectID childID, GameObjectID parentID)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetParent(0, 0);
```

`bool ClearParent(GameObjectID childID)`
- What it does: Removes, clears, or tears down data owned by `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.ClearParent(0);
```

`bool SetSiblingOrder(GameObjectID id, int order)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetSiblingOrder(0, 0);
```

`ChildDestroyPolicy GetChildDestroyPolicy(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetChildDestroyPolicy(0);
```

`bool SetChildDestroyPolicy(GameObjectID id, ChildDestroyPolicy policy)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetChildDestroyPolicy(0, /* ... */);
```

`Transform2D GetWorldTransform(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetWorldTransform(0);
```

`bool SetWorldTransform(GameObjectID id, const Transform2D& transform)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetWorldTransform(0, {});
```

`LocalTransformComponent GetLocalTransform(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetLocalTransform(0);
```

`bool SetLocalTransform(GameObjectID id, const LocalTransformComponent& transform)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetLocalTransform(0, {});
```

`glm::vec2 GetLocalPosition(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetLocalPosition(0);
```

`glm::vec2 GetLocalScale(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetLocalScale(0);
```

`glm::vec2 GetLocalPivot(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetLocalPivot(0);
```

`float GetLocalRotation(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetLocalRotation(0);
```

`glm::vec2 GetWorldPosition(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetWorldPosition(0);
```

`glm::vec2 GetWorldScale(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetWorldScale(0);
```

`glm::vec2 GetWorldPivot(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetWorldPivot(0);
```

`float GetWorldRotation(GameObjectID id) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetWorldRotation(0);
```

`bool SetLocalPosition(GameObjectID id, const glm::vec2& position)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetLocalPosition(0, {0.0f, 0.0f});
```

`bool SetLocalScale(GameObjectID id, const glm::vec2& scale)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetLocalScale(0, {0.0f, 0.0f});
```

`bool SetLocalPivot(GameObjectID id, const glm::vec2& pivot)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetLocalPivot(0, {0.0f, 0.0f});
```

`bool SetLocalRotation(GameObjectID id, float rotationDegrees)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetLocalRotation(0, 1.0f);
```

`bool SetWorldPosition(GameObjectID id, const glm::vec2& position)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetWorldPosition(0, {0.0f, 0.0f});
```

`bool SetWorldScale(GameObjectID id, const glm::vec2& scale)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetWorldScale(0, {0.0f, 0.0f});
```

`bool SetWorldPivot(GameObjectID id, const glm::vec2& pivot)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetWorldPivot(0, {0.0f, 0.0f});
```

`bool SetWorldRotation(GameObjectID id, float rotationDegrees)`
- What it does: Updates engine state through the supported API for `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.SetWorldRotation(0, 1.0f);
```

`bool OverlapsSolidBox(const glm::vec2& center, const glm::vec2& size, GameObjectID ignoredID = 0) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.OverlapsSolidBox({0.0f, 0.0f}, {0.0f, 0.0f}, 0);
```

`bool OverlapsSolidBox(const AABB2D& box, GameObjectID ignoredID = 0) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.OverlapsSolidBox({}, 0);
```

`AABB2D BuildWorldAABB(GameObjectID id) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.BuildWorldAABB(0);
```

`AABB2D BuildColliderAABB(GameObjectID id) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.BuildColliderAABB(0);
```

`AABB2D BuildColliderAABB(GameObjectID id, const glm::vec2& overridePosition) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.BuildColliderAABB(0, {0.0f, 0.0f});
```

`AABB2D BuildTileAABB(GameObjectID tileMapID, int tileX, int tileY) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.BuildTileAABB(0, 0, 0);
```

`bool Intersects(const AABB2D& a, const AABB2D& b) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.Intersects({}, {});
```

`glm::vec2 ResolveMovement(GameObjectID id, const glm::vec2& currentPosition, const glm::vec2& delta) const`
- What it does: Applies movement or spatial resolution logic associated with `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.ResolveMovement(0, {0.0f, 0.0f}, {0.0f, 0.0f});
```

`bool MoveWithCollision(GameObjectID id, const glm::vec2& delta)`
- What it does: Applies movement or spatial resolution logic associated with `Scene`.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.MoveWithCollision(0, {0.0f, 0.0f});
```

`void RenderCollisionDebug(IRenderer2D& renderer, GameObjectID focusedTileMapID, bool tileMapOnly)`
- What it does: Advances or displays `Scene` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
scene.RenderCollisionDebug(renderer, 0, true);
```

`Camera2D& GetCamera()`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetCamera();
```

`const Camera2D& GetCamera() const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
auto value = scene.GetCamera();
```

`entt::entity CreateEntityInternal(const std::string& name)`
- What it does: Creates or attaches something through `Scene` and returns or applies the new result.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`entt::entity CreateEntityWithID(const std::string& name, GameObjectID id)`
- What it does: Creates or attaches something through `Scene` and returns or applies the new result.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`entt::entity FindEntityByID(GameObjectID id) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void Clear()`
- What it does: Removes, clears, or tears down data owned by `Scene`.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`const SpriteAnimationSet* GetOrLoadAnimationSet(const std::string& path)`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`const TileSetAsset* GetOrLoadTileSetAsset(const std::string& path) const`
- What it does: Returns data from `Scene` without requiring the caller to reach into its internal state directly.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void MarkTransformDirty(GameObjectID id)`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`Transform2D ResolveWorldTransform(entt::entity entity)`
- What it does: Applies movement or spatial resolution logic associated with `Scene`.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void RenderEntity(IRenderer2D& renderer, entt::entity entity)`
- What it does: Advances or displays `Scene` as part of the current frame.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`std::vector<entt::entity> SortForRendering() const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void DestroyPendingGameObjects()`
- What it does: Removes, clears, or tears down data owned by `Scene`.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`bool WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void HandleChildrenOnDestroy(GameObjectID parentID)`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void DestroyGameObjectRecursive(GameObjectID id)`
- What it does: Removes, clears, or tears down data owned by `Scene`.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void ConnectRegistrySignals()`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void OnLocalTransformUpdated(entt::registry& registry, entt::entity entity)`
- What it does: Exposes a focused operation on `Scene` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`void ResolveRequiredComponents(entt::entity entity)`
- What it does: Applies movement or spatial resolution logic associated with `Scene`.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

`template<typename TObject, typename... Args> TObject CreateObject(bool beginPlayIfAlreadyPlaying, Args&&... args)`
- What it does: Creates or attaches something through `Scene` and returns or applies the new result.
- Notes: This is a private member in `Engine/Scene/Scene.h`. Use it when `Scene` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside Scene itself.
```

### `ActiveComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `ActiveComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for ActiveComponent
ActiveComponent activeComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `BoxColliderComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `BoxColliderComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for BoxColliderComponent
BoxColliderComponent boxColliderComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `IDComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `IDComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for IDComponent
IDComponent iDComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `NameComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `NameComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for NameComponent
NameComponent nameComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `ObjectTypeComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `ObjectTypeComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for ObjectTypeComponent
ObjectTypeComponent objectTypeComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `RelationshipComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `RelationshipComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for RelationshipComponent
RelationshipComponent relationshipComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `RequiredComponentsComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `RequiredComponentsComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for RequiredComponentsComponent
RequiredComponentsComponent requiredComponentsComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileMapComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `TileMapComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for TileMapComponent
TileMapComponent tileMapComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileMapComponent::Layer`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `TileMapComponent::Layer` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for TileMapComponent::Layer
TileMapComponent::Layer tileMapComponentLayer;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `WorldTransformComponent`

**Summary**
Represents a component or component-related helper used to attach data and behavior to game objects in a consistent way.

**Example Use Case**
Use `WorldTransformComponent` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for WorldTransformComponent
WorldTransformComponent worldTransformComponent;
```

**Highlights**
- Declared in `Engine/Scene/SceneComponents.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `SceneSerializer`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneSerializer` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for SceneSerializer
SceneSerializer sceneSerializer;
```

**Highlights**
- Declared in `Engine/Scene/SceneSerializer.h`.
- Type kind: class.
- Documented functions: 2.

`static bool SaveToFile(const Scene& scene, const std::string& path)`
- What it does: Moves data into or out of `SceneSerializer` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/SceneSerializer.h`. Use it when `SceneSerializer` is the right owner for this operation.
- Example use:
```cpp
SceneSerializer.SaveToFile(scene, "example");
```

`static bool LoadFromFile(Scene& scene, const std::string& path)`
- What it does: Moves data into or out of `SceneSerializer` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/SceneSerializer.h`. Use it when `SceneSerializer` is the right owner for this operation.
- Example use:
```cpp
SceneSerializer.LoadFromFile(scene, "example");
```

### `AnimationSetRef`

**Summary**
Defines animation data or playback helpers used to drive sprite motion and clip selection.

**Example Use Case**
Use `AnimationSetRef` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for AnimationSetRef
AnimationSetRef animationSetRef;
```

**Highlights**
- Declared in `Engine/Scene/SpriteAnimation.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `SpriteAnimation`

**Summary**
Defines animation data or playback helpers used to drive sprite motion and clip selection.

**Example Use Case**
Use `SpriteAnimation` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for SpriteAnimation
SpriteAnimation spriteAnimation;
```

**Highlights**
- Declared in `Engine/Scene/SpriteAnimation.h`.
- Type kind: class.
- Documented functions: 11.

`void SetAnimationSetPath(const std::string& path)`
- What it does: Updates engine state through the supported API for `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
spriteAnimation.SetAnimationSetPath("example");
```

`void Play(const std::string& clipName, bool restartIfSame = false)`
- What it does: Controls runtime playback or reset behavior exposed by `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
spriteAnimation.Play("example", true);
```

`void Stop()`
- What it does: Controls runtime playback or reset behavior exposed by `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
spriteAnimation.Stop();
```

`void Reset()`
- What it does: Controls runtime playback or reset behavior exposed by `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
spriteAnimation.Reset();
```

`bool IsPlaying() const`
- What it does: Answers a yes or no question about the current state of `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
if (spriteAnimation.IsPlaying()) {     // React to the returned state here. }
```

`bool HasFinished() const`
- What it does: Answers a yes or no question about the current state of `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
if (spriteAnimation.HasFinished()) {     // React to the returned state here. }
```

`bool IsPlayingClip(const std::string& clipName) const`
- What it does: Answers a yes or no question about the current state of `SpriteAnimation`.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
if (spriteAnimation.IsPlayingClip("example")) {     // React to the returned state here. }
```

`const std::string& GetRequestedClipName() const`
- What it does: Returns data from `SpriteAnimation` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteAnimation.GetRequestedClipName();
```

`const AnimationSetRef& GetAnimationSetRef() const`
- What it does: Returns data from `SpriteAnimation` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteAnimation.GetAnimationSetRef();
```

`const AnimationClip* GetCurrentClip() const`
- What it does: Returns data from `SpriteAnimation` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteAnimation.GetCurrentClip();
```

`void Update(float deltaTime, SpriteRenderer& sprite, const SpriteAnimationSet* animationSet)`
- What it does: Advances or displays `SpriteAnimation` as part of the current frame.
- Notes: This is a public member in `Engine/Scene/SpriteAnimation.h`. Use it when `SpriteAnimation` is the right owner for this operation.
- Example use:
```cpp
spriteAnimation.Update(1.0f, /* ... */, /* ... */);
```

### `AnimationClip`

**Summary**
Defines animation data or playback helpers used to drive sprite motion and clip selection.

**Example Use Case**
Use `AnimationClip` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for AnimationClip
AnimationClip animationClip;
```

**Highlights**
- Declared in `Engine/Scene/SpriteAnimationSet.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `SpriteAnimationSet`

**Summary**
Defines animation data or playback helpers used to drive sprite motion and clip selection.

**Example Use Case**
Use `SpriteAnimationSet` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for SpriteAnimationSet
SpriteAnimationSet spriteAnimationSet;
```

**Highlights**
- Declared in `Engine/Scene/SpriteAnimationSet.h`.
- Type kind: class.
- Documented functions: 3.

`bool LoadFromCSV(const std::string& path)`
- What it does: Moves data into or out of `SpriteAnimationSet` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/SpriteAnimationSet.h`. Use it when `SpriteAnimationSet` is the right owner for this operation.
- Example use:
```cpp
spriteAnimationSet.LoadFromCSV("example");
```

`const AnimationClip* FindClip(const std::string& name) const`
- What it does: Exposes a focused operation on `SpriteAnimationSet` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/SpriteAnimationSet.h`. Use it when `SpriteAnimationSet` is the right owner for this operation.
- Example use:
```cpp
spriteAnimationSet.FindClip("example");
```

`const std::vector<AnimationClip>& GetClips() const`
- What it does: Returns data from `SpriteAnimationSet` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteAnimationSet.h`. Use it when `SpriteAnimationSet` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteAnimationSet.GetClips();
```

### `IntRect`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `IntRect` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for IntRect
IntRect intRect;
```

**Highlights**
- Declared in `Engine/Scene/SpriteRenderer.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `SpriteRenderer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `SpriteRenderer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for SpriteRenderer
SpriteRenderer spriteRenderer;
```

**Highlights**
- Declared in `Engine/Scene/SpriteRenderer.h`.
- Type kind: class.
- Documented functions: 21.

`const TextureRef& GetTexture() const`
- What it does: Returns data from `SpriteRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteRenderer.GetTexture();
```

`void SetTexture(const TextureRef& texture)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetTexture({});
```

`void SetTexturePath(const std::string& path)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetTexturePath("example");
```

`const IntRect& GetSourceRect() const`
- What it does: Returns data from `SpriteRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteRenderer.GetSourceRect();
```

`bool UsesSourceRect() const`
- What it does: Exposes a focused operation on `SpriteRenderer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.UsesSourceRect();
```

`void ClearSourceRect()`
- What it does: Removes, clears, or tears down data owned by `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.ClearSourceRect();
```

`void SetSourceRect(int x, int y, int width, int height)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetSourceRect(0, 0, 0, 0);
```

`void SetSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetSourceRectFromGrid(0, 0, 0, 0);
```

`const glm::vec4& GetTint() const`
- What it does: Returns data from `SpriteRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteRenderer.GetTint();
```

`void SetTint(const glm::vec4& tint)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetTint({1.0f, 1.0f, 1.0f, 1.0f});
```

`bool IsVisible() const`
- What it does: Answers a yes or no question about the current state of `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
if (spriteRenderer.IsVisible()) {     // React to the returned state here. }
```

`void SetVisible(bool visible)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetVisible(true);
```

`int GetLayer() const`
- What it does: Returns data from `SpriteRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteRenderer.GetLayer();
```

`void SetLayer(int layer)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetLayer(0);
```

`bool IsFlippedX() const`
- What it does: Answers a yes or no question about the current state of `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
if (spriteRenderer.IsFlippedX()) {     // React to the returned state here. }
```

`bool IsFlippedY() const`
- What it does: Answers a yes or no question about the current state of `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
if (spriteRenderer.IsFlippedY()) {     // React to the returned state here. }
```

`void SetFlipX(bool flip)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetFlipX(true);
```

`void SetFlipY(bool flip)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetFlipY(true);
```

`const glm::vec2& GetSize() const`
- What it does: Returns data from `SpriteRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = spriteRenderer.GetSize();
```

`void SetSize(const glm::vec2& size)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetSize({0.0f, 0.0f});
```

`void SetSize(float width, float height)`
- What it does: Updates engine state through the supported API for `SpriteRenderer`.
- Notes: This is a public member in `Engine/Scene/SpriteRenderer.h`. Use it when `SpriteRenderer` is the right owner for this operation.
- Example use:
```cpp
spriteRenderer.SetSize(1.0f, 1.0f);
```

### `TextureRef`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `TextureRef` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for TextureRef
TextureRef textureRef;
```

**Highlights**
- Declared in `Engine/Scene/SpriteRenderer.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileMapAssetSerializer`

**Summary**
Handles tile-map or tile-set data so levels, tiles, and per-cell state can be edited and rendered consistently.

**Example Use Case**
Use `TileMapAssetSerializer` when you are authoring or consuming tile data, tile assets, or tile editing tools.
```cpp
// Example setup for TileMapAssetSerializer
TileMapAssetSerializer tileMapAssetSerializer;
```

**Highlights**
- Declared in `Engine/Scene/TileMapAssetSerializer.h`.
- Type kind: class.
- Documented functions: 2.

`static bool SaveToFile(Entity entity, const std::string& path)`
- What it does: Moves data into or out of `TileMapAssetSerializer` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/TileMapAssetSerializer.h`. Use it when `TileMapAssetSerializer` is the right owner for this operation.
- Example use:
```cpp
TileMapAssetSerializer.SaveToFile(entity, "example");
```

`static bool LoadIntoEntity(const std::string& path, Entity entity)`
- What it does: Moves data into or out of `TileMapAssetSerializer` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/TileMapAssetSerializer.h`. Use it when `TileMapAssetSerializer` is the right owner for this operation.
- Example use:
```cpp
TileMapAssetSerializer.LoadIntoEntity("example", entity);
```

### `TileDefinition`

**Summary**
Handles tile-map or tile-set data so levels, tiles, and per-cell state can be edited and rendered consistently.

**Example Use Case**
Use `TileDefinition` when you are authoring or consuming tile data, tile assets, or tile editing tools.
```cpp
// Example setup for TileDefinition
TileDefinition tileDefinition;
```

**Highlights**
- Declared in `Engine/Scene/TileSetAsset.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileSetAsset`

**Summary**
Handles tile-map or tile-set data so levels, tiles, and per-cell state can be edited and rendered consistently.

**Example Use Case**
Use `TileSetAsset` when you are authoring or consuming tile data, tile assets, or tile editing tools.
```cpp
// Example setup for TileSetAsset
TileSetAsset tileSetAsset;
```

**Highlights**
- Declared in `Engine/Scene/TileSetAsset.h`.
- Type kind: class.
- Documented functions: 11.

`bool LoadFromFile(const std::string& path)`
- What it does: Moves data into or out of `TileSetAsset` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
tileSetAsset.LoadFromFile("example");
```

`bool SaveToFile(const std::string& path) const`
- What it does: Moves data into or out of `TileSetAsset` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
tileSetAsset.SaveToFile("example");
```

`const std::string& GetTexturePath() const`
- What it does: Returns data from `TileSetAsset` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
auto value = tileSetAsset.GetTexturePath();
```

`void SetTexturePath(const std::string& path)`
- What it does: Updates engine state through the supported API for `TileSetAsset`.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
tileSetAsset.SetTexturePath("example");
```

`uint32_t GetColumns() const`
- What it does: Returns data from `TileSetAsset` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
auto value = tileSetAsset.GetColumns();
```

`uint32_t GetRows() const`
- What it does: Returns data from `TileSetAsset` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
auto value = tileSetAsset.GetRows();
```

`void SetGrid(uint32_t columns, uint32_t rows)`
- What it does: Updates engine state through the supported API for `TileSetAsset`.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
tileSetAsset.SetGrid(0, 0);
```

`bool IsTileSolid(uint32_t tileID) const`
- What it does: Answers a yes or no question about the current state of `TileSetAsset`.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
if (tileSetAsset.IsTileSolid(0)) {     // React to the returned state here. }
```

`void SetTileSolid(uint32_t tileID, bool solid)`
- What it does: Updates engine state through the supported API for `TileSetAsset`.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
tileSetAsset.SetTileSolid(0, true);
```

`uint32_t GetTileCount() const`
- What it does: Returns data from `TileSetAsset` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
auto value = tileSetAsset.GetTileCount();
```

`void ResizeTileDefinitions()`
- What it does: Exposes a focused operation on `TileSetAsset` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Scene/TileSetAsset.h`. Use it when `TileSetAsset` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileSetAsset itself.
```

## Object Layer

### `Actor`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `Actor` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Actor
Actor actor;
```

**Highlights**
- Declared in `Engine/Object/Actor.h`.
- Type kind: class.
- Documented functions: 59.

`~Actor() override`
- What it does: Exposes a focused operation on `Actor` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`bool HasParent() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.HasParent()) {     // React to the returned state here. }
```

`GameObjectID GetParentID() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetParentID();
```

`void SetParent(GameObjectID parentID)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetParent(0);
```

`void ClearParent()`
- What it does: Removes, clears, or tears down data owned by `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.ClearParent();
```

`Transform2D GetTransform() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetTransform();
```

`void SetTransform(const Transform2D& transform)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetTransform({});
```

`glm::vec2 GetPosition() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetPosition();
```

`void SetPosition(const glm::vec2& position)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetPosition({0.0f, 0.0f});
```

`glm::vec2 GetLocalPosition() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetLocalPosition();
```

`void SetLocalPosition(const glm::vec2& position)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetLocalPosition({0.0f, 0.0f});
```

`float GetRotation() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetRotation();
```

`void SetRotation(float rotInDeg)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetRotation(1.0f);
```

`float GetLocalRotation() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetLocalRotation();
```

`void SetLocalRotation(float rotInDeg)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetLocalRotation(1.0f);
```

`glm::vec2 GetScale() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetScale();
```

`void SetScale(const glm::vec2& scale)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetScale({0.0f, 0.0f});
```

`glm::vec2 GetLocalScale() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetLocalScale();
```

`void SetLocalScale(const glm::vec2& scale)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetLocalScale({0.0f, 0.0f});
```

`Transform2D GetWorldTransform() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetWorldTransform();
```

`void SetWorldTransform(const Transform2D& transform)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetWorldTransform({});
```

`Transform2D GetLocalTransform() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetLocalTransform();
```

`void SetLocalTransform(const Transform2D& transform)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetLocalTransform({});
```

`std::string GetSpriteTexturePath() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetSpriteTexturePath();
```

`IntRect GetSpriteSourceRect() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetSpriteSourceRect();
```

`bool SpriteUsesSourceRect() const`
- What it does: Exposes a focused operation on `Actor` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SpriteUsesSourceRect();
```

`glm::vec2 GetSpriteSize() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetSpriteSize();
```

`glm::vec4 GetSpriteTint() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetSpriteTint();
```

`int GetSpriteLayer() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetSpriteLayer();
```

`bool IsSpriteVisible() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.IsSpriteVisible()) {     // React to the returned state here. }
```

`bool IsSpriteFlippedX() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.IsSpriteFlippedX()) {     // React to the returned state here. }
```

`bool IsSpriteFlippedY() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.IsSpriteFlippedY()) {     // React to the returned state here. }
```

`void SetSpriteTexturePath(const std::string& path)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteTexturePath("example");
```

`void SetSpriteSourceRect(int x, int y, int width, int height)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteSourceRect(0, 0, 0, 0);
```

`void SetSpriteSourceRectFromGrid(int x, int y, int width, int height)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteSourceRectFromGrid(0, 0, 0, 0);
```

`void SetSpriteSourceRectFromGrid(int x, int y)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteSourceRectFromGrid(0, 0);
```

`void ClearSpriteSourceRect()`
- What it does: Removes, clears, or tears down data owned by `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.ClearSpriteSourceRect();
```

`void SetSpriteSize(const glm::vec2& size)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteSize({0.0f, 0.0f});
```

`void SetSpriteTint(const glm::vec4& tint)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteTint({1.0f, 1.0f, 1.0f, 1.0f});
```

`void SetSpriteLayer(int layer)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteLayer(0);
```

`void SetSpriteVisible(bool visible)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteVisible(true);
```

`void SetSpriteFlipX(bool flip)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteFlipX(true);
```

`void SetSpriteFlipY(bool flip)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetSpriteFlipY(true);
```

`void ConfigureSprite( const std::string& texturePath, const glm::vec2& size = glm::vec2`
- What it does: Exposes a focused operation on `Actor` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.ConfigureSprite();
```

`bool HasAnimation() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.HasAnimation()) {     // React to the returned state here. }
```

`void EnsureAnimation()`
- What it does: Exposes a focused operation on `Actor` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.EnsureAnimation();
```

`void RemoveAnimation()`
- What it does: Removes, clears, or tears down data owned by `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.RemoveAnimation();
```

`std::string GetAnimationSetPath() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetAnimationSetPath();
```

`std::string GetAnimationClipName() const`
- What it does: Returns data from `Actor` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
auto value = actor.GetAnimationClipName();
```

`void SetAnimationSetPath(const std::string& path)`
- What it does: Updates engine state through the supported API for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.SetAnimationSetPath("example");
```

`void PlayAnimation(const std::string& clipName, bool restartIfSame = false)`
- What it does: Controls runtime playback or reset behavior exposed by `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.PlayAnimation("example", true);
```

`void StopAnimation()`
- What it does: Controls runtime playback or reset behavior exposed by `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.StopAnimation();
```

`void ResetAnimation()`
- What it does: Controls runtime playback or reset behavior exposed by `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.ResetAnimation();
```

`bool IsAnimationPlaying() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.IsAnimationPlaying()) {     // React to the returned state here. }
```

`bool HasAnimationFinished() const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.HasAnimationFinished()) {     // React to the returned state here. }
```

`bool IsPlayingAnimationClip(const std::string& clipName) const`
- What it does: Answers a yes or no question about the current state of `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
if (actor.IsPlayingAnimationClip("example")) {     // React to the returned state here. }
```

`virtual void Initialize() = 0`
- What it does: Participates in lifecycle flow for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.Initialize();
```

`virtual void BeginPlay()`
- What it does: Participates in lifecycle flow for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.BeginPlay();
```

`virtual void EndPlay()`
- What it does: Participates in lifecycle flow for `Actor`.
- Notes: This is a public member in `Engine/Object/Actor.h`. Use it when `Actor` is the right owner for this operation.
- Example use:
```cpp
actor.EndPlay();
```

### `Object`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `Object` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Object
Object object;
```

**Highlights**
- Declared in `Engine/Object/Object.h`.
- Type kind: class.
- Documented functions: 12.

`explicit Object(Entity entity)`
- What it does: Exposes a focused operation on `Object` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
Object object(entity);
```

`virtual ~Object()`
- What it does: Exposes a focused operation on `Object` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`virtual const char* GetTypeName() const = 0`
- What it does: Returns data from `Object` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
auto value = object.GetTypeName();
```

`GameObjectID GetID() const`
- What it does: Returns data from `Object` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
auto value = object.GetID();
```

`bool IsValid() const`
- What it does: Answers a yes or no question about the current state of `Object`.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
if (object.IsValid()) {     // React to the returned state here. }
```

`std::string GetName() const`
- What it does: Returns data from `Object` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
auto value = object.GetName();
```

`void SetName(const std::string& name)`
- What it does: Updates engine state through the supported API for `Object`.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
object.SetName("example");
```

`bool IsActive() const`
- What it does: Answers a yes or no question about the current state of `Object`.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
if (object.IsActive()) {     // React to the returned state here. }
```

`void SetActive(bool active)`
- What it does: Updates engine state through the supported API for `Object`.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
object.SetActive(true);
```

`void Destroy()`
- What it does: Removes, clears, or tears down data owned by `Object`.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
object.Destroy();
```

`Entity& GetEntity()`
- What it does: Returns data from `Object` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
auto value = object.GetEntity();
```

`const Entity& GetEntity() const`
- What it does: Returns data from `Object` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Object.h`. Use it when `Object` is the right owner for this operation.
- Example use:
```cpp
auto value = object.GetEntity();
```

### `ObjectAutoRegistrar`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `ObjectAutoRegistrar` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for ObjectAutoRegistrar
ObjectAutoRegistrar objectAutoRegistrar;
```

**Highlights**
- Declared in `Engine/Object/ObjectRegistry.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `ObjectLifecycleHooks`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `ObjectLifecycleHooks` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for ObjectLifecycleHooks
ObjectLifecycleHooks objectLifecycleHooks;
```

**Highlights**
- Declared in `Engine/Object/ObjectRegistry.h`.
- Type kind: struct.
- Documented functions: 2.

`std::function<void(Entity)> beginPlay`
- What it does: Exposes a focused operation on `ObjectLifecycleHooks` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/ObjectRegistry.h`. Use it when `ObjectLifecycleHooks` is the right owner for this operation.
- Example use:
```cpp
objectLifecycleHooks.function<void(entity);
```

`std::function<void(Entity)> endPlay`
- What it does: Exposes a focused operation on `ObjectLifecycleHooks` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/ObjectRegistry.h`. Use it when `ObjectLifecycleHooks` is the right owner for this operation.
- Example use:
```cpp
objectLifecycleHooks.function<void(entity);
```

### `ObjectRegistry`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `ObjectRegistry` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for ObjectRegistry
ObjectRegistry objectRegistry;
```

**Highlights**
- Declared in `Engine/Object/ObjectRegistry.h`.
- Type kind: class.
- Documented functions: 3.

`static ObjectRegistry& Get()`
- What it does: Returns data from `ObjectRegistry` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/ObjectRegistry.h`. Use it when `ObjectRegistry` is the right owner for this operation.
- Example use:
```cpp
auto value = ObjectRegistry.Get();
```

`void Register(const std::string& typeName, ObjectLifecycleHooks hooks)`
- What it does: Exposes a focused operation on `ObjectRegistry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/ObjectRegistry.h`. Use it when `ObjectRegistry` is the right owner for this operation.
- Example use:
```cpp
objectRegistry.Register("example", /* ... */);
```

`const ObjectLifecycleHooks* Find(const std::string& typeName) const`
- What it does: Exposes a focused operation on `ObjectRegistry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/ObjectRegistry.h`. Use it when `ObjectRegistry` is the right owner for this operation.
- Example use:
```cpp
objectRegistry.Find("example");
```

### `Player`

**Summary**
Acts as a gameplay-facing wrapper around engine state so game code can work with objects through a clear, typed surface.

**Example Use Case**
Use `Player` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Player
Player player;
```

**Highlights**
- Declared in `Engine/Object/Player.h`.
- Type kind: class.
- Documented functions: 5.

`static const char* StaticName()`
- What it does: Exposes a focused operation on `Player` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Object/Player.h`. Use it when `Player` is the right owner for this operation.
- Example use:
```cpp
Player.StaticName();
```

`const char* GetTypeName() const override`
- What it does: Returns data from `Player` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Object/Player.h`. Use it when `Player` is the right owner for this operation.
- Example use:
```cpp
auto value = player.GetTypeName();
```

`void Initialize() override`
- What it does: Participates in lifecycle flow for `Player`.
- Notes: This is a public member in `Engine/Object/Player.h`. Use it when `Player` is the right owner for this operation.
- Example use:
```cpp
player.Initialize();
```

`void BeginPlay() override`
- What it does: Participates in lifecycle flow for `Player`.
- Notes: This is a public member in `Engine/Object/Player.h`. Use it when `Player` is the right owner for this operation.
- Example use:
```cpp
player.BeginPlay();
```

`void Move(const glm::vec2& input, float deltaTime)`
- What it does: Applies movement or spatial resolution logic associated with `Player`.
- Notes: This is a public member in `Engine/Object/Player.h`. Use it when `Player` is the right owner for this operation.
- Example use:
```cpp
player.Move({0.0f, 0.0f}, 1.0f);
```

## Renderer

### `ImGuiLayer`

**Summary**
Provides a focused engine type declared in `Engine/Renderer/ImGuiLayer.h` so this subsystem has a clear place for the behavior it exposes.

**Example Use Case**
Use `ImGuiLayer` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for ImGuiLayer
ImGuiLayer imGuiLayer;
```

**Highlights**
- Declared in `Engine/Renderer/ImGuiLayer.h`.
- Type kind: class.
- Documented functions: 10.

`void Init(GLFWwindow* window, VulkanRenderer& renderer)`
- What it does: Participates in lifecycle flow for `ImGuiLayer`.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.Init(/* ... */, renderer);
```

`void Shutdown()`
- What it does: Participates in lifecycle flow for `ImGuiLayer`.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.Shutdown();
```

`void BeginFrame()`
- What it does: Participates in lifecycle flow for `ImGuiLayer`.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.BeginFrame();
```

`void EndFrame()`
- What it does: Participates in lifecycle flow for `ImGuiLayer`.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.EndFrame();
```

`void Render(VkCommandBuffer commandBuffer)`
- What it does: Advances or displays `ImGuiLayer` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.Render(/* ... */);
```

`bool WantsKeyboardCapture() const`
- What it does: Exposes a focused operation on `ImGuiLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.WantsKeyboardCapture();
```

`bool WantsTextInput() const`
- What it does: Exposes a focused operation on `ImGuiLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.WantsTextInput();
```

`ImTextureID RegisterTexture(VkSampler sampler, VkImageView imageView, VkImageLayout layout)`
- What it does: Exposes a focused operation on `ImGuiLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.RegisterTexture(/* ... */, /* ... */, /* ... */);
```

`void UnregisterTexture(ImTextureID textureID)`
- What it does: Exposes a focused operation on `ImGuiLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
imGuiLayer.UnregisterTexture(/* ... */);
```

`void UploadFontTexture(VulkanRenderer& renderer)`
- What it does: Exposes a focused operation on `ImGuiLayer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/ImGuiLayer.h`. Use it when `ImGuiLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside ImGuiLayer itself.
```

### `IRenderer2D`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `IRenderer2D` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for IRenderer2D
IRenderer2D iRenderer2D;
```

**Highlights**
- Declared in `Engine/Renderer/IRenderer2D.h`.
- Type kind: class.
- Documented functions: 4.

`virtual ~IRenderer2D()`
- What it does: Exposes a focused operation on `IRenderer2D` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/IRenderer2D.h`. Use it when `IRenderer2D` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`virtual void SetCamera(const Camera2D& camera) = 0`
- What it does: Updates engine state through the supported API for `IRenderer2D`.
- Notes: This is a public member in `Engine/Renderer/IRenderer2D.h`. Use it when `IRenderer2D` is the right owner for this operation.
- Example use:
```cpp
iRenderer2D.SetCamera({});
```

`virtual void DrawSprite( const Transform2D& transform, const SpriteRenderer& sprite ) = 0`
- What it does: Advances or displays `IRenderer2D` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/IRenderer2D.h`. Use it when `IRenderer2D` is the right owner for this operation.
- Example use:
```cpp
iRenderer2D.DrawSprite({}, {});
```

`virtual void DrawRectOutline( const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness = 2.0f ) = 0`
- What it does: Advances or displays `IRenderer2D` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/IRenderer2D.h`. Use it when `IRenderer2D` is the right owner for this operation.
- Example use:
```cpp
iRenderer2D.DrawRectOutline({0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f);
```

### `GlobalUBO`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `GlobalUBO` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for GlobalUBO
GlobalUBO globalUBO;
```

**Highlights**
- Declared in `Engine/Renderer/RenderTypes.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `PreparedBatch`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `PreparedBatch` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for PreparedBatch
PreparedBatch preparedBatch;
```

**Highlights**
- Declared in `Engine/Renderer/RenderTypes.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `QuadCommand`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `QuadCommand` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for QuadCommand
QuadCommand quadCommand;
```

**Highlights**
- Declared in `Engine/Renderer/RenderTypes.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `QuadInstanceData`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `QuadInstanceData` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for QuadInstanceData
QuadInstanceData quadInstanceData;
```

**Highlights**
- Declared in `Engine/Renderer/RenderTypes.h`.
- Type kind: struct.
- Documented functions: 2.

`static VkVertexInputBindingDescription GetBindingDescription()`
- What it does: Returns data from `QuadInstanceData` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/RenderTypes.h`. Use it when `QuadInstanceData` is the right owner for this operation.
- Example use:
```cpp
auto value = QuadInstanceData.GetBindingDescription();
```

`static std::array<VkVertexInputAttributeDescription, 8> GetAttributeDescriptions()`
- What it does: Returns data from `QuadInstanceData` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/RenderTypes.h`. Use it when `QuadInstanceData` is the right owner for this operation.
- Example use:
```cpp
auto value = QuadInstanceData.GetAttributeDescriptions();
```

### `VulkanCommandBuffer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanCommandBuffer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanCommandBuffer
VulkanCommandBuffer vulkanCommandBuffer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanCommandBuffer.h`.
- Type kind: class.
- Documented functions: 5.

`void Init(VkDevice device, uint32_t queueFamilyIndex, uint32_t count)`
- What it does: Participates in lifecycle flow for `VulkanCommandBuffer`.
- Notes: This is a public member in `Engine/Renderer/VulkanCommandBuffer.h`. Use it when `VulkanCommandBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanCommandBuffer.Init(/* ... */, 0, 0);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanCommandBuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanCommandBuffer.h`. Use it when `VulkanCommandBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanCommandBuffer.Cleanup(/* ... */);
```

`void Record( VkRenderPass renderPass, const std::vector<VkFramebuffer>& framebuffers, VkExtent2D extent, VkPipeline pipeline)`
- What it does: Exposes a focused operation on `VulkanCommandBuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanCommandBuffer.h`. Use it when `VulkanCommandBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanCommandBuffer.Record(/* ... */, {}, /* ... */, /* ... */);
```

`const std::vector<VkCommandBuffer>& Get() const`
- What it does: Returns data from `VulkanCommandBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanCommandBuffer.h`. Use it when `VulkanCommandBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanCommandBuffer.Get();
```

`VkCommandPool GetCommandPool() const`
- What it does: Returns data from `VulkanCommandBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanCommandBuffer.h`. Use it when `VulkanCommandBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanCommandBuffer.GetCommandPool();
```

### `VulkanContext`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanContext` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanContext
VulkanContext vulkanContext;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanContext.h`.
- Type kind: class.
- Documented functions: 6.

`void Init(GLFWwindow* window)`
- What it does: Participates in lifecycle flow for `VulkanContext`.
- Notes: This is a public member in `Engine/Renderer/VulkanContext.h`. Use it when `VulkanContext` is the right owner for this operation.
- Example use:
```cpp
vulkanContext.Init(/* ... */);
```

`void Cleanup()`
- What it does: Exposes a focused operation on `VulkanContext` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanContext.h`. Use it when `VulkanContext` is the right owner for this operation.
- Example use:
```cpp
vulkanContext.Cleanup();
```

`VkInstance GetInstance() const`
- What it does: Returns data from `VulkanContext` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanContext.h`. Use it when `VulkanContext` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanContext.GetInstance();
```

`VkSurfaceKHR GetSurface() const`
- What it does: Returns data from `VulkanContext` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanContext.h`. Use it when `VulkanContext` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanContext.GetSurface();
```

`void CreateInstance()`
- What it does: Creates or attaches something through `VulkanContext` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanContext.h`. Use it when `VulkanContext` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanContext itself.
```

`void CreateSurface(GLFWwindow* window)`
- What it does: Creates or attaches something through `VulkanContext` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanContext.h`. Use it when `VulkanContext` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanContext itself.
```

### `VulkanDevice`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanDevice` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanDevice
VulkanDevice vulkanDevice;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanDevice.h`.
- Type kind: class.
- Documented functions: 13.

`void Init(VulkanContext& context)`
- What it does: Participates in lifecycle flow for `VulkanDevice`.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
vulkanDevice.Init(/* ... */);
```

`void Cleanup()`
- What it does: Exposes a focused operation on `VulkanDevice` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
vulkanDevice.Cleanup();
```

`VkDevice GetDevice() const`
- What it does: Returns data from `VulkanDevice` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanDevice.GetDevice();
```

`VkPhysicalDevice GetPhysicalDevice() const`
- What it does: Returns data from `VulkanDevice` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanDevice.GetPhysicalDevice();
```

`VkQueue GetGraphicsQueue() const`
- What it does: Returns data from `VulkanDevice` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanDevice.GetGraphicsQueue();
```

`VkQueue GetPresentQueue() const`
- What it does: Returns data from `VulkanDevice` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanDevice.GetPresentQueue();
```

`uint32_t GetGraphicsQueueFamily() const`
- What it does: Returns data from `VulkanDevice` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanDevice.GetGraphicsQueueFamily();
```

`uint32_t GetPresentQueueFamily() const`
- What it does: Returns data from `VulkanDevice` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanDevice.GetPresentQueueFamily();
```

`void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)`
- What it does: Creates or attaches something through `VulkanDevice` and returns or applies the new result.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
vulkanDevice.CreateBuffer(/* ... */, /* ... */, /* ... */, /* ... */, /* ... */);
```

`void CopyBuffer(VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)`
- What it does: Exposes a focused operation on `VulkanDevice` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
vulkanDevice.CopyBuffer(/* ... */, /* ... */, /* ... */, /* ... */, /* ... */);
```

`uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)`
- What it does: Exposes a focused operation on `VulkanDevice` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
vulkanDevice.FindMemoryType(0, /* ... */);
```

`void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)`
- What it does: Exposes a focused operation on `VulkanDevice` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanDevice itself.
```

`void CreateLogicalDevice()`
- What it does: Creates or attaches something through `VulkanDevice` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanDevice.h`. Use it when `VulkanDevice` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanDevice itself.
```

### `VulkanFramebuffer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanFramebuffer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanFramebuffer
VulkanFramebuffer vulkanFramebuffer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanFramebuffer.h`.
- Type kind: class.
- Documented functions: 3.

`void Init( VkDevice device, VkRenderPass renderPass, const std::vector<VkImageView>& imageViews, VkExtent2D extent)`
- What it does: Participates in lifecycle flow for `VulkanFramebuffer`.
- Notes: This is a public member in `Engine/Renderer/VulkanFramebuffer.h`. Use it when `VulkanFramebuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanFramebuffer.Init(/* ... */, /* ... */, {}, /* ... */);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanFramebuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanFramebuffer.h`. Use it when `VulkanFramebuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanFramebuffer.Cleanup(/* ... */);
```

`const std::vector<VkFramebuffer>& Get() const`
- What it does: Returns data from `VulkanFramebuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanFramebuffer.h`. Use it when `VulkanFramebuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanFramebuffer.Get();
```

### `VulkanIndexBuffer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanIndexBuffer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanIndexBuffer
VulkanIndexBuffer vulkanIndexBuffer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanIndexBuffer.h`.
- Type kind: class.
- Documented functions: 5.

`void Init( VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::vector<uint32_t>& indices )`
- What it does: Participates in lifecycle flow for `VulkanIndexBuffer`.
- Notes: This is a public member in `Engine/Renderer/VulkanIndexBuffer.h`. Use it when `VulkanIndexBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanIndexBuffer.Init(/* ... */, /* ... */, /* ... */, 0);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanIndexBuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanIndexBuffer.h`. Use it when `VulkanIndexBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanIndexBuffer.Cleanup(/* ... */);
```

`VkBuffer GetBuffer() const`
- What it does: Returns data from `VulkanIndexBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanIndexBuffer.h`. Use it when `VulkanIndexBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanIndexBuffer.GetBuffer();
```

`uint32_t GetIndexCount() const`
- What it does: Returns data from `VulkanIndexBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanIndexBuffer.h`. Use it when `VulkanIndexBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanIndexBuffer.GetIndexCount();
```

`void CreateIndexBuffer( VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::vector<uint32_t>& indices )`
- What it does: Creates or attaches something through `VulkanIndexBuffer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanIndexBuffer.h`. Use it when `VulkanIndexBuffer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanIndexBuffer itself.
```

### `VulkanPipeline`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanPipeline` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanPipeline
VulkanPipeline vulkanPipeline;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanPipeline.h`.
- Type kind: class.
- Documented functions: 6.

`void Init(VkDevice device, VkExtent2D extent, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout)`
- What it does: Participates in lifecycle flow for `VulkanPipeline`.
- Notes: This is a public member in `Engine/Renderer/VulkanPipeline.h`. Use it when `VulkanPipeline` is the right owner for this operation.
- Example use:
```cpp
vulkanPipeline.Init(/* ... */, /* ... */, /* ... */, /* ... */);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanPipeline` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanPipeline.h`. Use it when `VulkanPipeline` is the right owner for this operation.
- Example use:
```cpp
vulkanPipeline.Cleanup(/* ... */);
```

`VkPipeline Get() const`
- What it does: Returns data from `VulkanPipeline` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanPipeline.h`. Use it when `VulkanPipeline` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanPipeline.Get();
```

`VkPipelineLayout GetLayout() const`
- What it does: Returns data from `VulkanPipeline` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanPipeline.h`. Use it when `VulkanPipeline` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanPipeline.GetLayout();
```

`VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code)`
- What it does: Creates or attaches something through `VulkanPipeline` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanPipeline.h`. Use it when `VulkanPipeline` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanPipeline itself.
```

`std::vector<char> ReadFile(const std::string& filename)`
- What it does: Exposes a focused operation on `VulkanPipeline` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanPipeline.h`. Use it when `VulkanPipeline` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanPipeline itself.
```

### `VulkanQuadInstanceBuffer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanQuadInstanceBuffer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanQuadInstanceBuffer
VulkanQuadInstanceBuffer vulkanQuadInstanceBuffer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanQuadInstanceBuffer.h`.
- Type kind: class.
- Documented functions: 6.

`void Init(VulkanDevice device, size_t maxInstances)`
- What it does: Participates in lifecycle flow for `VulkanQuadInstanceBuffer`.
- Notes: This is a public member in `Engine/Renderer/VulkanQuadInstanceBuffer.h`. Use it when `VulkanQuadInstanceBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanQuadInstanceBuffer.Init(/* ... */, 0);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanQuadInstanceBuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanQuadInstanceBuffer.h`. Use it when `VulkanQuadInstanceBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanQuadInstanceBuffer.Cleanup(/* ... */);
```

`void Update(VulkanDevice device, const std::vector<QuadInstanceData>& instances)`
- What it does: Advances or displays `VulkanQuadInstanceBuffer` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/VulkanQuadInstanceBuffer.h`. Use it when `VulkanQuadInstanceBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanQuadInstanceBuffer.Update(/* ... */, {});
```

`VkBuffer GetBuffer() const`
- What it does: Returns data from `VulkanQuadInstanceBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanQuadInstanceBuffer.h`. Use it when `VulkanQuadInstanceBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanQuadInstanceBuffer.GetBuffer();
```

`uint32_t GetInstanceCount() const`
- What it does: Returns data from `VulkanQuadInstanceBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanQuadInstanceBuffer.h`. Use it when `VulkanQuadInstanceBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanQuadInstanceBuffer.GetInstanceCount();
```

`void CreateBuffer(VulkanDevice device, size_t instanceCapacity)`
- What it does: Creates or attaches something through `VulkanQuadInstanceBuffer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanQuadInstanceBuffer.h`. Use it when `VulkanQuadInstanceBuffer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanQuadInstanceBuffer itself.
```

### `VulkanRenderer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanRenderer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanRenderer
VulkanRenderer vulkanRenderer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanRenderer.h`.
- Type kind: class.
- Documented functions: 52.

`void Init(GLFWwindow* window, int width, int height)`
- What it does: Participates in lifecycle flow for `VulkanRenderer`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.Init(/* ... */, 0, 0);
```

`void Cleanup()`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.Cleanup();
```

`uint32_t LoadTexture(const char* path)`
- What it does: Moves data into or out of `VulkanRenderer` so it can be persisted or restored.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.LoadTexture("example");
```

`void SetCamera(const Camera2D& camera)`
- What it does: Updates engine state through the supported API for `VulkanRenderer`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.SetCamera({});
```

`void DrawSprite(const Transform2D& transform, const SpriteRenderer& sprite) override`
- What it does: Advances or displays `VulkanRenderer` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.DrawSprite({}, {});
```

`void BeginFrame()`
- What it does: Participates in lifecycle flow for `VulkanRenderer`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.BeginFrame();
```

`void EndFrame()`
- What it does: Participates in lifecycle flow for `VulkanRenderer`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.EndFrame();
```

`void DrawFrame()`
- What it does: Advances or displays `VulkanRenderer` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.DrawFrame();
```

`void OnFramebufferResized(int width, int height)`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.OnFramebufferResized(0, 0);
```

`int GetFramebufferWidth() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetFramebufferWidth();
```

`int GetFramebufferHeight() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetFramebufferHeight();
```

`void SetImGuiRenderCallback(std::function<void(VkCommandBuffer)> callback)`
- What it does: Updates engine state through the supported API for `VulkanRenderer`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.SetImGuiRenderCallback(/* ... */);
```

`void DrawRectOutline(const glm::vec2& min, const glm::vec2& max, const glm::vec4& color, float thickness) override`
- What it does: Advances or displays `VulkanRenderer` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.DrawRectOutline({0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f);
```

`uint32_t GetOrLoadTexture(const std::string& path)`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetOrLoadTexture("example");
```

`const VulkanTexture* GetTexture(uint32_t index) const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetTexture(0);
```

`ImTextureID GetOrCreateImGuiTextureID(const std::string& path)`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetOrCreateImGuiTextureID("example");
```

`VkInstance GetInstance() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetInstance();
```

`VkPhysicalDevice GetPhysicalDevice() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetPhysicalDevice();
```

`VkDevice GetDevice() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetDevice();
```

`VkQueue GetGraphicsQueue() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetGraphicsQueue();
```

`uint32_t GetGraphicsQueueFamily() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetGraphicsQueueFamily();
```

`VkRenderPass GetRenderPass() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetRenderPass();
```

`VkCommandPool GetUploadCommandPool() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetUploadCommandPool();
```

`uint32_t GetMinImageCount() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetMinImageCount();
```

`uint32_t GetImageCount() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetImageCount();
```

`void EnsureSceneViewportTarget(uint32_t width, uint32_t height)`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.EnsureSceneViewportTarget(0, 0);
```

`uint32_t GetSceneViewportWidth() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetSceneViewportWidth();
```

`uint32_t GetSceneViewportHeight() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetSceneViewportHeight();
```

`void SetImGuiLayer(ImGuiLayer* imguiLayer)`
- What it does: Updates engine state through the supported API for `VulkanRenderer`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.SetImGuiLayer(/* ... */);
```

`void RefreshSceneViewportTextureHandle()`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderer.RefreshSceneViewportTextureHandle();
```

`ImTextureID GetSceneViewportTextureID() const`
- What it does: Returns data from `VulkanRenderer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderer.GetSceneViewportTextureID();
```

`void DrawQuad(glm::vec2 position, glm::vec2 size, float rotationDegrees, glm::vec2 pivot, glm::vec2 uvMin, glm::vec2 uvMax, glm::vec4 tint =glm::vec4(1.0f), uint32_t textureIndex = 0)`
- What it does: Advances or displays `VulkanRenderer` as part of the current frame.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void RecordSceneViewportPass(VkCommandBuffer commandBuffer)`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void RecordSwapchainUiPass(VkCommandBuffer commandBuffer, uint32_t imageIndex)`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreatePersistentResources(GLFWwindow* window)`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void DestroyPersistentResources()`
- What it does: Removes, clears, or tears down data owned by `VulkanRenderer`.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreateGlobalResources(int width, int height)`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void DestroyGlobalResources()`
- What it does: Removes, clears, or tears down data owned by `VulkanRenderer`.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreateDescriptorSetLayout()`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreateDescriptorPool()`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreateDescriptorSets()`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void RebuildTextureDescriptorResources()`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreateFallbackTexture()`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void UpdateCameraMatrices()`
- What it does: Advances or displays `VulkanRenderer` as part of the current frame.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreateSwapchainResources(int width, int height)`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void DestroySwapchainResources()`
- What it does: Removes, clears, or tears down data owned by `VulkanRenderer`.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void RecreateSwapchain()`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void RecreateSwapchain(int width, int height)`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void CreatePerImageSyncObjects()`
- What it does: Creates or attaches something through `VulkanRenderer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`void DestroyPerImageSyncObjects()`
- What it does: Removes, clears, or tears down data owned by `VulkanRenderer`.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

`std::function<void(VkCommandBuffer)> m_ImGuiRenderCallback`
- What it does: Exposes a focused operation on `VulkanRenderer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanRenderer.h`. Use it when `VulkanRenderer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanRenderer itself.
```

### `VulkanRenderPass`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanRenderPass` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanRenderPass
VulkanRenderPass vulkanRenderPass;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanRenderPass.h`.
- Type kind: class.
- Documented functions: 3.

`void Init(VkDevice device, VkFormat swapchainFormat)`
- What it does: Participates in lifecycle flow for `VulkanRenderPass`.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderPass.h`. Use it when `VulkanRenderPass` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderPass.Init(/* ... */, /* ... */);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanRenderPass` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderPass.h`. Use it when `VulkanRenderPass` is the right owner for this operation.
- Example use:
```cpp
vulkanRenderPass.Cleanup(/* ... */);
```

`VkRenderPass Get() const`
- What it does: Returns data from `VulkanRenderPass` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanRenderPass.h`. Use it when `VulkanRenderPass` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanRenderPass.Get();
```

### `VulkanSwapChain`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanSwapChain` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanSwapChain
VulkanSwapChain vulkanSwapChain;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanSwapChain.h`.
- Type kind: class.
- Documented functions: 8.

`void Init(VulkanDevice& device, VkSurfaceKHR surface, int width, int height)`
- What it does: Participates in lifecycle flow for `VulkanSwapChain`.
- Notes: This is a public member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
vulkanSwapChain.Init(/* ... */, /* ... */, 0, 0);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanSwapChain` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
vulkanSwapChain.Cleanup(/* ... */);
```

`const std::vector<VkImageView>& GetImageViews() const`
- What it does: Returns data from `VulkanSwapChain` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanSwapChain.GetImageViews();
```

`VkFormat GetFormat() const`
- What it does: Returns data from `VulkanSwapChain` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanSwapChain.GetFormat();
```

`VkExtent2D GetExtent() const`
- What it does: Returns data from `VulkanSwapChain` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanSwapChain.GetExtent();
```

`VkSwapchainKHR& Get()`
- What it does: Returns data from `VulkanSwapChain` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanSwapChain.Get();
```

`void CreateSwapchain(VulkanDevice& device, VkSurfaceKHR surface, int width, int height)`
- What it does: Creates or attaches something through `VulkanSwapChain` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanSwapChain itself.
```

`void CreateImageViews(VkDevice device)`
- What it does: Creates or attaches something through `VulkanSwapChain` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanSwapChain.h`. Use it when `VulkanSwapChain` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanSwapChain itself.
```

### `VulkanSync`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanSync` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanSync
VulkanSync vulkanSync;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanSync.h`.
- Type kind: class.
- Documented functions: 4.

`void Init(VkDevice device, int framesInFlight)`
- What it does: Participates in lifecycle flow for `VulkanSync`.
- Notes: This is a public member in `Engine/Renderer/VulkanSync.h`. Use it when `VulkanSync` is the right owner for this operation.
- Example use:
```cpp
vulkanSync.Init(/* ... */, 0);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanSync` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanSync.h`. Use it when `VulkanSync` is the right owner for this operation.
- Example use:
```cpp
vulkanSync.Cleanup(/* ... */);
```

`VkSemaphore GetImageAvailable(uint32_t currentFrame) const`
- What it does: Returns data from `VulkanSync` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanSync.h`. Use it when `VulkanSync` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanSync.GetImageAvailable(0);
```

`VkFence& GetInFlightFence(uint32_t currentFrame)`
- What it does: Returns data from `VulkanSync` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanSync.h`. Use it when `VulkanSync` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanSync.GetInFlightFence(0);
```

### `VulkanTexture`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanTexture` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanTexture
VulkanTexture vulkanTexture;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanTexture.h`.
- Type kind: class.
- Documented functions: 15.

`~VulkanTexture()`
- What it does: Exposes a focused operation on `VulkanTexture` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`VulkanTexture& operator=(const VulkanTexture&)`
- What it does: Declares callable behavior exposed by `VulkanTexture`.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// Example unavailable for this declaration.
```

`VulkanTexture& operator=(VulkanTexture&& other) noexcept`
- What it does: Declares callable behavior exposed by `VulkanTexture`.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// Example unavailable for this declaration.
```

`void Init(VulkanDevice& device, VkCommandPool commandPool, VkQueue graphicsQueue, const char* path)`
- What it does: Participates in lifecycle flow for `VulkanTexture`.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
vulkanTexture.Init(/* ... */, /* ... */, /* ... */, "example");
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanTexture` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
vulkanTexture.Cleanup(/* ... */);
```

`VkImageView GetImageView() const`
- What it does: Returns data from `VulkanTexture` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanTexture.GetImageView();
```

`VkSampler GetSampler() const`
- What it does: Returns data from `VulkanTexture` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanTexture.GetSampler();
```

`uint32_t GetWidth() const`
- What it does: Returns data from `VulkanTexture` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanTexture.GetWidth();
```

`uint32_t GetHeight() const`
- What it does: Returns data from `VulkanTexture` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanTexture.GetHeight();
```

`void CreateTextureImage(VulkanDevice& device, VkCommandPool commandPool, VkQueue graphicsQueue, const char* path)`
- What it does: Creates or attaches something through `VulkanTexture` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanTexture itself.
```

`void CreateImage(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)`
- What it does: Creates or attaches something through `VulkanTexture` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanTexture itself.
```

`void TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)`
- What it does: Exposes a focused operation on `VulkanTexture` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanTexture itself.
```

`void CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)`
- What it does: Exposes a focused operation on `VulkanTexture` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanTexture itself.
```

`void CreateImageView(VkDevice device)`
- What it does: Creates or attaches something through `VulkanTexture` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanTexture itself.
```

`void CreateSampler(VulkanDevice& device)`
- What it does: Creates or attaches something through `VulkanTexture` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanTexture.h`. Use it when `VulkanTexture` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanTexture itself.
```

### `VulkanUniformBuffer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanUniformBuffer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanUniformBuffer
VulkanUniformBuffer vulkanUniformBuffer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanUniformBuffer.h`.
- Type kind: class.
- Documented functions: 5.

`void Init(VulkanDevice device, VkDeviceSize size)`
- What it does: Participates in lifecycle flow for `VulkanUniformBuffer`.
- Notes: This is a public member in `Engine/Renderer/VulkanUniformBuffer.h`. Use it when `VulkanUniformBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanUniformBuffer.Init(/* ... */, /* ... */);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanUniformBuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanUniformBuffer.h`. Use it when `VulkanUniformBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanUniformBuffer.Cleanup(/* ... */);
```

`void Update(VkDevice device, const void* data, VkDeviceSize size)`
- What it does: Advances or displays `VulkanUniformBuffer` as part of the current frame.
- Notes: This is a public member in `Engine/Renderer/VulkanUniformBuffer.h`. Use it when `VulkanUniformBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanUniformBuffer.Update(/* ... */, /* ... */, /* ... */);
```

`VkBuffer GetBuffer() const`
- What it does: Returns data from `VulkanUniformBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanUniformBuffer.h`. Use it when `VulkanUniformBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanUniformBuffer.GetBuffer();
```

`VkDeviceSize GetSize() const`
- What it does: Returns data from `VulkanUniformBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanUniformBuffer.h`. Use it when `VulkanUniformBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanUniformBuffer.GetSize();
```

### `Vertex`

**Summary**
Carries related engine data in a compact type so other systems can pass, store, or serialize it consistently.

**Example Use Case**
Use `Vertex` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for Vertex
Vertex vertex;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanVertexBuffer.h`.
- Type kind: struct.
- Documented functions: 2.

`static VkVertexInputBindingDescription GetVertexInputBindingDescription()`
- What it does: Returns data from `Vertex` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `Vertex` is the right owner for this operation.
- Example use:
```cpp
auto value = Vertex.GetVertexInputBindingDescription();
```

`static std::array<VkVertexInputAttributeDescription, 2> GetVertexInputAttributeDescription()`
- What it does: Returns data from `Vertex` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `Vertex` is the right owner for this operation.
- Example use:
```cpp
auto value = Vertex.GetVertexInputAttributeDescription();
```

### `VulkanVertexBuffer`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanVertexBuffer` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanVertexBuffer
VulkanVertexBuffer vulkanVertexBuffer;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanVertexBuffer.h`.
- Type kind: class.
- Documented functions: 5.

`void Init(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::vector<Vertex>& vertices)`
- What it does: Participates in lifecycle flow for `VulkanVertexBuffer`.
- Notes: This is a public member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `VulkanVertexBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanVertexBuffer.Init(/* ... */, /* ... */, /* ... */, {});
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanVertexBuffer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `VulkanVertexBuffer` is the right owner for this operation.
- Example use:
```cpp
vulkanVertexBuffer.Cleanup(/* ... */);
```

`VkBuffer GetBuffer() const`
- What it does: Returns data from `VulkanVertexBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `VulkanVertexBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanVertexBuffer.GetBuffer();
```

`uint32_t GetVertexCount() const`
- What it does: Returns data from `VulkanVertexBuffer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `VulkanVertexBuffer` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanVertexBuffer.GetVertexCount();
```

`void CreateVertexBuffer(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::vector<Vertex>& vertices)`
- What it does: Creates or attaches something through `VulkanVertexBuffer` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanVertexBuffer.h`. Use it when `VulkanVertexBuffer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanVertexBuffer itself.
```

### `VulkanViewportRenderTarget`

**Summary**
Wraps a renderer-facing system resource so higher-level engine code can perform drawing or GPU setup through a focused API.

**Example Use Case**
Use `VulkanViewportRenderTarget` when renderer code needs direct access to a specific GPU-side resource or rendering stage.
```cpp
// Example setup for VulkanViewportRenderTarget
VulkanViewportRenderTarget vulkanViewportRenderTarget;
```

**Highlights**
- Declared in `Engine/Renderer/VulkanViewportRenderTarget.h`.
- Type kind: class.
- Documented functions: 21.

`void Init(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format)`
- What it does: Participates in lifecycle flow for `VulkanViewportRenderTarget`.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
vulkanViewportRenderTarget.Init(/* ... */, 0, 0, /* ... */);
```

`void Cleanup(VkDevice device)`
- What it does: Exposes a focused operation on `VulkanViewportRenderTarget` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
vulkanViewportRenderTarget.Cleanup(/* ... */);
```

`void Resize(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format)`
- What it does: Exposes a focused operation on `VulkanViewportRenderTarget` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
vulkanViewportRenderTarget.Resize(/* ... */, 0, 0, /* ... */);
```

`void TransitionToShaderRead(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue)`
- What it does: Exposes a focused operation on `VulkanViewportRenderTarget` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
vulkanViewportRenderTarget.TransitionToShaderRead(/* ... */, /* ... */, /* ... */);
```

`void TransitionToColorAttachment(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue)`
- What it does: Exposes a focused operation on `VulkanViewportRenderTarget` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
vulkanViewportRenderTarget.TransitionToColorAttachment(/* ... */, /* ... */, /* ... */);
```

`uint32_t GetWidth() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetWidth();
```

`uint32_t GetHeight() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetHeight();
```

`bool IsValid() const`
- What it does: Answers a yes or no question about the current state of `VulkanViewportRenderTarget`.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
if (vulkanViewportRenderTarget.IsValid()) {     // React to the returned state here. }
```

`VkImage GetImage() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetImage();
```

`VkImageView GetImageView() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetImageView();
```

`VkSampler GetSampler() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetSampler();
```

`VkFormat GetFormat() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetFormat();
```

`VkRenderPass GetRenderPass() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetRenderPass();
```

`VkFramebuffer GetFramebuffer() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetFramebuffer();
```

`VkExtent2D GetExtent() const`
- What it does: Returns data from `VulkanViewportRenderTarget` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
auto value = vulkanViewportRenderTarget.GetExtent();
```

`void CreateImage(VulkanDevice& device)`
- What it does: Creates or attaches something through `VulkanViewportRenderTarget` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanViewportRenderTarget itself.
```

`void CreateImageView(VkDevice device)`
- What it does: Creates or attaches something through `VulkanViewportRenderTarget` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanViewportRenderTarget itself.
```

`void CreateSampler(VkDevice device)`
- What it does: Creates or attaches something through `VulkanViewportRenderTarget` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanViewportRenderTarget itself.
```

`void CreateRenderPass(VkDevice device)`
- What it does: Creates or attaches something through `VulkanViewportRenderTarget` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanViewportRenderTarget itself.
```

`void CreateFramebuffer(VkDevice device)`
- What it does: Creates or attaches something through `VulkanViewportRenderTarget` and returns or applies the new result.
- Notes: This is a private member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanViewportRenderTarget itself.
```

`void TransitionLayout( VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage)`
- What it does: Exposes a focused operation on `VulkanViewportRenderTarget` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Renderer/VulkanViewportRenderTarget.h`. Use it when `VulkanViewportRenderTarget` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside VulkanViewportRenderTarget itself.
```

## Editor

### `CoreComponentInspectorEntry`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `CoreComponentInspectorEntry` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for CoreComponentInspectorEntry
CoreComponentInspectorEntry coreComponentInspectorEntry;
```

**Highlights**
- Declared in `Engine/Editor/CoreComponentInspectorRegistry.h`.
- Type kind: struct.
- Documented functions: 4.

`std::function<bool(const Entity&)> hasComponent`
- What it does: Exposes a focused operation on `CoreComponentInspectorEntry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorEntry` is the right owner for this operation.
- Example use:
```cpp
coreComponentInspectorEntry.function<bool(entity);
```

`std::function<void(Entity&)> ensureComponent`
- What it does: Exposes a focused operation on `CoreComponentInspectorEntry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorEntry` is the right owner for this operation.
- Example use:
```cpp
coreComponentInspectorEntry.function<void(entity);
```

`std::function<void(Entity&)> removeComponent`
- What it does: Exposes a focused operation on `CoreComponentInspectorEntry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorEntry` is the right owner for this operation.
- Example use:
```cpp
coreComponentInspectorEntry.function<void(entity);
```

`std::function<void(Entity&, SceneEditorPanel&)> draw`
- What it does: Exposes a focused operation on `CoreComponentInspectorEntry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorEntry` is the right owner for this operation.
- Example use:
```cpp
coreComponentInspectorEntry.function<void(entity, /* ... */);
```

### `CoreComponentInspectorRegistry`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `CoreComponentInspectorRegistry` when an entity or editor feature needs this specific piece of attached state.
```cpp
// Example setup for CoreComponentInspectorRegistry
CoreComponentInspectorRegistry coreComponentInspectorRegistry;
```

**Highlights**
- Declared in `Engine/Editor/CoreComponentInspectorRegistry.h`.
- Type kind: class.
- Documented functions: 3.

`static CoreComponentInspectorRegistry& Get()`
- What it does: Returns data from `CoreComponentInspectorRegistry` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorRegistry` is the right owner for this operation.
- Example use:
```cpp
auto value = CoreComponentInspectorRegistry.Get();
```

`void Register(CoreComponentInspectorEntry entry)`
- What it does: Exposes a focused operation on `CoreComponentInspectorRegistry` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorRegistry` is the right owner for this operation.
- Example use:
```cpp
coreComponentInspectorRegistry.Register(/* ... */);
```

`const std::vector<CoreComponentInspectorEntry>& GetEntries() const`
- What it does: Returns data from `CoreComponentInspectorRegistry` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/CoreComponentInspectorRegistry.h`. Use it when `CoreComponentInspectorRegistry` is the right owner for this operation.
- Example use:
```cpp
auto value = coreComponentInspectorRegistry.GetEntries();
```

### `EditorLayer`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `EditorLayer` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for EditorLayer
EditorLayer editorLayer;
```

**Highlights**
- Declared in `Engine/Editor/EditorLayer.h`.
- Type kind: class.
- Documented functions: 15.

`~EditorLayer()`
- What it does: Exposes a focused operation on `EditorLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// Destructor calls are usually automatic when the object leaves scope.
```

`void Draw(Scene* scene, VulkanRenderer& renderer, ImTextureID sceneViewportTextureID, bool isPlaying)`
- What it does: Advances or displays `EditorLayer` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
editorLayer.Draw(scene, renderer, /* ... */, true);
```

`const SceneViewportState& GetSceneViewportState() const`
- What it does: Returns data from `EditorLayer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
auto value = editorLayer.GetSceneViewportState();
```

`bool ConsumePlayRequest()`
- What it does: Exposes a focused operation on `EditorLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
editorLayer.ConsumePlayRequest();
```

`bool ConsumeStopRequest()`
- What it does: Exposes a focused operation on `EditorLayer` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
editorLayer.ConsumeStopRequest();
```

`GameObjectID GetSelectedObjectID() const`
- What it does: Returns data from `EditorLayer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
auto value = editorLayer.GetSelectedObjectID();
```

`void SetSelectedObjectID(Scene& scene, GameObjectID id)`
- What it does: Updates engine state through the supported API for `EditorLayer`.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
editorLayer.SetSelectedObjectID(scene, 0);
```

`TileMapEditorPanel& GetTileMapEditorPanel()`
- What it does: Returns data from `EditorLayer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
auto value = editorLayer.GetTileMapEditorPanel();
```

`const TileMapEditorPanel& GetTileMapEditorPanel() const`
- What it does: Returns data from `EditorLayer` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
auto value = editorLayer.GetTileMapEditorPanel();
```

`void DrawDockspace()`
- What it does: Advances or displays `EditorLayer` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorLayer itself.
```

`void DrawTopBar(bool isPlaying)`
- What it does: Advances or displays `EditorLayer` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorLayer itself.
```

`void DrawSelectedObjectOverlay(Scene& scene, bool isPlaying)`
- What it does: Advances or displays `EditorLayer` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorLayer itself.
```

`void DrawTileStampPreview(Scene& scene, bool isPlaying)`
- What it does: Advances or displays `EditorLayer` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorLayer itself.
```

`bool TryGetSelectionCorners(Entity entity, std::array<glm::vec2, 4>& outCorners) const`
- What it does: Exposes a focused operation on `EditorLayer` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorLayer itself.
```

`float GetTopBarHeight() const`
- What it does: Returns data from `EditorLayer` without requiring the caller to reach into its internal state directly.
- Notes: This is a private member in `Engine/Editor/EditorLayer.h`. Use it when `EditorLayer` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorLayer itself.
```

### `EditorSceneController`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `EditorSceneController` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for EditorSceneController
EditorSceneController editorSceneController;
```

**Highlights**
- Declared in `Engine/Editor/EditorSceneController.h`.
- Type kind: class.
- Documented functions: 5.

`void SetSceneFilePath(const std::string& path)`
- What it does: Updates engine state through the supported API for `EditorSceneController`.
- Notes: This is a public member in `Engine/Editor/EditorSceneController.h`. Use it when `EditorSceneController` is the right owner for this operation.
- Example use:
```cpp
editorSceneController.SetSceneFilePath("example");
```

`bool Update( Scene& scene, EditorLayer& editorLayer, const InputState& input, const SceneViewportState& viewportState, bool isPlaying, float deltaTime)`
- What it does: Advances or displays `EditorSceneController` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/EditorSceneController.h`. Use it when `EditorSceneController` is the right owner for this operation.
- Example use:
```cpp
editorSceneController.Update(scene, /* ... */, input, viewportState, true, 1.0f);
```

`void Render(Scene& scene, EditorLayer& editorLayer, VulkanRenderer& renderer) const`
- What it does: Advances or displays `EditorSceneController` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/EditorSceneController.h`. Use it when `EditorSceneController` is the right owner for this operation.
- Example use:
```cpp
editorSceneController.Render(scene, /* ... */, renderer);
```

`bool TryGetSelectionShape(Entity entity, Transform2D& outTransform, glm::vec2& outSize) const`
- What it does: Exposes a focused operation on `EditorSceneController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/EditorSceneController.h`. Use it when `EditorSceneController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorSceneController itself.
```

`bool TryPickGameObject( Scene& scene, const InputState& input, const SceneViewportState& viewportState, GameObjectID& outObjectID) const`
- What it does: Exposes a focused operation on `EditorSceneController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/EditorSceneController.h`. Use it when `EditorSceneController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside EditorSceneController itself.
```

### `EditorViewportMath`

**Summary**
Provides grouped helper functions for the EditorViewportMath subsystem so related math or utility calls stay in one discoverable place.

**Example Use Case**
Use `EditorViewportMath` when you want stateless helpers instead of creating an object just to perform a calculation.
```cpp
// Example helper call
auto result = EditorViewportMath::/* function */;
```

**Highlights**
- Declared in `Engine/Editor/EditorViewportMath.h`.
- Type kind: namespace.
- Documented functions: 1.

`glm::vec2 ScreenToWorld( Scene& scene, const glm::vec2& screenPosition, const SceneViewportState& viewportState)`
- What it does: Exposes a focused operation on `EditorViewportMath` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/EditorViewportMath.h`. Use it when `EditorViewportMath` is the right owner for this operation.
- Example use:
```cpp
EditorViewportMath.ScreenToWorld(scene, {0.0f, 0.0f}, viewportState);
```

### `SceneEditorPanel`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneEditorPanel` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for SceneEditorPanel
SceneEditorPanel sceneEditorPanel;
```

**Highlights**
- Declared in `Engine/Editor/SceneEditorPanel.h`.
- Type kind: class.
- Documented functions: 12.

`void Draw(Scene& scene, GameObjectID& selectedObjectID)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
sceneEditorPanel.Draw(scene, 0);
```

`GameObjectID GetSelectedObjectID() const`
- What it does: Returns data from `SceneEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = sceneEditorPanel.GetSelectedObjectID();
```

`void DrawTransformSection(Entity& entity)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
sceneEditorPanel.DrawTransformSection(entity);
```

`void DrawSpriteSection(Entity& entity)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
sceneEditorPanel.DrawSpriteSection(entity);
```

`void DrawAnimationSection(Entity& entity)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
sceneEditorPanel.DrawAnimationSection(entity);
```

`void DrawBoxColliderSection(Entity& entity)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
sceneEditorPanel.DrawBoxColliderSection(entity);
```

`void SelectObject(Scene& scene, GameObjectID& selectedObjectID, GameObjectID id)`
- What it does: Exposes a focused operation on `SceneEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SceneEditorPanel itself.
```

`void SyncInspectorFromSelection(Scene& scene, GameObjectID selectedObjectID)`
- What it does: Exposes a focused operation on `SceneEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SceneEditorPanel itself.
```

`void DrawHierarchyPanel(Scene& scene, GameObjectID& selectedObjectID)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SceneEditorPanel itself.
```

`void DrawHierarchyNode(Scene& scene, GameObjectID& selectedObjectID, GameObjectID id)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SceneEditorPanel itself.
```

`void DrawInspectorPanel(Scene& scene, GameObjectID& selectedObjectID)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SceneEditorPanel itself.
```

`void DrawCoreComponentSections(Entity& entity)`
- What it does: Advances or displays `SceneEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/SceneEditorPanel.h`. Use it when `SceneEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SceneEditorPanel itself.
```

### `SceneViewportPanel`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneViewportPanel` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for SceneViewportPanel
SceneViewportPanel sceneViewportPanel;
```

**Highlights**
- Declared in `Engine/Editor/SceneViewportPanel.h`.
- Type kind: class.
- Documented functions: 2.

`void Draw(ImTextureID textureID)`
- What it does: Advances or displays `SceneViewportPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/SceneViewportPanel.h`. Use it when `SceneViewportPanel` is the right owner for this operation.
- Example use:
```cpp
sceneViewportPanel.Draw(/* ... */);
```

`const SceneViewportState& GetState() const`
- What it does: Returns data from `SceneViewportPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/SceneViewportPanel.h`. Use it when `SceneViewportPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = sceneViewportPanel.GetState();
```

### `SceneViewportState`

**Summary**
Owns or organizes scene state for this part of the engine, including object lifetime, transforms, and editor or runtime coordination.

**Example Use Case**
Use `SceneViewportState` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for SceneViewportState
SceneViewportState sceneViewportState;
```

**Highlights**
- Declared in `Engine/Editor/SceneViewportPanel.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileMapEditorController`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `TileMapEditorController` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for TileMapEditorController
TileMapEditorController tileMapEditorController;
```

**Highlights**
- Declared in `Engine/Editor/TileMapEditorController.h`.
- Type kind: class.
- Documented functions: 11.

`void Update( Scene& scene, TileMapEditorPanel& tileMapPanel, const InputState& input, const SceneViewportState& viewportState, bool allowEditing)`
- What it does: Advances or displays `TileMapEditorController` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
tileMapEditorController.Update(scene, /* ... */, input, viewportState, true);
```

`bool TryGetHoveredTile( Scene& scene, const SceneViewportState& viewportState, const InputState& input, Entity entity, glm::ivec2& outTile) const`
- What it does: Exposes a focused operation on `TileMapEditorController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void BeginTileStroke(Entity entity)`
- What it does: Participates in lifecycle flow for `TileMapEditorController`.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void RecordTileStrokeEdit(Entity entity, uint32_t layerIndex, int tileX, int tileY, int32_t newValue)`
- What it does: Exposes a focused operation on `TileMapEditorController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void ApplyTileStrokeStamp(TileMapEditorPanel& tileMapPanel, Entity entity, glm::ivec2 hoveredTile, bool erase)`
- What it does: Exposes a focused operation on `TileMapEditorController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void ApplyTileStrokeFill(TileMapEditorPanel& tileMapPanel, Entity entity, glm::ivec2 hoveredTile, bool erase)`
- What it does: Exposes a focused operation on `TileMapEditorController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void EndTileStroke()`
- What it does: Participates in lifecycle flow for `TileMapEditorController`.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void UndoTileStroke(Scene& scene)`
- What it does: Exposes a focused operation on `TileMapEditorController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`void RedoTileStroke(Scene& scene)`
- What it does: Exposes a focused operation on `TileMapEditorController` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`bool IsUndoShortcutPressed(const InputState& input) const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorController`.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

`bool IsRedoShortcutPressed(const InputState& input) const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorController`.
- Notes: This is a private member in `Engine/Editor/TileMapEditorController.h`. Use it when `TileMapEditorController` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorController itself.
```

### `TileMapEditorController::TileEditCell`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `TileMapEditorController::TileEditCell` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for TileMapEditorController::TileEditCell
TileMapEditorController::TileEditCell tileMapEditorControllerTileEditCell;
```

**Highlights**
- Declared in `Engine/Editor/TileMapEditorController.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileMapEditorController::TileEditStroke`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `TileMapEditorController::TileEditStroke` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for TileMapEditorController::TileEditStroke
TileMapEditorController::TileEditStroke tileMapEditorControllerTileEditStroke;
```

**Highlights**
- Declared in `Engine/Editor/TileMapEditorController.h`.
- Type kind: struct.
- Documented functions: 0.
- This type is primarily a data holder, marker, or grouping construct in the header.

### `TileMapEditorPanel`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `TileMapEditorPanel` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for TileMapEditorPanel
TileMapEditorPanel tileMapEditorPanel;
```

**Highlights**
- Declared in `Engine/Editor/TileMapEditorPanel.h`.
- Type kind: class.
- Documented functions: 20.

`void Draw(Scene& scene, VulkanRenderer& renderer, GameObjectID selectedObjectID)`
- What it does: Advances or displays `TileMapEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
tileMapEditorPanel.Draw(scene, renderer, 0);
```

`void SyncFromSelection(Scene& scene, GameObjectID selectedObjectID)`
- What it does: Exposes a focused operation on `TileMapEditorPanel` for code that needs this specific behavior.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
tileMapEditorPanel.SyncFromSelection(scene, 0);
```

`bool IsPaintModeEnabled() const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
if (tileMapEditorPanel.IsPaintModeEnabled()) {     // React to the returned state here. }
```

`bool IsEraseModeEnabled() const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
if (tileMapEditorPanel.IsEraseModeEnabled()) {     // React to the returned state here. }
```

`bool IsFillModeEnabled() const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
if (tileMapEditorPanel.IsFillModeEnabled()) {     // React to the returned state here. }
```

`bool IsTileMapViewEnabled() const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
if (tileMapEditorPanel.IsTileMapViewEnabled()) {     // React to the returned state here. }
```

`void SetTileMapViewEnabled(bool enabled)`
- What it does: Updates engine state through the supported API for `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
tileMapEditorPanel.SetTileMapViewEnabled(true);
```

`int GetSelectedTileID() const`
- What it does: Returns data from `TileMapEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = tileMapEditorPanel.GetSelectedTileID();
```

`glm::ivec2 GetSelectedAtlasOrigin() const`
- What it does: Returns data from `TileMapEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = tileMapEditorPanel.GetSelectedAtlasOrigin();
```

`glm::ivec2 GetSelectedAtlasSize() const`
- What it does: Returns data from `TileMapEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = tileMapEditorPanel.GetSelectedAtlasSize();
```

`int GetSelectedTileIDAtOffset(glm::ivec2 offset, int atlasColumns) const`
- What it does: Returns data from `TileMapEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = tileMapEditorPanel.GetSelectedTileIDAtOffset({0, 0}, 0);
```

`GameObjectID GetSelectedObjectID() const`
- What it does: Returns data from `TileMapEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = tileMapEditorPanel.GetSelectedObjectID();
```

`bool HasActiveTileMap(Scene& scene) const`
- What it does: Answers a yes or no question about the current state of `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
if (tileMapEditorPanel.HasActiveTileMap(scene)) {     // React to the returned state here. }
```

`glm::ivec2 GetHoveredTile() const`
- What it does: Returns data from `TileMapEditorPanel` without requiring the caller to reach into its internal state directly.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
auto value = tileMapEditorPanel.GetHoveredTile();
```

`void SetHoveredTile(glm::ivec2 tile)`
- What it does: Updates engine state through the supported API for `TileMapEditorPanel`.
- Notes: This is a public member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
tileMapEditorPanel.SetHoveredTile({0, 0});
```

`void DrawAtlasPicker(Entity& entity, VulkanRenderer& renderer)`
- What it does: Advances or displays `TileMapEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorPanel itself.
```

`void ApplyAtlusCellSize(Entity entity)`
- What it does: Exposes a focused operation on `TileMapEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorPanel itself.
```

`bool ApplyTileSetAsset(Entity entity)`
- What it does: Exposes a focused operation on `TileMapEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorPanel itself.
```

`void DrawLayerControls(Entity entity)`
- What it does: Advances or displays `TileMapEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorPanel itself.
```

`void DrawAssetControls(Scene& scene, Entity entity)`
- What it does: Advances or displays `TileMapEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/TileMapEditorPanel.h`. Use it when `TileMapEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileMapEditorPanel itself.
```

### `TileSetEditorPanel`

**Summary**
Packages editor-facing behavior for inspecting, editing, or interacting with engine data inside the tooling workflow.

**Example Use Case**
Use `TileSetEditorPanel` while building or extending the editor workflow for inspecting and changing engine data.
```cpp
// Example setup for TileSetEditorPanel
TileSetEditorPanel tileSetEditorPanel;
```

**Highlights**
- Declared in `Engine/Editor/TileSetEditorPanel.h`.
- Type kind: class.
- Documented functions: 5.

`void Draw(VulkanRenderer& renderer)`
- What it does: Advances or displays `TileSetEditorPanel` as part of the current frame.
- Notes: This is a public member in `Engine/Editor/TileSetEditorPanel.h`. Use it when `TileSetEditorPanel` is the right owner for this operation.
- Example use:
```cpp
tileSetEditorPanel.Draw(renderer);
```

`void SyncBuffersFromAsset()`
- What it does: Exposes a focused operation on `TileSetEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileSetEditorPanel.h`. Use it when `TileSetEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileSetEditorPanel itself.
```

`void ClampSelectedTile()`
- What it does: Exposes a focused operation on `TileSetEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileSetEditorPanel.h`. Use it when `TileSetEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileSetEditorPanel itself.
```

`void ApplyCellSizeFromTexture()`
- What it does: Exposes a focused operation on `TileSetEditorPanel` for code that needs this specific behavior.
- Notes: This is a private member in `Engine/Editor/TileSetEditorPanel.h`. Use it when `TileSetEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileSetEditorPanel itself.
```

`void DrawAtlasPicker(VulkanRenderer& renderer)`
- What it does: Advances or displays `TileSetEditorPanel` as part of the current frame.
- Notes: This is a private member in `Engine/Editor/TileSetEditorPanel.h`. Use it when `TileSetEditorPanel` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside TileSetEditorPanel itself.
```

## Sandbox

### `SandboxApp`

**Summary**
Provides a focused engine type declared in `Sandbox/SandboxApp.h` so this subsystem has a clear place for the behavior it exposes.

**Example Use Case**
Use `SandboxApp` when this subsystem is the natural owner of the behavior or data you are working with.
```cpp
// Example setup for SandboxApp
SandboxApp sandboxApp;
```

**Highlights**
- Declared in `Sandbox/SandboxApp.h`.
- Type kind: class.
- Documented functions: 9.

`void OnInit() override`
- What it does: Exposes a focused operation on `SandboxApp` for code that needs this specific behavior.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`void OnUpdate(float deltaTime) override`
- What it does: Exposes a focused operation on `SandboxApp` for code that needs this specific behavior.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`void OnRender(VulkanRenderer& renderer) override`
- What it does: Exposes a focused operation on `SandboxApp` for code that needs this specific behavior.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`Scene* GetEditorScene() override`
- What it does: Returns data from `SandboxApp` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`std::string GetPlayModeSnapshotPath() const override`
- What it does: Returns data from `SandboxApp` without requiring the caller to reach into its internal state directly.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`void OnEnterPlayMode() override`
- What it does: Exposes a focused operation on `SandboxApp` for code that needs this specific behavior.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`void OnExitPlayMode() override`
- What it does: Exposes a focused operation on `SandboxApp` for code that needs this specific behavior.
- Notes: This is a protected member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// Call this from SandboxApp or a derived type when extending behavior.
```

`void CreateDefaultScene()`
- What it does: Creates or attaches something through `SandboxApp` and returns or applies the new result.
- Notes: This is a private member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SandboxApp itself.
```

`void RefreshRuntimeHandles()`
- What it does: Exposes a focused operation on `SandboxApp` for code that needs this specific behavior.
- Notes: This is a private member in `Sandbox/SandboxApp.h`. Use it when `SandboxApp` is the right owner for this operation.
- Example use:
```cpp
// This helper is intended to be called from inside SandboxApp itself.
```

## Notes
- This file is generated from the current header surface in the repo.
- Regenerate it with `powershell -ExecutionPolicy Bypass -File Docs/Generate-EngineFunctionReference.ps1`.
- The docs site in `DocsSite/` reads this file as one of its primary sources.
