const systemDescriptions = {
    "Core": "Application flow, play mode transitions, per-frame input capture, and the runtime loop that everything else hangs from.",
    "Platform": "Window creation, framebuffer sizing, and the small GLFW bridge that keeps the renderer and input grounded.",
    "Math": "Transform math, coordinate conversion, reparenting helpers, and geometric utilities used by scene and editor tools.",
    "Reflection": "Property metadata, saveable field registration, and the serialization support behind custom editor-visible components.",
    "Component System": "The custom scene-component layer that sits on top of entities and makes gameplay data discoverable to the editor.",
    "Scene": "Entities, transforms, hierarchy, animations, tile maps, collision, camera control, and JSON scene serialization.",
    "Object Layer": "Typed wrappers like Actor and Player that turn raw entity handles into game-facing classes with lifecycle hooks.",
    "Renderer": "The Vulkan-backed 2D rendering stack, texture and buffer resources, batching, swapchain work, and the scene viewport target.",
    "Editor": "Panels, controllers, picking, inspectors, tile editing, and the interactive tooling that makes the engine usable as a game editor.",
    "Sandbox": "The sample application that demonstrates how the engine is wired together in practice."
};

const state = {
    systems: [],
    selectedSystemIndex: 0,
    selectedClassIndex: 0,
    query: ""
};

document.addEventListener("DOMContentLoaded", () => {
    initialize().catch(showLoadError);
});

async function initialize() {
    const [functionReference, engineOverview] = await Promise.all([
        fetchText("../Docs/EngineFunctionReference.md"),
        fetchText("../Docs/EngineDocumentation.md")
    ]);

    state.systems = parseFunctionReference(functionReference, engineOverview);
    normalizeSelection();
    renderStats();
    renderSystems();
    renderExplorer();
    bindEvents();
}

async function fetchText(path) {
    const response = await fetch(path);
    if (!response.ok) {
        throw new Error(`Failed to load ${path}: ${response.status}`);
    }
    return response.text();
}

function parseFunctionReference(markdown, engineOverview) {
    const lines = markdown.split(/\r?\n/);
    const systems = [];
    let currentSystem = null;
    let currentClass = null;
    let currentFunction = null;
    let activeSection = null;
    let inCodeFence = false;
    let codeLines = [];

    for (const rawLine of lines) {
        const trimmed = rawLine.trim();

        if (trimmed.startsWith("```")) {
            if (inCodeFence) {
                const code = codeLines.join("\n").trim();
                if (currentFunction) {
                    currentFunction.example = code;
                } else if (currentClass) {
                    currentClass.exampleCode = code;
                }

                inCodeFence = false;
                codeLines = [];
            } else {
                inCodeFence = true;
                codeLines = [];
            }
            continue;
        }

        if (inCodeFence) {
            codeLines.push(rawLine);
            continue;
        }

        if (trimmed.startsWith("## ")) {
            const name = trimmed.slice(3).trim();
            currentFunction = null;
            currentClass = null;
            activeSection = null;

            if (name === "Format" || name === "Notes") {
                currentSystem = null;
                continue;
            }

            currentSystem = {
                name,
                description: systemDescriptions[name] || extractOverviewSnippet(engineOverview, name),
                classes: []
            };
            systems.push(currentSystem);
            continue;
        }

        if (trimmed.startsWith("### ")) {
            currentFunction = null;
            activeSection = null;
            if (!currentSystem) {
                continue;
            }

            const header = extractCodeOrText(trimmed.slice(4).trim());
            currentClass = {
                header,
                name: toDisplayName(header),
                details: [],
                functions: [],
                summary: "",
                exampleUseCase: "",
                exampleCode: "",
                highlights: [],
                metadata: {}
            };
            currentSystem.classes.push(currentClass);
            continue;
        }

        if (!trimmed) {
            if (currentFunction && !activeSection) {
                currentFunction = null;
            }
            continue;
        }

        if (!currentClass) {
            continue;
        }

        if (isSectionHeading(trimmed)) {
            activeSection = normalizeSectionLabel(trimmed);
            continue;
        }

        if (isStandaloneSignatureLine(trimmed)) {
            const signature = extractCodeOrText(trimmed);
            if (!looksCallable(signature)) {
                currentClass.details.push({ type: "text", text: signature });
                applyClassTextDetail(currentClass, activeSection, signature);
                currentFunction = null;
                continue;
            }

            currentFunction = {
                signature,
                bullets: [],
                paragraphs: [],
                group: null,
                what: "",
                notes: [],
                example: ""
            };
            currentClass.functions.push(currentFunction);
            activeSection = null;
            continue;
        }

        if (isBulletSignatureLine(trimmed)) {
            const maybeFunction = createFunctionFromBullet(trimmed, currentClass.details);
            if (maybeFunction) {
                currentFunction = maybeFunction;
                currentClass.functions.push(currentFunction);
            } else {
                currentFunction = null;
                const bulletText = trimmed.slice(2).trim();
                currentClass.details.push({ type: "bullet", text: bulletText });
                applyClassBulletDetail(currentClass, activeSection, bulletText);
            }
            continue;
        }

        if (trimmed.startsWith("- ")) {
            const bulletText = trimmed.slice(2).trim();
            if (currentFunction) {
                currentFunction.bullets.push(bulletText);
                applyFunctionBulletDetail(currentFunction, bulletText);
            } else {
                currentClass.details.push({ type: "bullet", text: bulletText });
                applyClassBulletDetail(currentClass, activeSection, bulletText);
            }
            continue;
        }

        if (currentFunction) {
            currentFunction.paragraphs.push(trimmed);
            applyFunctionParagraphDetail(currentFunction, activeSection, trimmed);
        } else {
            currentClass.details.push({ type: "text", text: trimmed });
            applyClassTextDetail(currentClass, activeSection, trimmed);
        }
    }

    systems.forEach(system => {
        system.classes.forEach(classEntry => finalizeClassEntry(classEntry));
    });

    return systems.filter(system => system.classes.length > 0);
}

function isSectionHeading(trimmed) {
    return /^\*\*.+\*\*$/.test(trimmed);
}

function normalizeSectionLabel(trimmed) {
    return trimmed.replace(/^\*\*|\*\*$/g, "").trim().toLowerCase();
}

function applyClassTextDetail(classEntry, section, text) {
    if (!text) {
        return;
    }

    if (section === "summary" && !classEntry.summary) {
        classEntry.summary = text;
        return;
    }

    if (section === "example use case" && !classEntry.exampleUseCase) {
        classEntry.exampleUseCase = text;
    }
}

function applyClassBulletDetail(classEntry, section, text) {
    if (!text) {
        return;
    }

    if (section === "highlights") {
        classEntry.highlights.push(text);
    }
}

function applyFunctionParagraphDetail(fn, section, text) {
    if (!text) {
        return;
    }

    if (section === "what it does" && !fn.what) {
        fn.what = text;
        return;
    }

    if (section === "notes") {
        fn.notes.push(text);
    }
}

function applyFunctionBulletDetail(fn, text) {
    const labeledMatch = text.match(/^([^:]+):\s*(.*)$/);
    if (!labeledMatch) {
        fn.notes.push(text);
        return;
    }

    const label = labeledMatch[1].trim().toLowerCase();
    const value = labeledMatch[2].trim();

    if (label === "what it does") {
        fn.what = value;
        return;
    }

    if (label === "notes") {
        if (value) {
            fn.notes.push(value);
        }
        return;
    }

    if (label === "example use") {
        return;
    }

    fn.notes.push(text);
}

function finalizeClassEntry(classEntry) {
    const meta = {
        declaredIn: "",
        kind: "",
        documentedFunctions: classEntry.functions.length
    };

    classEntry.highlights = classEntry.highlights
        .map(text => {
            const declaredMatch = text.match(/^Declared in `(.+)`\.$/);
            if (declaredMatch) {
                meta.declaredIn = declaredMatch[1];
                return null;
            }

            const kindMatch = text.match(/^Type kind:\s*(.+)\.$/i);
            if (kindMatch) {
                meta.kind = kindMatch[1];
                return null;
            }

            const countMatch = text.match(/^Documented functions:\s*(\d+)\.$/i);
            if (countMatch) {
                meta.documentedFunctions = Number.parseInt(countMatch[1], 10);
                return null;
            }

            return text;
        })
        .filter(Boolean);

    classEntry.metadata = meta;
}

function extractOverviewSnippet(engineOverview, systemName) {
    const lines = engineOverview.split(/\r?\n/);
    const projectStructureIndex = lines.findIndex(line => line.trim() === "## Project Structure");
    if (projectStructureIndex === -1) {
        return "Reference information for this engine area.";
    }

    const relevant = lines.slice(projectStructureIndex + 1, projectStructureIndex + 24);
    const byPrefix = {
        "Core": "Engine/Core",
        "Platform": "Engine/Platform",
        "Renderer": "Engine/Renderer",
        "Scene": "Engine/Scene",
        "Object Layer": "Engine/Object",
        "Component System": "Engine/Component",
        "Reflection": "Engine/Reflection",
        "Editor": "Engine/Editor",
        "Sandbox": "Sandbox"
    };

    const prefix = byPrefix[systemName];
    if (!prefix) {
        return "Reference information for this engine area.";
    }

    const match = relevant.find(line => line.includes(`- \`${prefix}\``));
    if (!match) {
        return "Reference information for this engine area.";
    }

    const parts = match.split(":");
    return parts.length > 1 ? parts.slice(1).join(":").trim() : "Reference information for this engine area.";
}

function extractCodeOrText(value) {
    const match = value.match(/`([^`]+)`/);
    return match ? match[1] : value.replace(/^#+\s*/, "").trim();
}

function looksCallable(signature) {
    return /\(/.test(signature);
}

function isStandaloneSignatureLine(trimmed) {
    return trimmed.startsWith("`");
}

function isBulletSignatureLine(trimmed) {
    return /^-\s*`/.test(trimmed);
}

function createFunctionFromBullet(trimmed, classDetails) {
    const signature = extractCodeOrText(trimmed);
    if (!looksCallable(signature)) {
        return null;
    }

    const textAfterCodeMatch = trimmed.match(/`\s*:\s*(.+)$/);
    const currentGroup = findCurrentGroupLabel(classDetails);
    const paragraphs = [];

    if (textAfterCodeMatch) {
        paragraphs.push(textAfterCodeMatch[1].trim());
    }

    return {
        signature,
        bullets: [],
        paragraphs,
        group: currentGroup,
        what: "",
        notes: [],
        example: ""
    };
}

function findCurrentGroupLabel(classDetails) {
    for (let index = classDetails.length - 1; index >= 0; index -= 1) {
        const detail = classDetails[index];
        if (detail.type === "text" && detail.text.endsWith(":")) {
            return detail.text.replace(/:$/, "").trim();
        }
    }
    return null;
}

function toDisplayName(header) {
    const lastSegment = header.split("/").pop() || header;
    return lastSegment.replace(/\.h$/i, "");
}

function normalizeSelection() {
    if (!state.systems.length) {
        state.selectedSystemIndex = 0;
        state.selectedClassIndex = 0;
        return;
    }

    state.selectedSystemIndex = Math.min(state.selectedSystemIndex, state.systems.length - 1);
    const classes = getFilteredClasses(state.systems[state.selectedSystemIndex]);
    state.selectedClassIndex = Math.min(state.selectedClassIndex, Math.max(classes.length - 1, 0));
}

function getFilteredClasses(system) {
    if (!system) {
        return [];
    }

    const query = state.query.trim().toLowerCase();
    if (!query) {
        return system.classes;
    }

    return system.classes.filter(classEntry => {
        const haystack = [
            classEntry.name,
            classEntry.header,
            ...classEntry.functions.map(fn => fn.signature)
        ].join(" ").toLowerCase();

        return haystack.includes(query);
    });
}

function renderStats() {
    const classCount = state.systems.reduce((sum, system) => sum + system.classes.length, 0);
    const functionCount = state.systems.reduce((sum, system) =>
        sum + system.classes.reduce((classSum, classEntry) => classSum + classEntry.functions.length, 0), 0);

    document.getElementById("systemCount").textContent = state.systems.length.toString();
    document.getElementById("classCount").textContent = classCount.toString();
    document.getElementById("functionCount").textContent = functionCount.toString();
}

function renderSystems() {
    const grid = document.getElementById("systemsGrid");
    const template = document.getElementById("systemCardTemplate");
    grid.innerHTML = "";

    state.systems.forEach((system, index) => {
        const fragment = template.content.cloneNode(true);
        const button = fragment.querySelector(".system-card");
        const classCount = system.classes.length;
        const functionCount = system.classes.reduce((sum, classEntry) => sum + classEntry.functions.length, 0);

        fragment.querySelector(".system-kicker").textContent = `System ${String(index + 1).padStart(2, "0")}`;
        fragment.querySelector(".system-name").textContent = system.name;
        fragment.querySelector(".system-description").textContent = system.description;

        const stats = fragment.querySelector(".system-stats");
        stats.append(createPill(`${classCount} classes`));
        stats.append(createPill(`${functionCount} functions`));

        if (index === state.selectedSystemIndex) {
            button.classList.add("active");
        }

        button.addEventListener("click", () => {
            state.selectedSystemIndex = index;
            state.selectedClassIndex = 0;
            normalizeSelection();
            renderSystems();
            renderExplorer();
        });

        grid.append(fragment);
    });
}

function renderExplorer() {
    const system = state.systems[state.selectedSystemIndex];
    const classList = getFilteredClasses(system);
    const selectedClass = classList[state.selectedClassIndex] || classList[0] || null;

    renderClassList(system, classList, selectedClass);
    renderClassDetails(system, selectedClass);
}

function renderClassList(system, classList, selectedClass) {
    const selectedSystemName = document.getElementById("selectedSystemName");
    const selectedSystemMeta = document.getElementById("selectedSystemMeta");
    const classListNode = document.getElementById("classList");
    const template = document.getElementById("classItemTemplate");

    if (!system) {
        selectedSystemName.textContent = "Classes";
        selectedSystemMeta.textContent = "0 entries";
        classListNode.innerHTML = `<div class="empty-state">No systems were parsed from the docs.</div>`;
        return;
    }

    selectedSystemName.textContent = system.name;
    selectedSystemMeta.textContent = `${classList.length} types`;
    classListNode.innerHTML = "";

    if (!classList.length) {
        classListNode.innerHTML = `<div class="empty-state">No classes matched the current search inside ${system.name}.</div>`;
        return;
    }

    classList.forEach((classEntry, index) => {
        const fragment = template.content.cloneNode(true);
        const button = fragment.querySelector(".class-item");
        const kind = classEntry.metadata.kind || inferTypeKind(classEntry);

        fragment.querySelector(".class-item-name").textContent = classEntry.name;
        fragment.querySelector(".class-item-count").textContent = kind;
        fragment.querySelector(".class-item-path").textContent = formatClassPath(classEntry);

        if (selectedClass && getClassKey(classEntry) === getClassKey(selectedClass)) {
            button.classList.add("active");
        }

        button.addEventListener("click", () => {
            state.selectedClassIndex = index;
            renderClassList(system, classList, classEntry);
            renderClassDetails(system, classEntry);
        });

        classListNode.append(fragment);
    });
}

function renderClassDetails(system, classEntry) {
    const classTitle = document.getElementById("classTitle");
    const classPath = document.getElementById("classPath");
    const classBadges = document.getElementById("classBadges");
    const classSummary = document.getElementById("classSummary");
    const functionMeta = document.getElementById("functionMeta");
    const functionCards = document.getElementById("functionCards");
    const template = document.getElementById("functionCardTemplate");

    if (!classEntry) {
        classTitle.textContent = "No class selected";
        classPath.textContent = system ? `No class matched inside ${system.name}.` : "Select a system to begin.";
        classBadges.innerHTML = "";
        classSummary.innerHTML = `<div class="empty-state">Choose a class from the list to inspect its role and functions.</div>`;
        functionMeta.textContent = "0 functions";
        functionCards.innerHTML = "";
        return;
    }

    classTitle.textContent = classEntry.name;
    classPath.textContent = formatClassPath(classEntry);
    classBadges.innerHTML = "";
    classBadges.append(createPill(system.name));
    classBadges.append(createPill(`${classEntry.functions.length} functions`));
    classBadges.append(createPill(classEntry.metadata.kind || inferTypeKind(classEntry)));

    classSummary.innerHTML = "";
    buildClassSummary(system, classEntry).forEach(node => classSummary.append(node));

    functionMeta.textContent = `${classEntry.functions.length} functions`;
    functionCards.innerHTML = "";

    if (!classEntry.functions.length) {
        functionCards.innerHTML = `<div class="empty-state">No functions were listed for this class section.</div>`;
        return;
    }

    classEntry.functions.forEach(fn => {
        const fragment = template.content.cloneNode(true);
        fragment.querySelector(".function-signature").textContent = fn.signature;

        const body = fragment.querySelector(".function-body");
        if (fn.group) {
            const p = document.createElement("p");
            p.className = "function-group";
            p.textContent = fn.group;
            body.append(p);
        }

        const explanation = getFunctionExplanation(fn, classEntry);
        const notes = getFunctionNotes(fn, classEntry);
        const example = buildFunctionExample(fn, classEntry);

        body.append(createLabeledBlock("What it does", explanation));
        if (notes.length) {
            body.append(createListBlock("Notes", notes));
        }
        body.append(createCodeBlock("Example use", example));

        functionCards.append(fragment);
    });
}

function createPill(text) {
    const pill = document.createElement("span");
    pill.textContent = text;
    return pill;
}

function buildClassSummary(system, classEntry) {
    const nodes = [];
    nodes.push(createLabeledBlock("Summary", getClassExplanation(system, classEntry)));

    const useCase = getClassUseCase(classEntry);
    if (useCase) {
        nodes.push(createLabeledBlock("Example use case", useCase));
    }

    nodes.push(createCodeBlock("Quick example", buildClassExample(classEntry)));

    const highlights = getClassHighlights(classEntry);
    if (highlights.length) {
        nodes.push(createListBlock("Highlights", highlights));
    }

    return nodes;
}

function getClassKey(classEntry) {
    return `${classEntry.header}::${classEntry.name}`;
}

function getClassExplanation(system, classEntry) {
    if (classEntry.summary) {
        return classEntry.summary;
    }

    const textDetails = classEntry.details
        .filter(detail => detail.type === "text")
        .map(detail => detail.text)
        .filter(text => text !== classEntry.name);
    const bulletDetails = classEntry.details
        .filter(detail => detail.type === "bullet")
        .map(detail => detail.text);

    if (textDetails.length) {
        return textDetails[0];
    }
    if (bulletDetails.length) {
        return bulletDetails[0];
    }

    return `${classEntry.name} belongs to the ${system.name} system and exposes ${classEntry.functions.length} documented functions.`;
}

function getClassUseCase(classEntry) {
    if (classEntry.exampleUseCase) {
        return classEntry.exampleUseCase;
    }

    return "";
}

function getClassHighlights(classEntry) {
    if (classEntry.highlights.length) {
        return classEntry.highlights.slice(0, 3);
    }

    const textDetails = classEntry.details
        .filter(detail => detail.type === "text")
        .map(detail => detail.text)
        .filter(text => text !== classEntry.name);
    const bulletDetails = classEntry.details
        .filter(detail => detail.type === "bullet")
        .map(detail => detail.text);

    return [...textDetails.slice(1, 2), ...bulletDetails.slice(0, 3)].slice(0, 3);
}

function getFunctionExplanation(fn, classEntry) {
    if (fn.what) {
        return fn.what;
    }

    if (fn.paragraphs.length) {
        return fn.paragraphs[0];
    }
    if (fn.bullets.length) {
        return fn.bullets[0];
    }

    const methodName = extractMethodName(fn.signature);
    return `${methodName} is part of ${classEntry.name} and is available through this API surface.`;
}

function getFunctionNotes(fn) {
    if (fn.notes.length) {
        return fn.notes.slice(0, 4);
    }

    const notes = [];
    if (fn.paragraphs.length > 1) {
        notes.push(...fn.paragraphs.slice(1));
    }
    if (fn.bullets.length > 1) {
        notes.push(...fn.bullets.slice(1));
    }
    return notes.slice(0, 4);
}

function createLabeledBlock(label, text) {
    const wrapper = document.createElement("div");
    wrapper.className = "doc-block";

    const heading = document.createElement("p");
    heading.className = "doc-label";
    heading.textContent = label;
    wrapper.append(heading);

    const body = document.createElement("p");
    body.textContent = text;
    wrapper.append(body);
    return wrapper;
}

function createListBlock(label, items) {
    const wrapper = document.createElement("div");
    wrapper.className = "doc-block";

    const heading = document.createElement("p");
    heading.className = "doc-label";
    heading.textContent = label;
    wrapper.append(heading);

    const ul = document.createElement("ul");
    items.forEach(item => {
        const li = document.createElement("li");
        li.textContent = item;
        ul.append(li);
    });
    wrapper.append(ul);
    return wrapper;
}

function createCodeBlock(label, code) {
    const wrapper = document.createElement("div");
    wrapper.className = "doc-block";

    const heading = document.createElement("p");
    heading.className = "doc-label";
    heading.textContent = label;
    wrapper.append(heading);

    const pre = document.createElement("pre");
    pre.className = "example-code";
    const codeNode = document.createElement("code");
    codeNode.textContent = code;
    pre.append(codeNode);
    wrapper.append(pre);
    return wrapper;
}

function buildClassExample(classEntry) {
    if (classEntry.exampleCode) {
        return classEntry.exampleCode;
    }

    const map = {
        "Application": [
            "class MyGameApp : public Application",
            "{",
            "protected:",
            "    void OnInit() override",
            "    {",
            "        // Create startup scene content here.",
            "    }",
            "};"
        ].join("\n"),
        "InputState": [
            "if (input.CanUseRuntimeViewportInput() && input.IsKeyDown(GLFW_KEY_W))",
            "{",
            "    player.MoveWithCollision({0.0f, -speed * deltaTime});",
            "}"
        ].join("\n"),
        "Window": [
            "Window window(1920, 1080, \"Vulkan Engine Window\");",
            "if (window.Init())",
            "{",
            "    auto [width, height] = window.GetFramebufferSize();",
            "}"
        ].join("\n"),
        "Scene": [
            "Scene scene;",
            "Entity player = scene.CreateEntity(\"Player\");",
            "player.SetSpriteTexturePath(\"Assets/Textures/player.png\");",
            "scene.Update(deltaTime);",
            "scene.Render(renderer);"
        ].join("\n"),
        "Entity": [
            "Entity entity = scene.CreateEntity(\"Enemy\");",
            "entity.SetPosition({128.0f, 64.0f});",
            "entity.EnsureBoxCollider();",
            "entity.SetBoxColliderSize({32.0f, 32.0f});"
        ].join("\n"),
        "Camera2D": [
            "scene.GetCamera().SetTopLeftPosition({0.0f, 0.0f});",
            "scene.GetCamera().SetZoom(1.5f);"
        ].join("\n"),
        "SpriteRenderer": [
            "SpriteRenderer sprite;",
            "sprite.SetTexturePath(\"Assets/Textures/tiles.png\");",
            "sprite.SetSourceRectFromGrid(1, 0, 32, 32);"
        ].join("\n"),
        "SpriteAnimation": [
            "entity.EnsureAnimation();",
            "entity.SetAnimationSetPath(\"Assets/Animations/CharacterSpriteSheet.csv\");",
            "entity.PlayAnimation(\"Walk\");"
        ].join("\n"),
        "SpriteAnimationSet": [
            "SpriteAnimationSet animationSet;",
            "if (animationSet.LoadFromCSV(\"Assets/Animations/CharacterSpriteSheet.csv\"))",
            "{",
            "    const AnimationClip* walk = animationSet.FindClip(\"Walk\");",
            "}"
        ].join("\n"),
        "TileSetAsset": [
            "TileSetAsset tileSet;",
            "tileSet.SetTexturePath(\"Assets/Textures/tiles.png\");",
            "tileSet.SetGrid(8, 8);",
            "tileSet.SetTileSolid(3, true);"
        ].join("\n"),
        "Object": [
            "Player player = scene.Place<Player>();",
            "if (player.IsValid())",
            "{",
            "    player.SetName(\"Hero\");",
            "}"
        ].join("\n"),
        "Actor": [
            "Player player = scene.Place<Player>();",
            "player.SetPosition({64.0f, 64.0f});",
            "player.ConfigureSprite(\"Assets/Textures/character-spritesheet.png\", {64.0f, 64.0f});"
        ].join("\n"),
        "Player": [
            "Player player = scene.Place<Player>();",
            "player.BeginPlay();",
            "player.Move({1.0f, 0.0f}, deltaTime);"
        ].join("\n"),
        "VulkanRenderer": [
            "renderer.BeginFrame();",
            "scene.Render(renderer);",
            "renderer.EndFrame();"
        ].join("\n"),
        "EditorLayer": [
            "editorLayer.Draw(&scene, renderer, renderer.GetSceneViewportTextureID(), isPlaying);"
        ].join("\n"),
        "EditorSceneController": [
            "editorController.Update(scene, editorLayer, input, viewportState, isPlaying, deltaTime);",
            "editorController.Render(scene, editorLayer, renderer);"
        ].join("\n"),
        "SceneEditorPanel": [
            "sceneEditorPanel.Draw(scene, selectedObjectID);"
        ].join("\n"),
        "TileMapEditorPanel": [
            "tileMapEditorPanel.Draw(scene, renderer, selectedObjectID);"
        ].join("\n"),
        "TileMapEditorController": [
            "tileMapEditorController.Update(scene, tileMapEditorPanel, input, viewportState, true);"
        ].join("\n"),
        "TileSetEditorPanel": [
            "tileSetEditorPanel.Draw(renderer);"
        ].join("\n"),
        "SandboxApp": [
            "SandboxApp app;",
            "if (app.Init())",
            "{",
            "    app.Run();",
            "}"
        ].join("\n")
    };

    return map[classEntry.name] || buildGenericClassExample(classEntry);
}

function inferTypeKind(classEntry) {
    const name = classEntry.name;
    if (name.includes("::")) {
        return "nested type";
    }
    if ((classEntry.metadata.kind || "").trim()) {
        return classEntry.metadata.kind;
    }
    if (/Math/.test(classEntry.header)) {
        return "utility";
    }
    if (/^[A-Z]/.test(name) && /Component$/.test(name)) {
        return "component";
    }
    return "type";
}

function formatClassPath(classEntry) {
    return classEntry.metadata.declaredIn || classEntry.header;
}

function buildGenericClassExample(classEntry) {
    const identifier = toVariableName(classEntry.name);
    return [
        `// Example for ${classEntry.name}`,
        `${classEntry.name} ${identifier};`,
        `// Use the functions below to work with ${identifier}.`
    ].join("\n");
}

function buildFunctionExample(fn, classEntry) {
    const signature = fn.signature;
    const methodName = extractMethodName(signature);
    const args = extractArgumentList(signature);
    const argsExample = buildArgumentExample(args);
    const target = buildInvocationTarget(signature, classEntry);

    if (!methodName) {
        return `// Example for ${signature}`;
    }

    if (isConstructorSignature(signature, classEntry.name)) {
        const constructorArgs = argsExample || "";
        return `${classEntry.name} ${toVariableName(classEntry.name)}(${constructorArgs});`;
    }

    if (methodName.startsWith("~")) {
        return `// ${methodName} is normally called automatically when ${classEntry.name} goes out of scope.`;
    }

    if (/^(Is|Has|Can|Was)/.test(methodName)) {
        return [
            `if (${target}.${methodName}(${argsExample}))`,
            "{",
            "    // React to the returned state here.",
            "}"
        ].join("\n");
    }

    if (/^Get/.test(methodName)) {
        return `auto value = ${target}.${methodName}(${argsExample});`;
    }

    if (/^Set/.test(methodName)) {
        return `${target}.${methodName}(${argsExample});`;
    }

    if (/^(Create|Load|Save|Init|Update|Render|Draw|Ensure|Remove|Destroy|Begin|End|Move|Play|Stop|Reset|Register|Resize|Clear|Sync|Refresh)/.test(methodName)) {
        return `${target}.${methodName}(${argsExample});`;
    }

    return `${target}.${methodName}(${argsExample});`;
}

function buildInvocationTarget(signature, classEntry) {
    if (/^static /.test(signature)) {
        return classEntry.name;
    }

    if (signature.includes("::")) {
        const owner = signature.split("::")[0].split(" ").pop();
        return owner;
    }

    return toVariableName(classEntry.name);
}

function extractMethodName(signature) {
    const callMatch = signature.match(/([~A-Za-z_][A-Za-z0-9_:<>]*)\s*\(/);
    if (!callMatch) {
        return "";
    }

    const fullName = callMatch[1];
    const pieces = fullName.split("::");
    return pieces[pieces.length - 1];
}

function isConstructorSignature(signature, className) {
    return extractMethodName(signature) === className;
}

function extractArgumentList(signature) {
    const match = signature.match(/\((.*)\)/);
    if (!match) {
        return [];
    }

    const inner = match[1].trim();
    if (!inner || inner === "void") {
        return [];
    }

    return inner.split(",").map(part => part.trim());
}

function buildArgumentExample(args) {
    return args.map(arg => {
        if (/bool/.test(arg)) return "true";
        if (/float/.test(arg)) return "1.0f";
        if (/double/.test(arg)) return "1.0";
        if (/int/.test(arg) || /uint/.test(arg) || /size_t/.test(arg)) return "0";
        if (/std::string/.test(arg) || /const char/.test(arg)) return "\"example\"";
        if (/glm::vec2/.test(arg)) return "{0.0f, 0.0f}";
        if (/glm::vec4/.test(arg)) return "{1.0f, 1.0f, 1.0f, 1.0f}";
        if (/Transform2D/.test(arg)) return "{}";
        if (/Entity/.test(arg)) return "entity";
        if (/Scene/.test(arg)) return "scene";
        if (/VulkanRenderer/.test(arg)) return "renderer";
        if (/InputState/.test(arg)) return "input";
        if (/SceneViewportState/.test(arg)) return "viewportState";
        if (/GameObjectID/.test(arg)) return "0";
        return "/* ... */";
    }).join(", ");
}

function toVariableName(name) {
    const cleaned = name.replace(/[^A-Za-z0-9]/g, "");
    if (!cleaned) {
        return "value";
    }

    return cleaned.charAt(0).toLowerCase() + cleaned.slice(1);
}

function bindEvents() {
    const searchInput = document.getElementById("searchInput");
    searchInput.addEventListener("input", event => {
        state.query = event.target.value;
        state.selectedClassIndex = 0;
        normalizeSelection();
        renderExplorer();
    });
}

function showLoadError(error) {
    document.getElementById("systemsGrid").innerHTML = `
        <div class="empty-state">
            The docs site could not load its markdown sources.<br>
            Start it with <code>DocsSite\\launch-docs.bat</code> or <code>DocsSite\\launch-docs.ps1</code> so the browser can fetch the docs files.
            <br><br>
            <strong>Details:</strong> ${error.message}
        </div>`;
}
