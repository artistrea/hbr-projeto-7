#include "include/quizz_app.h"
#include "include/ssd1306_font.h"
#include "include/menu.h"
#include "include/global.h"

#include <pico/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int guessed_correct = 0;
static int tried_guessing = 0;

void on_correct_click() {
  guessed_correct = 1;
  tried_guessing = 1;
}

void on_incorrect_click() {
  guessed_correct = 0;
  tried_guessing = 1;
}

int check_if_should_exit() {
  return tried_guessing;
}

struct menu lit_quizz_menu = {
  .n_options=4,
  .options={
    (struct menu_option){
      .name="QUEM E O AUTOR DE \"O Principe\"?",
      .on_click=0
    },
    (struct menu_option){
      .name="Maquiavel",
      .on_click=&on_correct_click
    },
    (struct menu_option){
      .name="Montesquieu",
      .on_click=&on_incorrect_click
    },
    (struct menu_option){
      .name="Thomas Hobbes",
      .on_click=&on_incorrect_click
    },
  }
};

struct menu bio_quizz_menu = {
  .n_options=4,
  .options={
    (struct menu_option){
      .name="QUAL ANIMAL NAO E DO CERRADO?",
      .on_click=0
    },
    (struct menu_option){
      .name="Lobo Guara",
      .on_click=&on_incorrect_click
    },
    (struct menu_option){
      .name="Onca Pintada",
      .on_click=&on_incorrect_click
    },
    (struct menu_option){
      .name="Leao",
      .on_click=&on_correct_click
    },
  }
};

struct menu art_quizz_menu = {
  .n_options=4,
  .options={
    (struct menu_option){
      .name="Quem pintou Guernica?",
      .on_click=0
    },
    (struct menu_option){
      .name="Pablo Picasso",
      .on_click=&on_correct_click
    },
    (struct menu_option){
      .name="Salvador Dalí",
      .on_click=&on_incorrect_click
    },
    (struct menu_option){
      .name="Tarsila do Amaral",
      .on_click=&on_incorrect_click
    },
  }
};

int try_quizz(struct render_area* frame_area, int8_t *ssd, struct vec2_int *cursor, struct menu *quizz_menu) {
  tried_guessing = 0;
  guessed_correct = 0;

  open_menu(frame_area, ssd, quizz_menu, cursor, 1, &check_if_should_exit);

  if (!tried_guessing) return 1;

  static struct menu getout_menu = {
    .n_options=2,
    .options={
      (struct menu_option){
        .name="",
        .on_click=NULL
      },
      (struct menu_option){
        .name="",
        .on_click=NULL
      }
    }
  };

  char * msg;
  if (guessed_correct) {
    getout_menu.options[0].name = "Acertaram (1):";
    getout_menu.options[1].name = "VOCE";
  } else {
    getout_menu.options[0].name = "Ninguem acertou...";
    getout_menu.options[1].name = "";
  }

  open_menu(frame_area, ssd, &getout_menu, cursor, 1, NULL);

  return 0;
}

void quizz_app_entrypoint(struct render_area* frame_area, int8_t *ssd, struct vec2_int *cursor) {
  struct menu *quizzes[] = {
    &bio_quizz_menu,
    &art_quizz_menu,
    &lit_quizz_menu
  };

  for (int i=0;i<3;i++) {
    if (try_quizz(frame_area, ssd, cursor, quizzes[i])) {
      // exited from quizz
      break;
    }
  }
}
