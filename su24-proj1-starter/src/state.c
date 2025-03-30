#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);
static void create_one_fruit_1(game_state_t *state);
static void create_one_fruit_2(game_state_t *state);
static void create_snakes(game_state_t *state);
static void create_board(game_state_t *state);
/*hard code the board*/
#define LENGTH 18
#define WIDTH 20
#define SNKNUM 1


int random_in_range(int a, int b) {
  return a + rand() % (b - a + 1);
}
/* Task 1 */
game_state_t *create_default_state() {
  game_state_t *state = malloc(sizeof(game_state_t));
  state->num_rows = LENGTH;
  create_board(state);
  state->num_snakes = SNKNUM;
  create_snakes(state);
  create_one_fruit_1(state);
  return state;
}

void create_board(game_state_t *state){
  state->board = malloc(LENGTH * sizeof(char*));
  char *edge , *mid;
  edge = malloc((WIDTH + 1)* sizeof(char));
  mid = malloc((WIDTH + 1 )* sizeof(char));
  for(int i = 0 ; i < WIDTH; i++){
    edge[i] = '#';
    if(i == 0 || i == WIDTH - 1) mid[i] = '#';
    else mid[i] = ' ';
  }
  mid[WIDTH] = edge[WIDTH] = '\0';
  for(int i = 0 ; i < LENGTH ; i++){
    state->board[i] = malloc((WIDTH + 1 ) * sizeof(char));
    if(i == 0 || i == LENGTH - 1) strcpy(state->board[i] , edge);
    else strcpy(state->board[i] , mid);
  }
  free(edge);
  free(mid);
}

void create_snakes(game_state_t *state){
  state->snakes = malloc(SNKNUM * sizeof(snake_t));  
  if(SNKNUM == 1){
    state->snakes[0].tail_row = 2;
    state->snakes[0].tail_col = 2;
    state->snakes[0].head_row = 2;
    state->snakes[0].head_col = 4;
    state->snakes[0].live = true;
    set_board_at(state , 2 , 2 , 'd');
    set_board_at(state , 2 , 3 , '>');
    set_board_at(state , 2 , 4 , 'D');
  }
}

void create_one_fruit_1(game_state_t *state){
  state->board[2][9] = '*';
  // int row , col;
  // while(1){
  //   row = random_in_range(1 , LENGTH - 1);
  //   col = random_in_range(1 , WIDTH - 1);
  //   if(state->board[row][col] == ' ') break;
  // }
  // state->board[row][col] = '*';
}

void create_one_fruit_2(game_state_t *state){
  int row , col;
  while(1){
    row = random_in_range(1 , LENGTH - 1);
    col = random_in_range(1 , WIDTH - 1);
    if(state->board[row][col] == ' ') break;
  }
  state->board[row][col] = '*';
}
/* Task 2 */
void free_state(game_state_t *state) {
  for(int i = 0 ; i < state->num_rows ; i++){
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for(int i = 0 ; i < state->num_rows ; i ++){
    fprintf(fp ,"%s\n" , state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  switch(c){
    case 'w':
    case 'a':
    case 's':
    case 'd':return true;break;
    default: return false;
  }  
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/ 
static bool is_head(char c) {
  switch(c){
    case 'W':
    case 'A':
    case 'S':
    case 'D':
    case 'x':return true; break;
    default: return false;
  }  
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  switch(c){
    case 'W':
    case 'A':
    case 'S':
    case 'D':
    case 'w':
    case 'a':
    case 's':
    case 'd':
    case '^':
    case 'v':
    case '<':
    case '>':
    case 'x':return true; break;
    default: return false;
  }
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch(c){
    case '^':return 'w';
    case 'v':return 's';
    case '<':return 'a';
    case '>':return 'd';
    default: return 0;
  }  
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  switch(c){
    case 'W':return '^';
    case 'A':return '<';
    case 'S':return 'v';
    case 'D':return '>';
    default: return 0;
  }  
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  switch(c){
    case 'v':case 's':case 'S':return cur_row + 1;
    case '^':case 'w':case 'W':return cur_row - 1;
    default: return cur_row;
  }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  switch(c){
    case '>':case 'd':case 'D':return cur_col + 1;
    case '<':case 'a':case 'A':return cur_col - 1;
    default: return cur_col;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  unsigned int row = state->snakes[snum].head_row;
  unsigned int col = state->snakes[snum].head_col;

  char head = get_board_at(state , row , col);
  return get_board_at(state , get_next_row(row , head) , get_next_col(col , head));
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  unsigned int row = state->snakes[snum].head_row;
  unsigned int col = state->snakes[snum].head_col;
  char now = get_board_at(state , row , col);
  state->snakes[snum].head_row = get_next_row(row , now); 
  state->snakes[snum].head_col = get_next_col(col , now);
  set_board_at(state , state->snakes[snum].head_row , state->snakes[snum].head_col , now);
  set_board_at(state , row , col , head_to_body(get_board_at(state,row,col)));
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  unsigned int row = state->snakes[snum].tail_row;
  unsigned int col = state->snakes[snum].tail_col;
  char now = get_board_at(state , row , col);
  state->snakes[snum].tail_row = get_next_row(row , now); 
  state->snakes[snum].tail_col = get_next_col(col , now);
  char next = get_board_at(state , state->snakes[snum].tail_row , state->snakes[snum].tail_col);
  set_board_at(state , state->snakes[snum].tail_row , state->snakes[snum].tail_col , body_to_tail(next));
  set_board_at(state , row , col , ' ');
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  for(unsigned int i = 0 ; i < state->num_snakes ; i++){
    char next = next_square(state , i);
    if(is_snake(next) || next == '#'){
      state->snakes[i].live = false;
      set_board_at(state , state->snakes[i].head_row , state->snakes[i].head_col , 'x');
      continue;
    }
    update_head(state , i);
    if(next == '*'){
      add_food(state);
      continue;
    }
    update_tail(state , i);
  }
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
