# 🎛️ MenuLCD1602 Project

Проект для тестирования и демонстрации работы сгенерированного меню на эмуляторе LCD1602 с использованием библиотеки [lcd1602sdl2sym](https://github.com/GrandFatherPikhto/lcd1602sdl2sym).

## 📋 Оглавление

- [🎯 Общее описание](#-общее-описание)
- [🏗️ Структура проекта](#️-структура-проекта)
- [🚀 Быстрый старт](#-быстрый-старт)
- [🔧 Сборка проекта](#-сборка-проекта)
- [🎮 Использование](#-использование)
- [📁 Описание файлов](#-описание-файлов)
- [🔌 Интеграция с меню](#-интеграция-с-меню)
- [🐛 Отладка](#-отладка)

## 🎯 Общее описание

**MenuLCD1602** - это демонстрационный проект, который показывает работу автоматически сгенерированной системы меню на эмуляторе LCD дисплея 1602. Проект использует:

- **Сгенерированное меню** из Python-генератора
- **[Эмулятор LCD1602](https://github.com/GrandFatherPikhto/lcd1602sdl2sym)** на базе [SDL2](https://www.libsdl.org/)
- **Виртуальный энкодер** для навигации по меню

## 🏗️ Структура проекта

```
MenuLCD1602/
├── 📁 main/                          # Основное приложение
│   ├── CMakeLists.txt               # Конфигурация сборки main
│   └── 📁 src/
│       ├── 📁 include/              # Заголовочные файлы
│       │   ├── common.h
│       │   ├── lcd1602.h
│       │   ├── lcd1602_data.h       # Callback-функции меню
│       │   ├── rotary_encoder.h
│       │   └── rotenc_callback.h
│       ├── main.c                   # Главный файл приложения
│       ├── lcd1602_data.c           # Данные и callback'и меню
│       └── ...                      # Другие исходные файлы
├── 📁 menu/                         # Сгенерированное меню
│   ├── CMakeLists.txt               # Конфигурация сборки меню
│   └── 📁 src/
│       ├── 📁 include/              # Заголовочные файлы меню
│       │   ├── menu.h
│       │   ├── menu_context.h
│       │   ├── menu_type.h
│       │   └── ...
│       └── *.c                      # Сгенерированные исходные файлы
└── CMakeLists.txt                   # Корневой CMake
```

## 🚀 Быстрый старт

### Предварительные требования

1. **Установите зависимости:**
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libsdl2-ttf-dev cmake build-essential

# Arch Linux
sudo pacman -S sdl2 sdl2_ttf cmake gcc

# macOS (с Homebrew)
brew install sdl2 sdl2_ttf cmake
```

2. **Клонируйте и соберите библиотеку lcd1602sdl2sym:**
```bash
git clone https://github.com/GrandFatherPikhto/lcd1602sdl2sym.git
cd lcd1602sdl2sym
mkdir build && cd build
cmake .. && make
sudo make install  # или установите в пользовательскую директорию
```

### Сборка проекта

```bash
# Клонируйте проект
git clone <your-repo-url>
cd MenuLCD1602

# Создайте директорию сборки
mkdir build
cd build

# Настройте и соберите проект
cmake ..
make

# Запустите приложение
./main/Simulator
```

## 🔧 Сборка проекта

### Настройка путей

В файле `main/CMakeLists.txt` укажите правильный путь к библиотеке LCD1602:

```cmake
# Замените путь на актуальный
set(LCD1602_DIR "/path/to/your/lcd1602sdl2sym/cmake")
```

```cmake
# Подключите библиотеку menu
target_link_libraries(${PROJECT_NAME} PRIVATE
    LCD1602::LCD1602
    Menu
)
```

# Должно получиться что-то такое:
```cmake
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
    # ${LIB_NAME}
    LCD1602::LCD1602
    Menu
)

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
```

### Опции сборки

Проект использует CMake с следующими целями:

- **Menu** - библиотека сгенерированного меню
- **Simulator** - главное приложение с эмулятором

## 🎮 Использование

### Управление меню

Приложение эмулирует работу с физическим энкодером:

- **🔄 Поворот энкодера** - навигация по меню
- **🔘 Короткое нажатие** - вход в пункт/подтверждение
- **🔘 Долгое нажатие** - возврат/отмена  
- **🔘 Двойное нажатие** - сброс значений

### Интерфейс LCD1602

Дисплей показывает две строки:
- **Верхняя строка**: название текущего пункта меню
- **Нижняя строка**: значение или подпункты

## 📁 Описание файлов

### Главные файлы приложения

#### `main.c`
Основной файл приложения, содержащий:
- Инициализацию меню и эмулятора LCD
- Callback-функции для обработки событий энкодера
- Главный цикл приложения
- Функцию отрисовки меню на LCD

#### `lcd1602_data.c`
Содержит пользовательские callback-функции для меню:
- `pwm_frequency_display_cb()` - отображение частоты ШИМ
- `pwm_frequency_change_cb()` - изменение частоты ШИМ
- `draw_version_cb()` - отображение версии прошивки
- `rotenc_hi_duty_changed()` - обработка изменения скважности

### Сгенерированные файлы меню

Файлы в директории `menu/src/` автоматически генерируются Python-скриптом и включают:

- **`menu.h`** - основной заголовочный файл
- **`menu_context.h/c`** - контекст меню
- **`menu_type.h`** - типы данных и структуры
- **`menu_navigate.h/c`** - функции навигации
- **`menu_draw.h/c`** - функции отрисовки

## 🔌 Интеграция с меню

### Инициализация меню

```c
// В main.c
menu_init();  // Инициализация системы меню
```

### Обработка событий энкодера

```c
void on_position_change(int new_pos) {
    // Обработка поворота энкодера
    menu_position(delta);
}

void on_push_button() {
    // Короткое нажатие - вход
    menu_enter();
}

void on_long_push_button() {
    // Долгое нажатие - возврат
    menu_back();
}
```

### Работа с отрисовкой

```c
void menu_draw(sdl_handle_t *lcd) {
    lcd_sdl_clear(lcd);
    lcd_sdl_set_cursor(lcd, 0, 0);
    lcd_sdl_print_str(lcd, menu_title_buf());  // Заголовок
    lcd_sdl_set_cursor(lcd, 0, 1);
    lcd_sdl_print_str(lcd, menu_value_buf());  // Значение
}
```

### Пользовательские callback-функции

Пример callback-функции для отображения частоты:

```c
void pwm_frequency_display_cb(menu_context_t *ctx, menu_id_t id) {
    char *value_str = menu_value_buf();
    // Форматирование значения частоты
    // Установка флага перерисовки
    menu_set_dirty();
}
```

## 🐛 Отладка

### Включение отладочной информации

Добавьте в `main.c`:

```c
#include <signal.h>
#include <execinfo.h>

void signal_handler(int sig) {
    void *array[10];
    size_t size = backtrace(array, 10);
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main() {
    signal(SIGSEGV, signal_handler);
    signal(SIGABRT, signal_handler);
    // ... остальной код
}
```

### Проверка работы меню

1. **Запустите приложение** и проверьте отображение на эмуляторе LCD
2. **Проверьте навигацию** с помощью виртуального энкодера
3. **Убедитесь**, что callback-функции вызываются корректно
4. **Проверьте обработку** различных типов данных (числа, строки, перечисления)

### Распространенные проблемы

1. **Библиотека LCD1602 не найдена**
   - Проверьте путь в `main/CMakeLists.txt`
   - Убедитесь, что библиотека собрана и установлена

2. **Меню не отображается**
   - Проверьте вызов `menu_init()`
   - Убедитесь, что callback-функции установлены

3. **Навигация не работает**
   - Проверьте обработчики событий энкодера
   - Убедитесь в правильной передаче delta в `menu_position()`

## 🔄 Обновление меню

При изменении конфигурации меню:

1. **Настройте пути выгрузки файлов ```config/files.json``` в [Menu Processor](https://github.com/GrandFatherPikhto/menu_processor)**
2. **Запустите генератор** меню из Python-проекта **Menu Processor**
3. **Пересоберите проект**
4. **Протестируйте** новое меню

---
