# Список доделок

## ✅ Что уже хорошо:
- Четкое разделение на модули (engine, navigate, draw, edit)
- Консистентные имена функций (`module_action_object`)
- Правильное разделение dirty/invalidate для двухэтапной отрисовки
- Логичная структура контекста

## 🔧 Что можно улучшить:

### 1. **Критическая ошибка в `menu_values.c`**
```c
// Сейчас (ОШИБКА!)
menu_node_value_t *menu_values_get_nodes(menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return 0;
    return &(s_menu_node_values[MENU_ID_COUNT]); // ❌ Всегда возвращает последний элемент!
}

// Должно быть:
menu_node_value_t *menu_values_get_node(menu_id_t id) {
    if (id >= MENU_ID_COUNT)
        return NULL;
    return &s_menu_node_values[id]; // ✅ Правильный индекс
}
```

### 2. **Упрощение публичного API (как ты и хотел)**
Создай `menu.h` с чистым API:

```c
// menu.h
#ifndef MENU_H
#define MENU_H

#include <stdint.h>

// Инициализация
void menu_init(void);

// Публичное API для внешнего мира
void menu_handle_position(int8_t delta);
void menu_handle_enter(void);
void menu_handle_back(void);
void menu_update(void);

// Для отрисовки (можно вынести в отдельный menu_display.h)
const char* menu_get_display_title(void);
const char* menu_get_display_value(void);
bool menu_needs_redraw(void);
void menu_ack_redraw(void);

#endif
```

### 3. **Убери дублирование в menu_context.h**
```c
// Сейчас (дублирование):
menu_context_t *menu_get_context(void); // Объявлено дважды

// Убери одно объявление
```

### 4. **Оптимизация отрисовки!**
Ты правильно разделил:
- **dirty** = данные изменились, нужно пересчитать строки
- **invalidate** = строки готовы, нужно перерисовать дисплей

Оставь как есть - это профессиональное решение.

### 5. **Улучши названия в menu_draw.c**
```c
// Было: 
void menu_draw_string_fixed_value_cb(menu_context_t *ctx, menu_id_t id);

// Стало (уберем _cb, т.к. это не колбэк, а функция отрисовки):
void menu_draw_string_fixed_value(menu_context_t *ctx, menu_id_t id);
```

### 6. **menu_common.h - вынеси типы в отдельный файл**
Создай `menu_types.h` для чистых типов данных:

```c
// menu_types.h
#ifndef MENU_TYPES_H
#define MENU_TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MENU_ID_ROOT,
    MENU_ID_START,
    // ... все ID
    MENU_ID_COUNT
} menu_id_t;

typedef enum {
    MENU_STATE_NAVIGATION = 1,
    MENU_STATE_EDIT = 2
} menu_state_t;

// Forward declarations
typedef struct menu_context menu_context_t;
typedef struct menu_node menu_node_t;
typedef struct menu_node_config menu_node_config_t;
typedef struct menu_node_value menu_node_value_t;

#endif
```

### 7. **Главный цикл можно упростить**
```c
// main.c
while (lcd1602_sdl_next_tick(lcd)) {
    menu_update(); // Обрабатывает dirty -> готовит строки -> устанавливает invalidate
    
    if (menu_needs_redraw()) {
        lcd_sdl_clear(lcd);
        lcd_sdl_print_str(lcd, menu_get_display_title());
        lcd_sdl_print_str(lcd, menu_get_display_value());
        menu_ack_redraw();
    }
    
    usleep(10000);
}
```

### 8. **Финалная структура модулей:**
```
menu.h              // Публичное API
menu_private.h      // Для внутреннего использования модулями
menu_types.h        // Чистые типы данных
menu_context.h/c    // Контекст и управление им
menu_engine.h/c     // Движок (state machine)
menu_navigate.h/c   // Публичные функции навигации  
menu_draw.h/c       // Отрисовка значений
menu_edit.h/c       // Функции редактирования
menu_tree.h/c       // Структура меню (генерируется)
menu_config.h/c     // Конфигурация (генерируется)
menu_values.h/c     // Значения (генерируется)
```

## 🎯 Итоговые рекомендации:

1. **Исправь критический баг** в `menu_values.c`
2. **Создай чистый публичный API** в `menu.h` 
3. **Вынеси типы** в `menu_types.h`
4. **Убери дублирование** объявлений
5. **Оставь архитектуру отрисовки** как есть - она идеальна для embedded

## Список
1. Исправление ошибки в menu_values.c: В функции menu_values_get_nodes (которая, судя по имени, должна возвращать массив значений) возвращается один элемент, но индекс указан неверно. Кроме того, имя функции не соответствует тому, что она возвращает один элемент. Лучше переименовать ее в menu_values_get_node (в единственном числе) и исправить индекс.
2. Убрать дублирование функции menu_get_context: В menu_context.h объявлена дважды? Нужно оставить одно объявление.
3. Упрощение отрисовки: Вы предложили разделить dirty и invalidate. Это хорошая идея, но давайте проверим, как это работает. Сейчас в menu_navigate_update мы вызываем menu_draw_update, которая устанавливает invalidate и сбрасывает dirty. Затем в главном цикле проверяем invalidate и вызываем menu_draw (которая рисует на LCD). Однако сейчас menu_draw не использует контекст, а использует глобальные функции для получения строк. Это может быть проблемой, если мы захотим иметь несколько меню. Лучше передавать контекст в menu_draw.
4. Названия функций в menu_navigate.h: Они теперь начинаются с menu_navigate_, что хорошо. Но давайте проверим, нет ли путаницы между menu_navigate_out и menu_engine_handle_out. В menu_navigate_out мы вызываем menu_engine_handle_out. Это нормально, но нужно быть последовательным.
5. В menu_engine.c в функции menu_engine_handle_enter есть проверка if (config == 0 || node == 0). Но мы получаем указатели на элементы массива, они не могут быть NULL, если индекс корректен. Проверка на индекс уже есть в начале функции. Можно убрать эти проверки.
6. В menu_draw.c в функции menu_draw_update мы проверяем ctx->dirty и выходим, если false. Но затем мы сбрасываем dirty в false после отрисовки. Однако эта функция вызывается из menu_navigate_update, которая уже проверяет dirty. Возможно, дублирование проверки не нужно.
7. В menu_context.c функция menu_context_init возвращает const menu_context_t *, но мы можем захотеть изменять контекст. Лучше возвращать указатель без const.
8. В menu_common.h мы объявили menu_items как static, что ограничивает его видимость одним файлом. Если он нужен в других файлах, нужно убрать static и объявить extern.
9. В menu_engine.h название файла guard не соответствует имени файла. Вместо MENU_H должно быть MENU_ENGINE_H.
10. В menu_navigate.c мы включаем menu_engine.h и menu_draw.h, но не используем их напрямую. Нужно проверить, что все необходимые заголовки включены.
11. В main.c мы определяем функцию menu_draw, которая зависит от глобального контекста. Лучше сделать ее независимой, передавая контекст.
12. В menu_config.c мы используем функции из menu_edit.h и menu_draw.h, но не включаем их. Нужно включить.
13. В menu_edit.c мы включаем menu_engine.h, но не используем его. Можно убрать.
14. В menu_engine.c мы включаем menu_draw.h, но не используем его. Можно убрать.
15. В menu_draw.c мы включаем menu_common.h, menu_context.h, menu_config.h, menu_values.h, menu_tree.h. Но в menu_draw.h мы уже включаем menu_common.h. Нужно проверить, что все зависимости удовлетворены.
16. В menu_context.h мы объявили menu_context_t с полем event, но не используем его. Если не используется, можно удалить.
17. В menu_engine.c в функции menu_engine_handle_enter и menu_engine_handle_out мы проверяем, что node->child или node->parent не равны MENU_ID_COUNT. Это правильно, но можно добавить проверку, что мы не выходим за пределы дерева.
18. В menu_engine.c в функции menu_engine_get_sibling мы проходим по списку siblings. Но если шагов больше, чем элементов, мы можем зациклиться? Нет, потому что мы останавливаемся, когда next_id == MENU_ID_COUNT. Это правильно.
19. В menu_engine.c в функции menu_engine_handle_position мы вызываем menu_engine_get_sibling и затем menu_engine_go_to. Но если menu_engine_get_sibling вернет MENU_ID_COUNT, то menu_engine_go_to не выполнит переход. Это правильно.
20. В menu_navigate.c функции menu_navigate_enter, menu_navigate_out и menu_navigate_position вызывают функции двигателя, передавая текущий ID из контекста. Это правильно.
21. В menu_draw.c функции отрисовки значения (например, menu_draw_string_fixed_value_cb) печатают в буфер значения и добавляют индикатор режима ('>' или '*'). Это хорошо.
22. В main.c мы используем menu_context_get_title_str и menu_context_get_value_str для получения строк для отрисовки. Но эти строки обновляются в menu_draw_update. Это правильно.
23. В menu_draw.c в функции menu_draw_update мы не проверяем, что node и config не NULL. Но мы уже проверили, что id в пределах, и массивы nodes и configs инициализированы. Можно добавить assert для отладки.
24. В menu_context.c мы инициализируем контекст как static. Это нормально для одного меню.
25. В menu_config.c мы определяем массивы факторов и строк, а затем массив конфигов. Это правильно.
26. В menu_values.c мы определяем массив значений. Обратите внимание, что для некоторых пунктов меню (например, MENU_ID_VERSION) мы используем тип callback_callback_value_t, но у нас нет обработчика для callback. Нужно быть осторожным, чтобы не обратиться к этому значению.
27. В menu_edit.c мы изменяем значения и устанавливаем dirty в true. Это правильно.
28. В menu_navigate.c в функции menu_navigate_update мы сбрасываем dirty после вызова menu_draw_update. Это правильно.
29. В main.c мы вызываем menu_navigate_update в главном цикле, а затем проверяем invalidate и рисуем. Но menu_navigate_update уже вызвал menu_draw_update, которая установила invalidate и сбросила dirty. Затем мы рисуем. Это правильно.
30. В menu_draw.c в функции menu_draw_update мы сбрасываем dirty и устанавливаем invalidate. Но invalidate устанавливается в true только если была отрисовка. А отрисовка происходит только если dirty был true. Это правильно.

