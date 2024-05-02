#ifndef ARH_CAT_H
#define ARH_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options_s {
  int opt_b;
  int opt_e;
  int opt_n;
  int opt_s;
  int opt_t;
  int opt_v;
};

typedef struct options_s opt_t;

int handler_flags(int argc, char* argv[], opt_t* options);
void file_opens(int argc, char* argv[], opt_t options);
void file_print(opt_t options, FILE* file);
void print_cust_char(opt_t options, int ch, int* line_counter,
                     int* is_prev_empty, int* prev_c, int* skip_line);

#endif  // ARH_CAT_H