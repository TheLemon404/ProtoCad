set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

file(GLOB_RECURSE src CONFIGURE_DEPENDS "*.cpp" "*.h")
add_executable(ProtoCAD ${src})

# vulkan
target_include_directories(ProtoCAD PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/vulkan/vulkan_loader/windows/include)

target_link_directories(ProtoCAD PRIVATE ${PROJECT_SOURCE_DIR}/dependencies/vulkan/vulkan_loader/windows/lib)

target_link_libraries(ProtoCAD PRIVATE glfw glm vulkan-1)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ProtoCAD PROPERTY CXX_STANDARD 20)
endif()