#include "ARHgrep.h"

int main(int argc, char* argv[]) {
  option_t ops = {0};
  char* templates_ar[100] = {NULL};
  int templates_count = 0;
  char* files_address_ar[100] = {NULL};
  int files_address_count = 0;
  int more_than_one_er_fil = 0;
  handle_flags(argc, argv, &ops, templates_ar, &templates_count,
               &more_than_one_er_fil);
  finder_files(argc, argv, files_address_ar, &files_address_count, ops);
  opener_files(files_address_ar, files_address_count, templates_ar,
               templates_count, ops, more_than_one_er_fil);

  return 0;
}

int handle_flags(int argc, char* argv[], option_t* ops, char* templates_ar[],
                 int* templates_count, int* more_than_one_er_fil) {
  const char* options_list = "e:ivcln";
  int error = 0;
  int c = 0;
  while ((c = getopt_long(argc, argv, options_list, NULL, NULL)) != -1) {
    switch (c) {
      case 'e':
        ops->opt_e = 1;
        templates_ar[(*templates_count)++] = optarg;
        break;
      case 'i':
        ops->opt_i = 1;
        break;
      case 'v':
        ops->opt_v = 1;
        break;
      case 'c':
        ops->opt_c = 1;
        break;
      case 'l':
        ops->opt_l = 1;
        break;
      case 'n':
        ops->opt_n = 1;
        break;
      case '?':
        if (optopt == 'e') {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
          error = 1;
        } else {
          *more_than_one_er_fil = 1;
        }
        break;
    }
  }
  if (!ops->opt_e) {
    for (int count = 1; count < argc; ++count) {
      if (argv[count][0] != '-') {
        templates_ar[(*templates_count)++] = argv[count];
        count = argc;
      }
    }
  }
  return error;
}

int finder_files(int argc, char* argv[], char* files_address_ar[],
                 int* files_address_count, option_t ops) {
  int errors = 0;
  int is_first = 1;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-e") == 0) {
      ++i;
      continue;
    }
    if (argv[i][0] != '-') {
      if (!ops.opt_e && (i == 1 || is_first)) {
        is_first = 0;
        continue;
      } else {
        files_address_ar[(*files_address_count)++] = argv[i];
      }
    }
  }
  if (*files_address_count == 0) {
    errors = 1;
  }
  return errors;
}

int opener_files(char* files_address_ar[], int files_address_count,
                 char* templates_ar[], int templates_count, option_t ops,
                 int more_than_one_er_fil) {
  for (int i = 0; i < files_address_count; ++i) {
    FILE* file;
    file = fopen(files_address_ar[i], "r");
    if (file == NULL) {
      perror(files_address_ar[i]);
      continue;
    }
    string_retrieval(file, templates_ar, templates_count, ops,
                     files_address_count, files_address_ar[i],
                     more_than_one_er_fil);
    fclose(file);
  }

  return 0;
}

int string_retrieval(FILE* file, char* templates_ar[], int templates_count,
                     option_t ops, int files_address_count, char file_name[],
                     int more_than_one_er_fil) {
  char* line_b = NULL;
  size_t b_size = 0;
  long long int len_line = getline(&line_b, &b_size, file);
  //
  int line_n = 1;
  int line_c = 0;
  int print_this = 0;
  while (len_line >= 0) {
    print_changed_output_line(line_b, templates_ar, templates_count, ops,
                              line_n, files_address_count, file_name, &line_c,
                              &print_this, more_than_one_er_fil);
    len_line = getline(&line_b, &b_size, file);
    line_n++;
  }
  free(line_b);
  if (ops.opt_c && !ops.opt_l) {
    if (files_address_count > 1 || more_than_one_er_fil) {
      printf("%s:", file_name);
    }
    printf("%d\n", line_c);
  }
  if (ops.opt_l && !ops.opt_c && print_this) {
    printf("%s\n", file_name);
  }
  if (ops.opt_l && ops.opt_c) {
    if (files_address_count > 1 || more_than_one_er_fil)
      printf("%s:", file_name);
    printf("%d\n", line_c > 1 ? 1 : line_c);
    if (print_this) {
      printf("%s\n", file_name);
    }
  }

  return 0;
}

int print_changed_output_line(char* line_b, char* templates_ar[],
                              int templates_count, option_t ops, int line_n,
                              int files_address_count, const char file_name[],
                              int* line_c, int* print_this,
                              int more_than_one_er_fil) {
  int errors = 0;
  int print_this_lo = 0;
  int reg = REG_EXTENDED;
  if (ops.opt_i) {
    reg = REG_ICASE;
  }
  errors = print_this_line(line_b, templates_ar, templates_count,
                           &print_this_lo, reg);
  if (ops.opt_v) print_this_lo = !print_this_lo;
  if (files_address_count > 1 || more_than_one_er_fil) {
    if (!ops.opt_l && !ops.opt_c && print_this_lo) {
      printf("%s:", file_name);
    }
  }
  if (ops.opt_n && !ops.opt_c && !ops.opt_l) {
    if (print_this_lo) printf("%d:", line_n);
  }
  if (print_this_lo && !ops.opt_c && !ops.opt_l) custom_printf(line_b);

  if (print_this_lo && ops.opt_c) (*line_c)++;
  if (print_this_lo) {
    *print_this = 1;
  }

  return errors;
}

int print_this_line(char* line_b, char* templates_ar[], int templates_count,
                    int* print_this, int reg) {
  int errors = 0;
  for (int i = 0; i < templates_count && errors == 0; ++i) {
    regex_t re;
    if (regcomp(&re, templates_ar[i], reg) != 0) {
      errors = 2;
      continue;
    }
    if (regexec(&re, line_b, 0, NULL, 0) == 0) {
      *print_this = 1;
    }
    regfree(&re);
  }
  return errors;
}

void custom_printf(const char* line_b) {
  int was_en = 0;
  for (unsigned long int i = 0; i < strlen(line_b); i++) {
    if (line_b[i] == '\n') was_en = 1;
    putc(line_b[i], stdout);
  }
  if (was_en != 1) putc('\n', stdout);
}