#include "ARHcat.h"

int main(int argc, char* argv[]) {
  int errors = 0;
  opt_t options = {0};

  errors = handler_flags(argc, argv, &options);
  if (errors == 0) {
    file_opens(argc, argv, options);
  }

  return errors;
}

int handler_flags(int argc, char* argv[], opt_t* options) {
  int errors = 0;
  const char* short_options = "benstvET";
  struct option long_optins[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                 {"show-ends", no_argument, NULL, 'E'},
                                 {"number", no_argument, NULL, 'n'},
                                 {"squeeze-blank", no_argument, NULL, 's'},
                                 {"show-tabs", no_argument, NULL, 'T'},
                                 {"show-nonprinting", no_argument, NULL, 'v'},
                                 {NULL, 0, NULL, 0}};
  int op = 0;
  while ((op = getopt_long(argc, argv, short_options, long_optins, NULL)) !=
         -1) {
    switch (op) {
      case 'b':
        options->opt_b = 1;
        options->opt_n = 0;
        break;
      case 'e':
        options->opt_e = 1;
        options->opt_v = 1;
        break;
      case 'E':
        options->opt_e = 1;
        break;
      case 'n':
        options->opt_n = options->opt_b == 1 ? 0 : 1;
        break;
      case 's':
        options->opt_s = 1;
        break;
      case 't':
        options->opt_t = 1;
        options->opt_v = 1;
        break;
      case 'v':
        options->opt_v = 1;
        break;
      default:
        errors = 1;
        break;
    }
  }
  return errors;
}

void file_opens(int argc, char* argv[], opt_t options) {
  for (int count = 1; count < argc; ++count) {
    if (argv[count][0] != '-') {
      FILE* file;
      file = fopen(argv[count], "r");
      if (file == NULL) {
        perror(argv[count]);
        continue;
      }
      file_print(options, file);
    }
  }
}

void file_print(opt_t options, FILE* file) {
  int c = fgetc(file);
  int line_counter = 1;
  int is_prev_empty = 1;
  int prev_c = '\n';
  int skip_line = 0;
  while (c != EOF) {
    print_cust_char(options, c, &line_counter, &is_prev_empty, &prev_c,
                    &skip_line);
    c = fgetc(file);
  }
}

void print_cust_char(opt_t options, int ch, int* line_counter,
                     int* is_prev_empty, int* prev_c, int* skip_line) {
  if (options.opt_s && *skip_line && ch == '\n') {
  } else {
    if (options.opt_b && *prev_c == '\n' && ch != '\n') {
      printf("%*d\t", 6, *line_counter);
      (*line_counter)++;
    }
    if (options.opt_n && *prev_c == '\n') {
      printf("%*d\t", 6, *line_counter);
      (*line_counter)++;
    }
    if (options.opt_e && ch == '\n') {
      printf("$");
    }
    if (options.opt_t && ch == '\t') {
      printf("^");
      ch = ch + 64;
    }
    if (options.opt_v) {
      if (ch > 127 && ch < 160) printf("M-^");
      if ((ch < 32 && ch != '\n' && ch != '\t') || ch == 127) printf("^");
      if ((ch < 32 || (ch > 126 && ch < 160)) && ch != '\n' && ch != '\t')
        ch = ch > 126 ? ch - 128 + 64 : ch + 64;
    }
    printf("%c", ch);
  }

  if (*is_prev_empty && ch == '\n' && options.opt_s)
    *skip_line = 1;
  else
    *skip_line = 0;
  if (ch == '\n')
    *is_prev_empty = 1;
  else
    *is_prev_empty = 0;

  *prev_c = ch;
}