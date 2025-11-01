cmake_minimum_required(VERSION 3.14)
project(SpotifyPlaylistSplitter VERSION 0.1 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
set(SOURCES
        src/main.cpp
        src/Controller.cpp
        src/Model.cpp
        src/View.cpp
)

# Executable
add_executable(SpotifyPlaylistSplitter ${SOURCES})

# Public include directory (your headers)
target_include_directories(SpotifyPlaylistSplitter
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Find nlohmann_json (system package). If found, link target; otherwise assume single-header in include/
find_package(nlohmann_json QUIET)
if (nlohmann_json_FOUND)
    message(STATUS "Using system nlohmann_json package")
    target_link_libraries(SpotifyPlaylistSplitter PRIVATE nlohmann_json::nlohmann_json)
else()
    message(STATUS "System nlohmann_json not found — using include/ (ensure include/nlohmann/json.hpp exists)")
    # include/ already added above
endif()

# Find gtkmm via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Add compile and include flags from pkg-config
target_include_directories(SpotifyPlaylistSplitter PRIVATE ${GTKMM_INCLUDE_DIRS})
target_compile_options(SpotifyPlaylistSplitter PRIVATE ${GTKMM_CFLAGS_OTHER})
target_link_libraries(SpotifyPlaylistSplitter PRIVATE ${GTKMM_LIBRARIES})

# Recommended compiler warnings
target_compile_options(SpotifyPlaylistSplitter PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
)

# Install target
install(TARGETS SpotifyPlaylistSplitter RUNTIME DESTINATION bin)
