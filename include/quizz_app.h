#ifndef QUIZZ_APP_H_
#define QUIZZ_APP_H_

#include "include/ssd1306.h"
#include "include/utils.h"

#include <stdint.h>

void quizz_app_entrypoint(struct render_area* frame_area, int8_t *ssd, struct vec2_int *cursor);

#endif // QUIZZ_APP_H_
