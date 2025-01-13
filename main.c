#include "../include/utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VERSION "0.0.1"

#define LINES 0x01
#define WORDS 0x02
#define BYTE 0x04
#define CHARS 0x08

#define FILE_OPEN_ERROR -1

typedef long (*calc_func)(FILE *);

long calculate_bytes(char *filename) {
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    fclose(file);
    return FILE_OPEN_ERROR;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fclose(file);
  return size;
}

long calculate_bytes_stdin(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);
  return size;
}

long calculate_lines(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
    fclose(file);
    return FILE_OPEN_ERROR;
  }
  long lines = 0;
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }
  fclose(file);
  return lines;
}

long calculate_lines_stdin(FILE *file) {
  long lines = 0;
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }
  rewind(file);
  return lines;
}

long calculate_words(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
    fclose(file);
    return FILE_OPEN_ERROR;
  }
  long words = 0;
  char c;
  int in_word = 0; // Tracks whether we're currently inside a word

  while ((c = fgetc(file)) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
      if (in_word) {
        // Transition from inside a word to whitespace
        words++;
        in_word = 0; // Reset in_word flag
      }
    } else {
      // Encountered a non-whitespace character
      in_word = 1;
    }
  }

  // If the file ends while still in a word, count the last word
  if (in_word) {
    words++;
  }

  fclose(file);
  return words;
}

long calculate_words_stdin(FILE *file) {
  long words = 0;
  char c;
  int in_word = 0; // Tracks whether we're currently inside a word

  while ((c = fgetc(file)) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
      if (in_word) {
        // Transition from inside a word to whitespace
        words++;
        in_word = 0; // Reset in_word flag
      }
    } else {
      // Encountered a non-whitespace character
      in_word = 1;
    }
  }

  // If the file ends while still in a word, count the last word
  if (in_word) {
    words++;
  }
  rewind(file);
  return words;
}

long calculate_chars(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
    fclose(file);
    return FILE_OPEN_ERROR;
  }
  long chars = 0;
  char c;
  while ((c = fgetc(file)) != EOF) {
    chars++;
  }
  fclose(file);
  return chars;
}

long calculate_chars_stdin(FILE *file) {
  long chars = 0;
  char c;
  while ((c = fgetc(file)) != EOF) {
    chars++;
  }
  rewind(file);
  return chars;
}

long process_stdin_func(calc_func func) { return func(stdin); }

void process_stdin(int option) {
  if (option & LINES) {
    printf("%6ld ", process_stdin_func(calculate_lines_stdin));
  }
  if (option & WORDS) {
    printf("%6ld ", process_stdin_func(calculate_words_stdin));
  }
  if (option & BYTE) {
    printf("%6ld ", process_stdin_func(calculate_bytes_stdin));
  }
  if (option & CHARS) {
    printf("%6ld ", process_stdin_func(calculate_chars_stdin));
  }
  printf("\n");
}

void process_file(char **files, int num_files, int option) {
  long total_bytes = 0;
  long total_lines = 0;
  long total_words = 0;
  long total_chars = 0;

  for (int i = 0; i < num_files; i++) {
    char *filename = files[i];

    if (option & LINES) {
      long num_lines = calculate_lines(filename);
      if (num_lines == FILE_OPEN_ERROR) {
        fprintf(stderr, "ccwc: %s: No such file or directory\n", filename);
        continue;
      }
      printf("%6ld ", num_lines);
      total_lines += num_lines;
    }

    if (option & WORDS) {
      long num_words = calculate_words(filename);
      if (num_words == FILE_OPEN_ERROR) {
        fprintf(stderr, "ccwc: %s: No such file or directory\n", filename);
        continue;
      }
      printf("%6ld ", num_words);
      total_words += num_words;
    }

    if (option & BYTE) {
      long num_bytes = calculate_bytes(filename);
      if (num_bytes == FILE_OPEN_ERROR) {
        fprintf(stderr, "ccwc: %s: No such file or directory\n", filename);
        continue;
      }
      printf("%6ld ", num_bytes);
      total_bytes += num_bytes;
    }

    if (option & CHARS) {
      long num_chars = calculate_chars(filename);
      if (num_chars == FILE_OPEN_ERROR) {
        fprintf(stderr, "ccwc: %s: No such file or directory\n", filename);
        continue;
      }
      printf("%6ld ", num_chars);
      total_chars += num_chars;
    }
    printf("%s\n", filename);
  }

  if (num_files > 1) {
    if (option & BYTE) {
      printf("%ld ", total_bytes);
    }
    if (option & LINES) {
      printf("%ld ", total_lines);
    }
    if (option & WORDS) {
      printf("%ld ", total_words);
    }
    if (option & CHARS) {
      printf("%ld ", total_chars);
    }
    printf("total\n");
  }
}

void parse_args(int argc, char *argv[]) {
  char **files = NULL;
  int option = 0;
  int num_files = 0;
  int opt;

  while ((opt = getopt(argc, argv, "hmvcwl")) != -1) {
    switch (opt) {
    case 'h':
      printf("Usage: ./ccwc [OPTION]... [FILE]...\n");
      printf("Print line, word, and byte counts for each FILE.\n");
      printf("A word is a non-zero-length sequence of characters delimited by "
             "white space.\n");
      exit(EXIT_SUCCESS);
    case 'v':
      printf("ccwc (C) 2021 v%s\n", VERSION);
      exit(EXIT_SUCCESS);
    case 'c':
      option |= BYTE;
      break;
    case 'w':
      option |= WORDS;
      break;
    case 'l':
      option |= LINES;
      break;
    case 'm':
      option |= CHARS;
      break;
    default:
      fprintf(stderr, "Usage: ./ccwc [OPTION]... %s ...\n", argv[0]);
      exit(EXIT_SUCCESS);
    }
  }

  num_files = argc - optind;
  if (num_files > 0) {
    files = argv + optind;
  }

  if (option == 0) {
    option = BYTE | WORDS | LINES;
  }

  if (num_files == 0) {
    process_stdin(option);
  } else {
    process_file(files, num_files, option);
  }
}

int main(int argc, char *argv[]) {
  parse_args(argc, argv);
  return EXIT_SUCCESS;
}
