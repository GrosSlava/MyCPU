cmake_minimum_required(VERSION 3.10)

project(CPUTools)



# Compiler config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_SUPPRESS_REGENERATION ON)

# Set CMake property
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/Install" CACHE PATH "default install path" FORCE)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Bin/Release")

# Platform config
if(MSVC)
	add_definitions(/MP)
	add_compile_definitions(WIN32 WIN64)
elseif(UNIX AND NOT APPLE)
	add_compile_definitions(LINUX __linux)
else()
	message(FATAL_ERROR "Unsupported platform!")
endif()

# Build config
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(EngineBuildOutputDir "${CMAKE_BINARY_DIR}/Bin/Debug")
	add_compile_definitions(_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(EngineBuildOutputDir "${CMAKE_BINARY_DIR}/Bin/Release")
	add_compile_definitions(_RELEASE NDEBUG)
else()
	message(FATAL_ERROR "Build type is not selected!")
endif()
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(UNICODE _UNICODE)


# Tools
add_subdirectory(ASM)

# Install
install(TARGETS ASM DESTINATION CPUTools)

# Tests
enable_testing()
add_subdirectory(Tests)
#add_custom_target(AllTests DEPENDS AsmTestsMain)
#set_target_properties(AllTests AsmTestsMain PROPERTIES FOLDER Tests)