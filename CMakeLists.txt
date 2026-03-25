cmake_minimum_required(VERSION 3.20)
project(fx-movies CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# fx_ui — core UI library (Menu, MenuItem, Command)
# ---------------------------------------------------------------------------
add_library(fx_ui
    src/ui/menu_item.cpp
    src/ui/menu.cpp
)
target_include_directories(fx_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(fx_tests
    tests/unit/menu_tests.cpp
)
target_link_libraries(fx_tests
    fx_ui
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(fx_tests)
