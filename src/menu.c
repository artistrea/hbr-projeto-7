#include "include/ssd1306.h"
#include "include/ssd1306_font.h"
#include "include/menu.h"
#include "include/cursor.h"
#include "include/global.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


void render_menu(uint8_t *ssd, const struct vec2_int *cursor_pos, const struct menu* menu, int *selected_menu_item) {
  int padding = 3;
  int spacing = FONT_HEIGHT + padding; // char height
  int start_render = (ssd1306_height - spacing * menu->n_options)/2;
  *selected_menu_item = -1;

  for (char i=0;i<menu->n_options;i++) {
    int str_size = strlen(menu->options[i].name);

    int padding_left = (ssd1306_width - (FONT_WIDTH+1)*str_size)/2;
    if (padding_left < padding) padding_left = padding;
    int bot_y = start_render + spacing * i;
    // if (str_size == )
    int top_y = bot_y + spacing;

    if (menu->options[i].on_click && cursor_pos->y >= bot_y && cursor_pos->y < top_y) {
      // selector may appear if option is clickable
      *selected_menu_item = i;
      ssd1306_draw_line(
        ssd,
        padding_left-FONT_HEIGHT,
        bot_y + padding/2,
        padding_left - FONT_HEIGHT/2,
        bot_y + FONT_HEIGHT/2 + padding/2,
        1
      );
      ssd1306_draw_line(
        ssd,
        padding_left-FONT_HEIGHT,
        bot_y + FONT_HEIGHT + padding/2,
        padding_left - FONT_HEIGHT/2,
        bot_y + FONT_HEIGHT/2 + padding/2,
        1
      );
    }

    ssd1306_draw_string(
      ssd,
      padding_left,
      bot_y + padding/2,
      menu->options[i].name
    );
  }
}

void open_menu(
  struct render_area* frame_area,
  int8_t *ssd,
  struct menu* menu,
  struct vec2_int *cursor,
  int can_exit,
  int (*should_exit)()
) {
  int closed=0;

  absolute_time_t now = get_absolute_time();
  int64_t prev_time_ms;
  prev_time_ms = to_ms_since_boot(now);
  int64_t time_ms;

  int selected_menu_item=0;
  int i=0;
  while (!closed) {
    if (next_tick) {
      if (should_exit && should_exit()) {
        closed=1;
      }
      next_tick = 0;
      // reset buffer
      memset(ssd, 0, ssd1306_buffer_length);
      if (can_exit) {
        ssd1306_draw_line(ssd, 0, 5, 5, 0, 1);
        ssd1306_draw_line(ssd, 0, 5, 5, 10, 1);
        ssd1306_draw_line(ssd, 0, 5, 10, 5, 1);
      }

      render_menu(ssd, cursor, menu, &selected_menu_item);
      time_ms = to_ms_since_boot(get_absolute_time());
      int64_t dt = time_ms - prev_time_ms;
      prev_time_ms = time_ms;
      update_cursor_pos(cursor, dt);
      ssd1306_print_cursor(ssd, cursor->x, cursor->y);

      // send buffer
      render_on_display(ssd, frame_area);
    }
    if (has_unattended_button_click) {
      has_unattended_button_click = false;
      if (can_exit && cursor->x <= 10 && cursor->y <= 10) {
        closed = 1;
      }

      if (selected_menu_item != -1 &&  menu->options[selected_menu_item].on_click) {
        menu->options[selected_menu_item].on_click(frame_area, ssd, cursor);
      }
    }
    sleep_ms(4);
  }
}

