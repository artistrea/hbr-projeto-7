#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>
#include <pico/time.h>

// variables to check if interrupt fired in main routine
extern int next_tick;

extern bool has_unattended_button_click;

struct button_task {
  int id;
  void (*func);
  struct button_task *prev;
  struct button_task *next;
};

#define BUTTON_B 6

void gpio_isr(uint gpio, uint32_t events);

bool update_next_tick(repeating_timer_t *rt);

#endif // GLOBAL_H_
