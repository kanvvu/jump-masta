file(GLOB TEST_SOURCES CONFIGURE_DEPENDS test_*.cpp )

add_executable(unit_tests ${TEST_SOURCES} ../src/game/saveSystem.cpp)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(unit_tests PRIVATE raylib)

if(MSVC)
    target_compile_options(unit_tests PRIVATE /W4 /permissive-)
endif()

add_test(NAME GridTests COMMAND unit_tests)
