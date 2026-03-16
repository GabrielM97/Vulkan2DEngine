# Root include path for Engine modules
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------
# Core module
# ----------------------------
file(GLOB_RECURSE CORE_SRC Core/*.cpp Core/*.h)
if(CORE_SRC)
    add_library(Core ${CORE_SRC})
    target_include_directories(Core PUBLIC Core)
    target_link_libraries(Core PUBLIC EngineDeps)
    foreach(_file ${CORE_SRC})
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${_file})
    endforeach()
endif()

# ----------------------------
# Platform module
# ----------------------------
file(GLOB_RECURSE PLATFORM_SRC Platform/*.cpp Platform/*.h)
if(PLATFORM_SRC)
    add_library(Platform ${PLATFORM_SRC})
    target_include_directories(Platform PUBLIC Platform)
    target_link_libraries(Platform PUBLIC EngineDeps)
    foreach(_file ${PLATFORM_SRC})
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${_file})
    endforeach()
endif()

# ----------------------------
# Renderer module
# ----------------------------
file(GLOB_RECURSE RENDERER_SRC Renderer/*.cpp Renderer/*.h)
if(RENDERER_SRC)
    add_library(Renderer ${RENDERER_SRC})
    target_include_directories(Renderer PUBLIC Renderer)
    target_link_libraries(Renderer PUBLIC EngineDeps)
    foreach(_file ${RENDERER_SRC})
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${_file})
    endforeach()
endif()