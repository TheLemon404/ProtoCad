set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

# find packages
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE src CONFIGURE_DEPENDS "*.cpp" "*.h")
add_executable(ProtoCAD ${src})

# occt
find_package(OpenCASCADE REQUIRED)

target_include_directories(ProtoCAD PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/vulkan/vulkan_loader/windows/include ${OpenCASCADE_INCLUDE_DIRS})
target_link_directories(ProtoCAD PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/vulkan/vulkan_loader/windows/lib ${OpenCASCADE_LIBRARY_DIR})

target_link_libraries(ProtoCAD PRIVATE glfw glm vulkan-1 ImGui ImGuiBackends glad ImOGuizmo ImGuizmo)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ProtoCAD PROPERTY CXX_STANDARD 20)
endif()