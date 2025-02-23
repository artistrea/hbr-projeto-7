#include "include/global.h"
#include <pico/time.h>
#include <stdio.h>
#include "hardware/gpio.h"

int next_tick=0;

// ISR for timer
// could use systick, like RTOS does, but that's kinda overkill
bool update_next_tick(repeating_timer_t *rt) {
  next_tick = 1;

  return 1;
}

// ISR for button
bool has_unattended_button_click = 0;

int64_t reactivate_gpio_interrupt(__unused alarm_id_t id, __unused void *user_data) {
  // printf("timer fired\n");
  gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, 1);

  return 0;
}

void gpio_isr(uint gpio, uint32_t events) {
  if (gpio != BUTTON_B || !(events & GPIO_IRQ_EDGE_FALL)) return;

  // debounce
  gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL, 0);
  add_alarm_in_ms(100, &reactivate_gpio_interrupt, NULL, false);

  has_unattended_button_click = 1;
}

