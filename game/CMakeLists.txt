cmake_minimum_required(VERSION 4.1)
project(visual-script-bin)

add_subdirectory(external/raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS code/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
)
target_include_directories(${PROJECT_NAME}
        PRIVATE code
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CLEAN_BIN_DIR}
        OUTPUT_NAME "vs"
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/game/resources"
        "${CLEAN_BIN_DIR}/resources"
)

execute_process(
        COMMAND git rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_HASH
        RESULT_VARIABLE GIT_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT GIT_RESULT EQUAL 0)
    set(GIT_HASH "nogit")
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_GIT_HASH="${GIT_HASH}")
