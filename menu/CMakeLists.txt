cmake_minimum_required(VERSION 3.5.0)

project(Menu VERSION 0.1.0 LANGUAGES C)

set(PROJECT_NAME menu)

set(EXEC_NAME ${PROJECT_NAME})
set(LIB_NAME ${PROJECT_NAME}_lib)

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
    src/lcd1602.c
    src/rotenc_menu.c
    src/rotary_encoder.c
    src/rotenc_callback.c
    )

set(EXEC_SOURCES
    src/main.c
)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

add_executable(${EXEC_NAME}
    ${EXEC_SOURCES} 
    ${INCLUDES})

add_library(${LIB_NAME}
    ${LIB_SOURCES}
    ${INCLUDES}
)    

target_link_libraries(${EXEC_NAME} PRIVATE
    ${LIB_NAME}
    LCD1602::LCD1602
)

target_include_directories(${EXEC_NAME} PRIVATE 
    ${INCLUDE_DIRECTORIES}
    ${SDL2_INCLUDE_DIRS}
    ${LCD1602_INCLUDE_DIRS}
    ${LIB_LCD1602SDL_INCLUDE}
)

target_include_directories(${LIB_NAME} PRIVATE 
    ${INCLUDE_DIRECTORIES}
    ${SDL2_INCLUDE_DIRS}
    ${LCD1602_INCLUDE_DIRS}
    ${LIB_LCD1602SDL_INCLUDE}
)

set_target_properties(${EXEC_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
)

set_target_properties(${LIB_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
)
