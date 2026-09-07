file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.c *.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.h *.hpp)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

option(DEV_MODE "Enable Developer Mode to save directly to src" ON)

if(DEV_MODE)
    # Tells C++ that DEV_MODE exists
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEV_MODE)
    
    # Safely passes the absolute path to C++
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        "SOURCE_RESOURCE_DIR=\"${CMAKE_SOURCE_DIR}/src/resources/\""
    )
endif()