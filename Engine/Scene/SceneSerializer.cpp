#include "SceneSerializer.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Reflection/PropertySerialization.h"
#include "Scene.h"
#include "Component/SceneComponentRegistry.h"

using json = nlohmann::json;

namespace
{
    json SerializeVec2(const glm::vec2& value)
    {
        return json::array({value.x, value.y});
    }

    json SerializeVec4(const glm::vec4& value)
    {
        return json::array({value.x, value.y, value.z, value.w});
    }

    glm::vec2 DeserializeVec2(const json& value)
    {
        return {
            value.at(0).get<float>(),
            value.at(1).get<float>()
        };
    }

    glm::vec4 DeserializeVec4(const json& value)
    {
        return {
            value.at(0).get<float>(),
            value.at(1).get<float>(),
            value.at(2).get<float>(),
            value.at(3).get<float>()
        };
    }

    json SerializeTransform(const Transform2D& transform)
    {
        return {
            {"position", SerializeVec2(transform.position)},
            {"scale", SerializeVec2(transform.scale)},
            {"pivot", SerializeVec2(transform.pivot)},
            {"rotationDegrees", transform.rotationDegrees}
        };
    }

    Transform2D DeserializeTransform(const json& value)
    {
        Transform2D transform;
        transform.position = DeserializeVec2(value.at("position"));
        transform.scale = DeserializeVec2(value.at("scale"));
        transform.pivot = DeserializeVec2(value.at("pivot"));
        transform.rotationDegrees = value.at("rotationDegrees").get<float>();
        return transform;
    }

    json SerializeSprite(const SpriteComponent& sprite)
    {
        json value = {
            {"texturePath", sprite.GetTexture().path},
            {"usesSourceRect", sprite.UsesSourceRect()},
            {"size", SerializeVec2(sprite.GetSize())},
            {"tint", SerializeVec4(sprite.GetTint())},
            {"layer", sprite.GetLayer()},
            {"visible", sprite.IsVisible()},
            {"flipX", sprite.IsFlippedX()},
            {"flipY", sprite.IsFlippedY()}
        };

        if (sprite.UsesSourceRect())
        {
            const IntRect rect = sprite.GetSourceRect();
            value["sourceRect"] = json::array({rect.x, rect.y, rect.width, rect.height});
        }

        return value;
    }

    void DeserializeSprite(const json& value, SpriteComponent& sprite)
    {
        sprite.SetTexturePath(value.value("texturePath", ""));
        sprite.SetSize(DeserializeVec2(value.at("size")));
        sprite.SetTint(DeserializeVec4(value.at("tint")));
        sprite.SetLayer(value.value("layer", 0));
        sprite.SetVisible(value.value("visible", true));
        sprite.SetFlipX(value.value("flipX", false));
        sprite.SetFlipY(value.value("flipY", false));

        if (value.value("usesSourceRect", false) && value.contains("sourceRect"))
        {
            const json& rect = value.at("sourceRect");
            sprite.SetSourceRect(
                rect.at(0).get<int>(),
                rect.at(1).get<int>(),
                rect.at(2).get<int>(),
                rect.at(3).get<int>()
            );
        }
        else
        {
            sprite.ClearSourceRect();
        }
    }

    std::string SerializeDestroyPolicy(ChildDestroyPolicy policy)
    {
        return policy == ChildDestroyPolicy::DestroyWithParent
            ? "DestroyWithParent"
            : "DetachToRoot";
    }

    ChildDestroyPolicy DeserializeDestroyPolicy(const std::string& value)
    {
        return value == "DestroyWithParent"
            ? ChildDestroyPolicy::DestroyWithParent
            : ChildDestroyPolicy::DetachToRoot;
    }

    json SerializeRequiredComponents(const RequiredComponentsComponent& requiredComponents)
    {
        json value = json::array();

        for (ComponentTypeID componentID : requiredComponents.componentIDs)
            value.push_back(static_cast<uint32_t>(componentID));

        return value;
    }

    std::vector<ComponentTypeID> DeserializeRequiredComponents(const json& value)
    {
        std::vector<ComponentTypeID> requiredComponents;

        if (!value.is_array())
            return requiredComponents;

        requiredComponents.reserve(value.size());
        for (const json& entry : value)
            requiredComponents.push_back(static_cast<ComponentTypeID>(entry.get<uint32_t>()));

        return requiredComponents;
    }
    
    json SerializeTileMap(const TileMapComponent& tileMap)
    {
        json layers = json::array();
        for (const auto& layer : tileMap.layers)
        {
            layers.push_back({
                {"name", layer.name},
                {"visible", layer.visible},
                {"tiles", layer.tiles}
            });
        }

        return {
                {"width", tileMap.width},
                {"height", tileMap.height},
                {"tileSize", SerializeVec2(tileMap.tileSize)},
                {"atlasCellSize", json::array({tileMap.atlasCellSize.x, tileMap.atlasCellSize.y})},
                {"columns", tileMap.columns},
                {"rows", tileMap.rows},
                {"tilesetTexturePath", tileMap.tilesetTexturePath},
                {"assetPath", tileMap.assetPath},
                {"activeLayerIndex", tileMap.activeLayerIndex},
                {"layers", layers}
        };
    }

    void DeserializeTileMap(const json& value, TileMapComponent& tileMap)
    {
        tileMap.width = value.value("width", 0u);
        tileMap.height = value.value("height", 0u);
        tileMap.tileSize = value.contains("tileSize")
            ? DeserializeVec2(value.at("tileSize"))
            : glm::vec2{32.0f, 32.0f};
        if (value.contains("atlasCellSize") && value.at("atlasCellSize").is_array() && value.at("atlasCellSize").size() >= 2)
        {
            const json& atlasCellSize = value.at("atlasCellSize");
            tileMap.atlasCellSize = {
                std::max(1, atlasCellSize.at(0).get<int>()),
                std::max(1, atlasCellSize.at(1).get<int>())
            };
        }
        else
        {
            tileMap.atlasCellSize = {
                std::max(1, static_cast<int>(tileMap.tileSize.x)),
                std::max(1, static_cast<int>(tileMap.tileSize.y))
            };
        }
        tileMap.columns = std::max<uint32_t>(1, value.value("columns", 1u));
        tileMap.rows = std::max<uint32_t>(1, value.value("rows", 1u));
        tileMap.tilesetTexturePath = value.value("tilesetTexturePath", "");
        tileMap.assetPath = value.value("assetPath", "");
        const std::size_t expectedCount =
            static_cast<std::size_t>(tileMap.width) * static_cast<std::size_t>(tileMap.height);

        tileMap.layers.clear();

        if (value.contains("layers") && value.at("layers").is_array())
        {
            for (const json& layerValue : value.at("layers"))
            {
                TileMapComponent::Layer layer;
                layer.name = layerValue.value("name", "Layer");
                layer.visible = layerValue.value("visible", true);
                layer.tiles = layerValue.value("tiles", std::vector<int32_t>{});

                if (layer.tiles.size() != expectedCount)
                    layer.tiles.assign(expectedCount, -1);

                tileMap.layers.push_back(std::move(layer));
            }
        }
        else
        {
            TileMapComponent::Layer layer;
            layer.name = "Layer 0";
            layer.visible = true;
            layer.tiles = value.value("tiles", std::vector<int32_t>{});
            if (layer.tiles.size() != expectedCount)
                layer.tiles.assign(expectedCount, -1);
            tileMap.layers.push_back(std::move(layer));
        }

        if (tileMap.layers.empty())
        {
            TileMapComponent::Layer layer;
            layer.name = "Layer 0";
            layer.visible = true;
            layer.tiles.assign(expectedCount, -1);
            tileMap.layers.push_back(std::move(layer));
        }

        tileMap.activeLayerIndex = std::min<uint32_t>(
            value.value("activeLayerIndex", 0u),
            static_cast<uint32_t>(tileMap.layers.size() - 1)
        );
    }
    
    std::string SerializeColliderBodyType(ColliderBodyType bodyType)
    {
        return bodyType == ColliderBodyType::Dynamic ? "Dynamic" : "Static";
    }

    ColliderBodyType DeserializeColliderBodyType(const std::string& value)
    {
        return value == "Dynamic" ? ColliderBodyType::Dynamic : ColliderBodyType::Static;
    }

    json SerializeBoxCollider(const BoxColliderComponent& collider)
    {
        return {
                {"size", SerializeVec2(collider.size)},
                {"offset", SerializeVec2(collider.offset)},
                {"bodyType", SerializeColliderBodyType(collider.type)},
                {"isTrigger", collider.isTrigger},
                {"enabled", collider.enabled}
        };
    }

    void DeserializeBoxCollider(const json& value, BoxColliderComponent& collider)
    {
        collider.size = value.contains("size")
            ? DeserializeVec2(value.at("size"))
            : glm::vec2{32.0f, 32.0f};
        collider.offset = value.contains("offset")
            ? DeserializeVec2(value.at("offset"))
            : glm::vec2{0.0f, 0.0f};
        collider.type = DeserializeColliderBodyType(value.value("bodyType", "Static"));
        collider.isTrigger = value.value("isTrigger", false);
        collider.enabled = value.value("enabled", true);
    }
}

bool SceneSerializer::SaveToFile(const Scene& scene, const std::string& path)
{
    json root;
    root["version"] = 1;
    root["entities"] = json::array();

    for (GameObjectID id : scene.GetGameObjectIDs())
    {
        const entt::entity entity = scene.FindEntityByID(id);
        if (entity == entt::null)
            continue;

        const auto& relationship = scene.m_Registry.get<RelationshipComponent>(entity);
        const auto& sprite = scene.m_Registry.get<SpriteComponent>(entity);
        const auto& requiredComponents = scene.m_Registry.get<RequiredComponentsComponent>(entity);

        json entityJson = {
            {"id", id},
            {"name", scene.GetGameObjectName(id)},
            {"objectType", scene.m_Registry.get<ObjectTypeComponent>(entity).typeName},
            {"active", scene.IsGameObjectActive(id)},
            {"relationship", {
                {"parentId", relationship.parentID},
                {"siblingOrder", relationship.siblingOrder},
                {"childDestroyPolicy", SerializeDestroyPolicy(relationship.childDestroyPolicy)}
            }},
            {"localTransform", SerializeTransform(scene.GetLocalTransform(id))},
            {"sprite", SerializeSprite(sprite)},
            {"requiredComponents", SerializeRequiredComponents(requiredComponents)}
        };

        if (scene.m_Registry.all_of<SpriteAnimationComponent>(entity))
        {
            const auto& animation = scene.m_Registry.get<SpriteAnimationComponent>(entity);
            entityJson["animation"] = {
                {"animationSetPath", animation.GetAnimationSetRef().path},
                {"clipName", animation.GetRequestedClipName()},
                {"playing", animation.IsPlaying()}
            };
        }
        
        if (scene.m_Registry.all_of<TileMapComponent>(entity))
        {
            const auto& tileMap = scene.m_Registry.get<TileMapComponent>(entity);
            entityJson["tileMap"] = SerializeTileMap(tileMap);
        }
        
        if (scene.m_Registry.all_of<BoxColliderComponent>(entity))
        {
            const auto& collider = scene.m_Registry.get<BoxColliderComponent>(entity);
            entityJson["boxCollider"] = SerializeBoxCollider(collider);
        }

        Entity wrapped = scene.GetEntity(id);
        json componentsJson = json::object();

        for (ComponentTypeID componentID : wrapped.GetTrackedComponentIDs())
        {
            const SceneComponent* component = SceneComponentRegistry::Get().Find(componentID);
            if (component == nullptr)
                continue;

            if (!component->Has(wrapped))
                continue;

            json componentJson;
            component->Serialize(wrapped, componentJson);
            componentsJson[std::to_string(componentID)] = std::move(componentJson);
        }

        entityJson["components"] = std::move(componentsJson);
        root["entities"].push_back(std::move(entityJson));
    }

    const std::filesystem::path filePath(path);
    if (filePath.has_parent_path())
        std::filesystem::create_directories(filePath.parent_path());

    std::ofstream out(filePath);
    if (!out.is_open())
        return false;

    out << root.dump(4);
    return true;
}

bool SceneSerializer::LoadFromFile(Scene& scene, const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open())
        return false;

    json root;
    in >> root;

    if (!root.contains("version") || root.at("version").get<int>() != 1)
        return false;

    if (!root.contains("entities") || !root.at("entities").is_array())
        return false;

    scene.Clear();

    struct PendingRelationship
    {
        GameObjectID id = 0;
        GameObjectID parentId = 0;
        int siblingOrder = 0;
        ChildDestroyPolicy childDestroyPolicy = ChildDestroyPolicy::DetachToRoot;
    };

    std::vector<PendingRelationship> pendingRelationships;
    GameObjectID maxLoadedID = 0;

    for (const json& entityJson : root.at("entities"))
    {
        const GameObjectID id = entityJson.at("id").get<GameObjectID>();
        const std::string name = entityJson.value("name", "Entity");
        const entt::entity entity = scene.CreateEntityWithID(name, id);

        maxLoadedID = std::max(maxLoadedID, id);

        scene.SetGameObjectActive(id, entityJson.value("active", true));
        scene.SetLocalTransform(id, DeserializeTransform(entityJson.at("localTransform")));
        scene.m_Registry.get<ObjectTypeComponent>(entity).typeName = entityJson.value("objectType", "");

        if (entityJson.contains("requiredComponents"))
        {
            auto& required = scene.m_Registry.get<RequiredComponentsComponent>(entity);
            required.componentIDs = DeserializeRequiredComponents(entityJson.at("requiredComponents"));
        }

        SpriteComponent& sprite = scene.m_Registry.get<SpriteComponent>(entity);
        DeserializeSprite(entityJson.at("sprite"), sprite);
        scene.MarkTransformDirty(id);

        if (entityJson.contains("animation"))
        {
            auto& animation = scene.m_Registry.emplace<SpriteAnimationComponent>(entity);
            const json& animationJson = entityJson.at("animation");

            animation.SetAnimationSetPath(animationJson.value("animationSetPath", ""));
            const std::string clipName = animationJson.value("clipName", "");
            if (!clipName.empty())
            {
                animation.Play(clipName, true);
                if (!animationJson.value("playing", true))
                    animation.Stop();
            }
        }
        
        if (entityJson.contains("tileMap"))
        {
            auto& tileMap = scene.m_Registry.emplace<TileMapComponent>(entity);
            DeserializeTileMap(entityJson.at("tileMap"), tileMap);
        }
        
        if (entityJson.contains("boxCollider"))
        {
            auto& collider = scene.m_Registry.emplace<BoxColliderComponent>(entity);
            DeserializeBoxCollider(entityJson.at("boxCollider"), collider);
        }

        Entity wrapped = scene.GetEntity(id);

        if (entityJson.contains("components"))
        {
            const json& componentsJson = entityJson.at("components");

            for (ComponentTypeID componentID : wrapped.GetTrackedComponentIDs())
            {
                const SceneComponent* component = SceneComponentRegistry::Get().Find(componentID);
                if (component == nullptr)
                    continue;

                const std::string key = std::to_string(componentID);
                if (!componentsJson.contains(key))
                    continue;

                component->Deserialize(wrapped, componentsJson.at(key));
            }
        }

        scene.ResolveRequiredComponents(entity);

        const json& relationshipJson = entityJson.at("relationship");
        pendingRelationships.push_back({
            id,
            relationshipJson.value("parentId", GameObjectID{0}),
            relationshipJson.value("siblingOrder", 0),
            DeserializeDestroyPolicy(relationshipJson.value("childDestroyPolicy", "DetachToRoot"))
        });
    }

    for (const PendingRelationship& pending : pendingRelationships)
    {
        const entt::entity entity = scene.FindEntityByID(pending.id);
        if (entity == entt::null)
            continue;

        auto& relationship = scene.m_Registry.get<RelationshipComponent>(entity);
        relationship.parentID = pending.parentId;
        relationship.siblingOrder = pending.siblingOrder;
        relationship.childDestroyPolicy = pending.childDestroyPolicy;

        scene.MarkTransformDirty(pending.id);
    }

    scene.m_NextGameObjectID = std::max<GameObjectID>(1, maxLoadedID + 1);
    return true;
}
