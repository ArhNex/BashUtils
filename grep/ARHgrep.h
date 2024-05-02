#ifndef ARH_GREP_H
#define ARH_GREP_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct OPTION_S {
  int opt_e;
  int opt_i;
  int opt_v;
  int opt_c;
  int opt_l;
  int opt_n;
};
typedef struct OPTION_S option_t;

int handle_flags(int argc, char* argv[], option_t* ops, char* templates_ar[],
                 int* templates_count, int* more_than_one_er_fil);

int finder_files(int argc, char* argv[], char* files_address_ar[],
                 int* files_address_count, option_t ops);

int opener_files(char* files_address_ar[], int files_address_count,
                 char* templates_ar[], int templates_count, option_t ops,
                 int more_than_one_er_fil);

int string_retrieval(FILE* file, char* templates_ar[], int templates_count,
                     option_t ops, int files_address_count, char file_name[],
                     int more_than_one_er_fil);

int print_changed_output_line(char* line_b, char* templates_ar[],
                              int templates_count, option_t ops, int line_n,
                              int files_address_count, const char file_name[],
                              int* line_c, int* print_this,
                              int more_than_one_er_fil);

int print_this_line(char* line_b, char* templates_ar[], int templates_count,
                    int* print_this, int reg);

void custom_printf(const char* line_b);

#endif  // ARH_GREP_H