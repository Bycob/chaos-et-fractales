cmake_minimum_required(VERSION 2.8)

project(RenduOpenGL)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build)

set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(GLOB_RECURSE
        SIMULATION_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sources/*.h)

file(GLOB_RECURSE
        GLAD_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/*.c)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenCV REQUIRED)

# Target principale

add_executable(simulation ${SIMULATION_SOURCES} ${GLAD_SOURCES})
target_link_libraries(simulation glfw ${OpenCV_LIBS})

#copie des dossiers
add_custom_command(TARGET simulation PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/sources/rendering/shaders $<TARGET_FILE_DIR:simulation>/shaders)

add_custom_command(TARGET simulation PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:simulation>/assets/)

add_custom_command(TARGET simulation PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/examples/ $<TARGET_FILE_DIR:simulation>/examples/)
