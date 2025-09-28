cmake_minimum_required(VERSION 3.5.0)

project(MenuData VERSION 0.1.0 LANGUAGES C)

message("Module PATH: ${CMAKE_MODULE_PATH}")

# Находим libconfig
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(LIB_SOURCES 
    src/menu_data.c
    )

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    "../lcd1602/src/include"
    "../work/src/include"
)

add_library(${PROJECT_NAME}
    ${LIB_SOURCES}
    ${INCLUDES}
)    

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIRECTORIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
)
