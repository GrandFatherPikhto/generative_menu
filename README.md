# LCD1602 Simulator

Виртуальный симулятор LCD-дисплея 16x2 на базе SDL2, предназначенный для разработки и тестирования ПО без реального оборудования.

## Особенности

- 🖥️ Точная визуализация LCD-дисплея 16x2 символов
- ⌨️ Управление через клавиатуру (эмуляция энкодера и кнопок)
- 🔄 Поддержка callback-функций для обработки событий
- 🎨 Реалистичное отображение с зеленой LCD-подсветкой
- 💡 Подсказки по управлению на интерфейсе
- 🚀 Кроссплатформенность (Windows/Linux/macOS)

## Структура проекта

```
lcd1602sdl02sym/
├── cmake/
│   ├── LCD1602Config.cmake      # Конфигурация пакета для CMake
│   └── lcd1602-config.cmake     # Альтернативная конфигурация
├── lcd1602/                     # Библиотека симулятора
│   ├── CMakeLists.txt
│   └── src/
│       ├── include/
│       │   ├── lcd1602_sdl.h        # Основной заголовочный файл
│       │   ├── lcd1602_sdl_types.h  # Определения типов
│       │   └── utils.h              # Вспомогательные функции
│       ├── lcd1602_sdl.c            # Реализация симулятора
│       └── utils.c                  # Реализация утилит
├── main/                         # Пример использования
│   ├── CMakeLists.txt
│   └── src/
│       ├── include/
│       └── main.c                   # Демонстрационная программа
├── CMakeLists.txt               # Корневой CMake
└── README.md
```

## Сборка проекта

### Зависимости

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev libsdl2-ttf-dev cmake build-essential
```

**Windows (MinGW):**
```bash
# Установите SDL2 и SDL2_ttf через vcpkg или скачайте библиотеки вручную
```

**macOS:**
```bash
brew install sdl2 sdl2_ttf cmake
```

### Компиляция

```bash
# Клонирование репозитория
git clone <repository-url>
cd lcd1602sdl02sym

# Создание build директории
mkdir build && cd build

# Конфигурация и сборка
cmake ..
make

# Запуск примера
./main/menu
```

## Использование в вашем проекте

### Способ 1: Подключение как CMake пакета

Добавьте в ваш `CMakeLists.txt`:

```cmake
# Укажите путь к конфигурационным файлам LCD1602
set(LCD1602_DIR "/path/to/lcd1602sdl02sym/cmake")

# Проверка существования конфигурационного файла
if(NOT EXISTS "${LCD1602_DIR}/LCD1602Config.cmake")
    message(FATAL_ERROR "LCD1602Config.cmake not found at: ${LCD1602_DIR}")
endif()

# Поиск пакета
find_package(LCD1602 REQUIRED)

# Создание целевого исполняемого файла
add_executable(your_target main.c)

# Подключение библиотеки LCD1602
target_link_libraries(your_target LCD1602::LCD1602)
```

### Способ 2: Подключение как поддиректории

```cmake
# Добавление библиотеки как подпроекта
add_subdirectory(lcd1602sdl02sym)

# Создание целевого исполняемого файла
add_executable(your_target main.c)

# Подключение библиотеки LCD1602
target_link_libraries(your_target LCD1602::LCD1602)
```

## Базовый пример использования

```c
#include "lcd1602_sdl.h"
#include <unistd.h>

// Callback-функции
void on_position_change(int new_pos) {
    printf("Position: %d\n", new_pos);
}

void on_button_push() {
    printf("Button pressed\n");
}

void on_long_push() {
    printf("Long button push\n");
}

void on_double_click() {
    printf("Double click\n");
}

int main() {
    // Создание симулятора
    sdl_handle_t *lcd = lcd1602_sdl_create("My LCD Display", 360, 150);
    if (!lcd) return -1;

    // Настройка callback-ов
    lcd1602_sdl_set_position_cb(lcd, on_position_change);
    lcd1602_sdl_set_push_button_cb(lcd, on_button_push);
    lcd1602_sdl_set_long_push_button_cb(lcd, on_long_push);
    lcd1602_sdl_set_double_click_cb(lcd, on_double_click);

    // Главный цикл
    while (lcd1602_sdl_next_tick(lcd)) {
        // Ваша логика здесь
        lcd_sdl_clear(lcd);
        lcd_sdl_set_cursor(lcd, 0, 0);
        lcd_sdl_print_str(lcd, "Hello World!");
        lcd_sdl_set_cursor(lcd, 0, 1);
        lcd_sdl_print_str(lcd, "Position: 123");
        
        usleep(10000); // 10ms
    }

    // Освобождение ресурсов
    lcd1602_sdl_release(lcd);
    return 0;
}
```

## Управление

| Клавиша | Действие |
|---------|----------|
| `↑`/`↓` | Эмуляция энкодера (изменение позиции) |
| `Enter` | Короткое нажатие кнопки |
| `L` | Длинное нажатие кнопки |
| `D` | Двойное нажатие кнопки |
| `Q` | Выход из программы |

*Подсказка по управлению отображается в интерфейсе симулятора*

## API Reference

### Основные функции

```c
// Создание и уничтожение
sdl_handle_t* lcd1602_sdl_create(const char* title, int width, int height);
void lcd1602_sdl_release(sdl_handle_t* handle);

// Главный цикл
bool lcd1602_sdl_next_tick(sdl_handle_t* handle);

// Управление дисплеем
void lcd_sdl_clear(sdl_handle_t* handle);
void lcd_sdl_set_cursor(sdl_handle_t* handle, int x, int y);
void lcd_sdl_print_char(sdl_handle_t* handle, char ch);
void lcd_sdl_print_str(sdl_handle_t* handle, const char* str);

// Настройка callback-ов
void lcd1602_sdl_set_position_cb(sdl_handle_t* handle, void (*cb)(int));
void lcd1602_sdl_set_push_button_cb(sdl_handle_t* handle, void (*cb)(void));
void lcd1602_sdl_set_long_push_button_cb(sdl_handle_t* handle, void (*cb)(void));
void lcd1602_sdl_set_double_click_cb(sdl_handle_t* handle, void (*cb)(void));

// Настройка параметров
void lcd1602_sdl_set_debounce(sdl_handle_t* handle, uint8_t debounce);
```

### Callback-функции

- **Position Callback**: Вызывается при изменении позиции энкодера
- **Push Button Callback**: Короткое нажатие кнопки
- **Long Push Callback**: Длинное нажатие (>1 сек)
- **Double Click Callback**: Двойное нажатие

## Отладка

Проект включает обработчик сигналов для отладки segmentation faults. При падении программы выводится backtrace для идентификации проблемы.

```bash
# Для расшифровки backtrace используйте:
addr2line -e ./your_program <address>
```

## Лицензия

Этот проект распространяется под лицензией MIT. Подробнее см. [LICENSE](LICENSE).

## Вклад в проект

Приветствуются pull requests и issue reports для улучшения функциональности и исправления ошибок.
```

## Дополнительно рекомендую создать:

### Файл `lcd1602/CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.12)
project(lcd1602 VERSION 1.0.0 LANGUAGES C)

# Настройка стандарта C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Поиск зависимостей
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Создание библиотеки
add_library(lcd1602
    src/lcd1602_sdl.c
    src/utils.c
)

# Настройка include директорий
target_include_directories(lcd1602 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${SDL2_INCLUDE_DIRS}
)

# Линковка зависимостей
target_link_libraries(lcd1602
    SDL2::SDL2 
    SDL2_ttf::SDL2_ttf
)

# Установка для использования другими проектами
install(TARGETS lcd1602
    EXPORT lcd1602-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY src/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT lcd1602-targets
    FILE lcd1602-targets.cmake
    NAMESPACE LCD1602::
    DESTINATION lib/cmake/lcd1602
)
```
