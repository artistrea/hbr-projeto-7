#include "include/utils.h"
#include "include/menu.h"
#include "include/quizz_app.h"
#include "include/fractions_app.h"
#include "include/cursor.h"
#include "include/global.h"

#include "hardware/gpio.h"
#include <pico/time.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

struct menu main_menu = {
  .n_options=2,
  .options={
    (struct menu_option){
      .on_click=&quizz_app_entrypoint,
      .name="QUIZZ"
    },
    (struct menu_option){
      .on_click=&fractions_app_entrypoint,
      .name="FRACOES"
    }
  }
};

struct vec2_int cursor = {0, 0};

int main() {
  stdio_init_all();
  adc_init();

  // init button B
  gpio_init(BUTTON_B);
  gpio_set_dir(BUTTON_B, GPIO_IN);
  gpio_pull_up(BUTTON_B);

  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, 1, &gpio_isr);

  // init joystick
  adc_gpio_init(26);
  adc_gpio_init(27);

  // init oled ssd1306
  i2c_init(I2C_PORT, ssd1306_i2c_clock*1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  struct render_area frame_area = {
      .start_column=0,
      .end_column=ssd1306_width - 1,
      .start_page=0,
      .end_page=ssd1306_n_pages - 1
  };

  calculate_render_area_buffer_length(&frame_area);

  // display buffer
  uint8_t ssd[ssd1306_buffer_length];
  memset(ssd, 0, ssd1306_buffer_length);

  // init component sending the i2c commands
  ssd1306_init();

  repeating_timer_t timer;

  // set frame rate
  // 20fps means 50ms between frame
  // negative value means interval between calls, ignoring execution time of task
  add_repeating_timer_ms(50, update_next_tick, NULL, &timer);

  absolute_time_t now = get_absolute_time();
  int64_t prev_time_ms;
  prev_time_ms = to_ms_since_boot(now);
  int64_t time_ms;

  open_menu(&frame_area, ssd, &main_menu, &cursor, 0, NULL);
}
