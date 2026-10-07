cmake_minimum_required(VERSION 3.24)

project(TouhouDanmakufuPh3sx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PH3SX_ENABLE_SLANG "Build the cross-platform Slang shader compiler" ON)

find_package(SDL3 CONFIG REQUIRED)
if(PH3SX_ENABLE_SLANG)
	find_package(slang CONFIG REQUIRED)
endif()

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

if(WIN32)
	add_library(ph3sx_renderer STATIC
		source/GcLib/renderer/RendererFactory.cpp
		source/GcLib/renderer/D3D11Device.cpp
	)
	target_link_libraries(ph3sx_renderer
		PUBLIC SDL3::SDL3
		PRIVATE d3d11 dxgi d3dcompiler
	)
elseif(APPLE)
	enable_language(OBJCXX)
	set(CMAKE_OBJCXX_STANDARD 20)
	set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)

	find_library(METAL_FRAMEWORK Metal REQUIRED)
	find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)
	find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)

	add_library(ph3sx_renderer STATIC
		source/GcLib/renderer/RendererFactory.cpp
		source/GcLib/renderer/MetalDevice.mm
	)
	target_link_libraries(ph3sx_renderer
		PUBLIC SDL3::SDL3
		PRIVATE
			"${METAL_FRAMEWORK}"
			"${QUARTZCORE_FRAMEWORK}"
			"${FOUNDATION_FRAMEWORK}"
	)
	target_compile_options(ph3sx_renderer PRIVATE
		-Wall -Wextra -Wpedantic
		$<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>
	)
endif()

if(TARGET ph3sx_renderer)
	target_include_directories(ph3sx_renderer
		PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/GcLib/renderer"
	)
	target_compile_features(ph3sx_renderer PUBLIC cxx_std_20)
	if(MSVC)
		target_compile_options(ph3sx_renderer PRIVATE /W4 /utf-8)
	endif()
endif()

if(PH3SX_ENABLE_SLANG)
	target_sources(ph3sx_renderer PRIVATE
		source/GcLib/renderer/ShaderCompiler.cpp
	)
	if(TARGET slang::slang)
		target_link_libraries(ph3sx_renderer PUBLIC slang::slang)
	elseif(TARGET slang)
		target_link_libraries(ph3sx_renderer PUBLIC slang)
	else()
		message(FATAL_ERROR
			"Slang was found, but its CMake package exports neither slang::slang nor slang.")
	endif()
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
	if(TARGET ph3sx_renderer)
		add_executable(ph3sx_renderer_factory_tests
			tests/RendererFactoryTests.cpp
		)
		target_link_libraries(ph3sx_renderer_factory_tests PRIVATE ph3sx_renderer)
		add_test(NAME ph3sx_renderer_factory_tests COMMAND ph3sx_renderer_factory_tests)
	endif()
	if(APPLE AND TARGET ph3sx_renderer)
		add_executable(ph3sx_metal_device_tests
			tests/MetalDeviceTests.mm
		)
		target_link_libraries(ph3sx_metal_device_tests PRIVATE ph3sx_renderer)
		target_compile_options(ph3sx_metal_device_tests PRIVATE -fobjc-arc)
		add_test(NAME ph3sx_metal_device_tests COMMAND ph3sx_metal_device_tests)
		set_tests_properties(ph3sx_metal_device_tests PROPERTIES SKIP_RETURN_CODE 77)
	endif()
	if(PH3SX_ENABLE_SLANG AND TARGET ph3sx_renderer)
		add_executable(ph3sx_shader_compiler_tests
			tests/ShaderCompilerTests.cpp
		)
		target_link_libraries(ph3sx_shader_compiler_tests PRIVATE ph3sx_renderer)
		add_test(NAME ph3sx_shader_compiler_tests COMMAND ph3sx_shader_compiler_tests)
	endif()
endif()
