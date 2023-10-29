cmake_minimum_required(VERSION 3.10)

project(ASM)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message(FATAL_ERROR "${PROJECT_NAME} can't be selected as main project!")
endif()



file(GLOB_RECURSE HFiles "${PROJECT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE CppFiles "${PROJECT_SOURCE_DIR}/*.cpp")
foreach(LSource IN LISTS HFiles CppFiles)
	get_filename_component(SourceAbsPath "${LSource}" ABSOLUTE)
	get_filename_component(GroupDir "${SourceAbsPath}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${GroupDir}")
	string(REPLACE "/" "\\" GROUP2 "${GROUP}")
	source_group("${GROUP2}" FILES "${LSource}")
endforeach()

add_executable(ASM ${HFiles} ${CppFiles})
target_include_directories(ASM 
	PUBLIC "${PROJECT_SOURCE_DIR}"
)
