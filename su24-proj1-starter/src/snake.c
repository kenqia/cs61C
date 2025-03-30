#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char *argv[]) {
  bool io_stdin = false;
  char *in_filename = NULL;
  char *out_filename = NULL;
  game_state_t *state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  if (in_filename != NULL) {
    FILE *fp = fopen(in_filename , "r");
    if(fp == NULL) return -1;
    state = load_board(fp);
    state = initialize_snakes(state);
    fclose(fp);
  } else if (io_stdin) {
  } else {
    state = create_default_state();
  }
  update_state(state , deterministic_food);
  if (out_filename != NULL) {
    save_board(state , out_filename);
  } else {
    for(int i = 0 ; i < state->num_rows ; i++){
      printf("%s\n" , state->board[i]);
    }
  }

  free_state(state);

  return 0;
}
