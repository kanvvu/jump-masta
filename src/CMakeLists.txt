file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.c *.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.h *.hpp)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

option(DEV_MODE "Enable Developer Mode to save directly to src" OFF)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
endif()

if(DEV_MODE)
    # Tells C++ that DEV_MODE exists
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEV_MODE)
    
    # Safely passes the absolute path to C++
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        "SOURCE_RESOURCE_DIR=\"${CMAKE_SOURCE_DIR}/src/resources/\""
    )
endif()