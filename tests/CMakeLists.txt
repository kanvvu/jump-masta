add_executable(unit_tests test_grid.cpp)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(unit_tests PRIVATE /W4 /permissive-)
endif()

add_test(NAME GridTests COMMAND unit_tests)
