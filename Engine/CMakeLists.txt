# --- Core ---
file(GLOB_RECURSE CORE_SRC Core/*.cpp Core/*.h)
add_library(Core STATIC ${CORE_SRC})
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Core PUBLIC EngineDeps)

# --- Platform ---
file(GLOB_RECURSE PLATFORM_SRC Platform/*.cpp Platform/*.h)
add_library(Platform STATIC ${PLATFORM_SRC})
target_include_directories(Platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Platform PUBLIC Core EngineDeps)

# --- Renderer ---
file(GLOB_RECURSE RENDERER_SRC Renderer/*.cpp Renderer/*.h)
add_library(Renderer STATIC ${RENDERER_SRC})
target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Renderer PUBLIC Core Platform EngineDeps)

# --- Scene ---
file(GLOB_RECURSE SCENE_SRC
        CONFIGURE_DEPENDS
        Scene/*.cpp
        Scene/*.h
)
add_library(Scene STATIC ${SCENE_SRC})
target_include_directories(Scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Scene PUBLIC Renderer EngineDeps)