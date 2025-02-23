#include "include/utils.h"
#include "include/cursor.h"
#include "include/ssd1306.h"
#include "hardware/adc.h"

struct vec2_int get_joystick_pos() {
  adc_select_input(0);
  // convert from uint to int so that we may have negative
  int adc_y_raw = adc_read();
  adc_select_input(1);
  int adc_x_raw = adc_read();

  // const uint adc_max = (1 << 12) - 1;

  return (struct vec2_int){
    .x = adc_x_raw - ADC_MAX/2,
    .y = adc_y_raw - ADC_MAX/2
  };
}

// speed may go from 0 to 3
struct vec2_int get_speed_from_input(struct vec2_int input, int max) {
  // if max == 3, then max/4 == lim
  // if < lim, 0; if < 2*lim, 1; if < 3*lim, 2; else 3
  int lim = max/4;

  return (struct vec2_int){ .x=input.x/lim, .y=input.y/lim };
}

// updates cursor in place
void update_cursor_pos(struct vec2_int *cursor, int dt_ms) {
  struct vec2_int v = get_joystick_pos();

  v = get_speed_from_input(v, ADC_MAX/2);

  cursor->x += (v.x * CURSOR_SPEED * dt_ms)/1000;
  cursor->y -= (v.y * CURSOR_SPEED * dt_ms)/1000;

  if (cursor->x < 0) cursor->x = 0;
  if (cursor->y < 0) cursor->y = 0;

  if (cursor->x >= ssd1306_width) cursor->x = ssd1306_width-1;
  if (cursor->y >= ssd1306_height) cursor->y = ssd1306_height-1;
}

