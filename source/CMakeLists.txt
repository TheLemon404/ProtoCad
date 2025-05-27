set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

# find packages
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE src CONFIGURE_DEPENDS "*.cpp" "*.h")
add_executable(ProtoCAD ${src})

target_include_directories(ProtoCAD PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/vulkan/vulkan_loader/windows/include)
target_link_directories(ProtoCAD PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/vulkan/vulkan_loader/windows/lib)

# Open CASCADE Technology
find_package (OpenCASCADE REQUIRED)
if (NOT OpenCASCADE_FOUND)
    message (FATAL_ERROR "could not find OpenCASCADE, please set OpenCASCADE_DIR variable" )
else()
    message (STATUS "Using OpenCASCADE from \"${OpenCASCADE_INSTALL_PREFIX}\"" )
    message (STATUS "OpenCASCADE_INCLUDE_DIR=${OpenCASCADE_INCLUDE_DIR}")
    message (STATUS "OpenCASCADE_LIBRARY_DIR=${OpenCASCADE_LIBRARY_DIR}")
    INCLUDE_DIRECTORIES(${OpenCASCADE_INCLUDE_DIR})
endif()

SET(OpenCASCADE_LIBS
        TKernel
        TKService
        TKV3d
        TKOpenGl
        TKBRep
        TKGeomBase
        TKGeomAlgo
        TKG3d
        TKG2d
        TKTopAlgo
        TKPrim
)

target_link_libraries(ProtoCAD PRIVATE glfw glm vulkan-1 ImGui ImGuiBackends glad ImOGuizmo ImGuizmo)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ProtoCAD PROPERTY CXX_STANDARD 20)
endif()