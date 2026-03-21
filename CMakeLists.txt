cmake_minimum_required(VERSION 3.20)
project(fx_movies)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# === GLFW ===
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# === Dear ImGui ===
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8
)
FetchContent_MakeAvailable(imgui)

add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_lib PUBLIC glfw)

# === FFmpeg (system) ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libavutil
    libswscale
)

# === OpenGL ===
find_package(OpenGL REQUIRED)

# === Main executable ===
add_executable(fx_movies
    main.cpp
    src/app/cli.cpp
    src/app/application.cpp
    src/asset/asset_scanner.cpp
    src/video/video_player.cpp
    src/ui/asset_panel.cpp
    src/ui/preview_panel.cpp
)
target_include_directories(fx_movies PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(fx_movies PRIVATE
    imgui_lib
    OpenGL::GL
    PkgConfig::LIBAV
)

# === GoogleTest ===
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(fx_movies_tests
    tests/unit/test_asset.cpp
    tests/unit/test_cli_args.cpp
    tests/unit/test_asset_scanner.cpp
    src/app/cli.cpp
    src/asset/asset_scanner.cpp
)
target_include_directories(fx_movies_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(fx_movies_tests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(fx_movies_tests)
