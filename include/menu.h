#ifndef MENU_H_
#define MENU_H_

#include "include/ssd1306.h"
#include "include/utils.h"
#include <stdint.h>

struct menu_option {
  void (*on_click)(struct render_area* frame_area, int8_t *ssd, struct vec2_int *cursor);
  char* name;
};

struct menu {
  char n_options;
  // since we should initialize all menus statically, this is ok
  struct menu_option options[];
};

void open_menu(
  struct render_area* frame_area,
  int8_t *ssd,
  struct menu* menu,
  struct vec2_int *cursor,
  int can_exit,
  int (*should_exit)()
);

#endif // MENU_H_
