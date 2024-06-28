file(GLOB SOURCES "src/*.c")

add_executable(createdisk ${SOURCES})
target_link_libraries(createdisk PUBLIC common fat)

add_custom_command(
    TARGET createdisk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running post build scripts..."
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/../tools/
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:createdisk> ${CMAKE_BINARY_DIR}/../tools/
)