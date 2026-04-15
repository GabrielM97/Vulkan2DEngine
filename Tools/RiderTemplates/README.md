# Rider Templates

These files are the repo-owned source for our Rider scene component templates.

## Files

- `SceneComponent.h.txt`
  - Minimal user-defined scene component.
- `SceneComponent.WithProperty.h.txt`
  - Same template with one reflected sample property.

## Team Rules

- User-defined scene components should be `class`es.
- User-defined scene components should inherit from `SceneComponent`.
- User-defined scene components should use `REGISTER_SCENE_COMPONENT(...)`.
- Reflected/editor-saved fields should use `PROPERTY_FIELD(...)`.
- Components should declare `REGISTER_PROPERTIES(...)` even if they currently expose no properties.

## How To Use In Rider

1. Open Rider file templates.
2. Create a new header template or paste one of these into an existing template.
3. Use `$NAME$` for the component class name.
4. Fill in `$COMPONENT_ID$` manually so IDs stay explicit and reviewable.

## Notes

- `SceneComponent.h.txt` is the recommended default template.
- `SceneComponent.WithProperty.h.txt` is useful when you know the component will expose reflected fields immediately.
