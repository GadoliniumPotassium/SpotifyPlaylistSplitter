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

# Include your headers
target_include_directories(SpotifyPlaylistSplitter PRIVATE ${CMAKE_SOURCE_DIR}/include)

# ---- Dependencies ----

# CPR (modern fetch method, no manual curl linking)
include(FetchContent)
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.10.4
)
FetchContent_MakeAvailable(cpr)
target_link_libraries(SpotifyPlaylistSplitter PRIVATE cpr::cpr)

# nlohmann_json (use system if available)
find_package(nlohmann_json QUIET)
if (nlohmann_json_FOUND)
    message(STATUS "Using system nlohmann_json package")
    target_link_libraries(SpotifyPlaylistSplitter PRIVATE nlohmann_json::nlohmann_json)
else ()
    message(WARNING "System nlohmann_json not found — using header-only include/")
    target_include_directories(SpotifyPlaylistSplitter PRIVATE ${CMAKE_SOURCE_DIR}/include)
endif ()

# GTKMM via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
target_include_directories(SpotifyPlaylistSplitter PRIVATE ${GTKMM_INCLUDE_DIRS})
target_compile_options(SpotifyPlaylistSplitter PRIVATE ${GTKMM_CFLAGS_OTHER})
target_link_libraries(SpotifyPlaylistSplitter PRIVATE ${GTKMM_LIBRARIES})

# laserpants_dotenv
find_package(laserpants_dotenv REQUIRED)
if (laserpants_dotenv_FOUND)
    target_link_libraries(SpotifyPlaylistSplitter PRIVATE laserpants::dotenv)
else ()
    message(FATAL_ERROR "laserpants_dotenv not found. Please install it.")
endif ()

# Compiler warnings
target_compile_options(SpotifyPlaylistSplitter PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
)

# Install target
install(TARGETS SpotifyPlaylistSplitter RUNTIME DESTINATION bin)
