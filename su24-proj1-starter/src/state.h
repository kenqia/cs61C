#ifndef _SNK_STATE_H
#define _SNK_STATE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct snake_t {
  unsigned int tail_row; //The row of the snake's tail.
  unsigned int tail_col; //The column of the snake's tail.
  unsigned int head_row; //The row of the snake's head.
  unsigned int head_col; //The column of the snake's head.

  bool live; //true if the snake is alive, and false if the snake is dead.
} snake_t;

typedef struct game_state_t {
  unsigned int num_rows; 
  char **board;

  unsigned int num_snakes;
  snake_t *snakes;
} game_state_t;

game_state_t *create_default_state();
void free_state(game_state_t *state);
void print_board(game_state_t *state, FILE *fp);
void save_board(game_state_t *state, char *filename);
void update_state(game_state_t *state, int (*add_food)(game_state_t *state));
char get_board_at(game_state_t *state, unsigned int row, unsigned int col);
game_state_t *initialize_snakes(game_state_t *state);
game_state_t *load_board(FILE *fp);

#endif
