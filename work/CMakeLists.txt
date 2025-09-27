cmake_minimum_required(VERSION 3.5.0)

project(MenuWork VERSION 0.1.0 LANGUAGES C)

set(LCD1602_DIR "/home/yevst/Projects/CCPP/lcd1602sdl02sym/cmake")

# Проверяем существование Config файла
if(NOT EXISTS "${LCD1602_DIR}/LCD1602Config.cmake")
    message(FATAL_ERROR "LCD1602Config.cmake not found at: ${LCD1602_DIR}")
endif()

message("Module PATH: ${CMAKE_MODULE_PATH}")

# Находим libconfig
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LCD1602 REQUIRED)

set(LIB_SOURCES 
    src/menu_tree.c
    src/menu_config.c
    src/menu_values.c
    src/menu_context.c
    src/menu_edit.c
    src/menu_draw.c
    src/menu_engine.c
    src/menu_navigate.c
    )

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

add_library(${PROJECT_NAME}
    ${LIB_SOURCES}
    ${INCLUDES}
)    

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIRECTORIES}
    ${SDL2_INCLUDE_DIRS}
    ${LCD1602_INCLUDE_DIRS}
    ${LIB_LCD1602SDL_INCLUDE}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
)
