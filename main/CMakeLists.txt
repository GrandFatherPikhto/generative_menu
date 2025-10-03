cmake_minimum_required(VERSION 3.5.0)

project(Simulator VERSION 0.1.0 LANGUAGES C)

set(LCD1602_DIR "../../../lcd1602sdl02sym/cmake")

message("Source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

# Проверяем существование Config файла
if(NOT EXISTS "${LCD1602_DIR}/LCD1602Config.cmake")
    message(FATAL_ERROR "LCD1602Config.cmake not found at: ${LCD1602_DIR}")
endif()

# Находим libconfig
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(LCD1602 REQUIRED)

set(EXEC_SOURCES
    src/main.c
    src/lcd1602_data.c
)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${LCD1602_INCLUDE_DIRS}
    ../menu/src/include
)

message("LCD1602 Include Dir ${LCD1602_INCLUDE_DIRS}")

add_executable(${PROJECT_NAME}
    ${EXEC_SOURCES} 
    ${INCLUDES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    LCD1602::LCD1602
    Menu
)

message("Menu Work Includes: ${MenuWork}")

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIRECTORIES}
    ${SDL2_INCLUDE_DIRS}
    ${LCD1602_INCLUDE_DIRS}
    ${LIB_LCD1602SDL_INCLUDE}
)

set_target_properties(${EXEC_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
)
