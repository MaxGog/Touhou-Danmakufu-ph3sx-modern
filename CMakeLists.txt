cmake_minimum_required(VERSION 3.24)

project(TouhouDanmakufuPh3sx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)

add_library(ph3sx_sdl_platform STATIC
	source/GcLib/platform/SDLPlatform.cpp
)

target_include_directories(ph3sx_sdl_platform
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/source/GcLib/platform"
)

target_link_libraries(ph3sx_sdl_platform PUBLIC SDL3::SDL3)

if(MSVC)
	target_compile_options(ph3sx_sdl_platform PRIVATE /W4 /utf-8)
else()
	target_compile_options(ph3sx_sdl_platform PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
	add_executable(ph3sx_sdl_platform_tests
		tests/SDLPlatformTests.cpp
	)
	target_link_libraries(ph3sx_sdl_platform_tests PRIVATE ph3sx_sdl_platform)
	add_test(NAME ph3sx_sdl_platform_tests COMMAND ph3sx_sdl_platform_tests)
	set_tests_properties(ph3sx_sdl_platform_tests PROPERTIES
		ENVIRONMENT "SDL_VIDEODRIVER=dummy"
	)
endif()
