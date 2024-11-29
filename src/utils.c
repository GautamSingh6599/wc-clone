#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void parse_args(int argc, char *argv[]) {
  char **file = malloc(sizeof(char *));
  long total_bytes = 0;
  long total_words = 0;
  long total_chars = 0;
  long total_lines = 0;
  long num_files = 0;
  int option = 0;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0) {
      option = PRINT_HELP;
      continue;
      break;
    }
    if (strcmp(argv[i], "--ver") == 0) {
      option = PRINT_VERSION;
      break;
    }
    if (argv[i][0] == '-') {
      int length = strlen(argv[i]);
      for (int j = 1; j < length; j++) {
        switch (argv[i][j]) {
        case 'l':
          option |= LINES;
          continue;
          break;
        case 'c':
          option |= BYTE;
          continue;
          break;
        case 'm':
          option |= CHARS;
          continue;
          break;
        case 'w':
          option |= WORDS;
          continue;
          break;
        default:
          printf("ccwc: invalid option -- '%c'\n", argv[i][j]);
          printf("Try './ccwc --help' for more information.\n");
          return;
        }
        switch (option) {
        case PRINT_HELP:
          print_help();
          return;
        case PRINT_VERSION:
          print_version();
          return;
        }
      }
      continue;
    }
    if (file_exists(argv[i])) {
      num_files++;
      file = realloc(file, num_files * sizeof(char));
      file[num_files - 1] = argv[i];
    } else {
      printf("ccwc: %s: No such file or directory\n", argv[i]);
      return;
    }
  }
  if (option == 0) {
    option = LINES | WORDS | BYTE;
  }
  for (int file_index = 0; file_index < num_files; file_index++) {
    if (option & LINES) {
      long lines = calculate_lines(file[file_index]);
      if (lines == FILE_OPEN_ERROR) {
        printf("ccwc: %s: No such file or directory\n", file[file_index]);
        return;
      }
      printf("%10ld ", lines);
      total_lines += lines;
    }
    if (option & WORDS) {
      long words = calculate_words(file[file_index]);
      if (words == FILE_OPEN_ERROR) {
        printf("ccwc: %s: No such file or directory\n", file[file_index]);
        return;
      }
      printf("%10ld ", words);
      total_words += words;
    }
    if (option & BYTE) {
      long bytes = calculate_bytes(file[file_index]);
      if (bytes == FILE_OPEN_ERROR) {
        printf("ccwc: %s: No such file or directory\n", file[file_index]);
        return;
      }
      printf("%10ld ", bytes);
      total_bytes += bytes;
    }
    if (option & CHARS) {
      long chars = calculate_chars(file[file_index]);
      if (chars == FILE_OPEN_ERROR) {
        printf("ccwc: %s: No such file or directory\n", file[file_index]);
        return;
      }
      printf("%10ld ", chars);
      total_chars += chars;
    }
    printf("%s\n", file[file_index]);
  }
  if (num_files > 1) {
    printf("%10ld %10ld %10ld total\n", total_lines, total_words, total_bytes);
  }
}

void print_help() {
  printf("Usage: ./ccwc [OPTION]... [FILE]...\n");
  printf("Print line, word, and byte counts for each FILE.\n");
  printf("A word is a non-zero-length sequence of characters delimited by "
         "white space.\n");
}

void print_version() { printf("ccwc (C) 2021 v%s\n", VERSION); }

int file_exists(const char *filename) { return access(filename, F_OK) != -1; }

long calculate_bytes(char *filename) {
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    return FILE_OPEN_ERROR;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fclose(file);
  return size;
}

long calculate_lines(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
    return FILE_OPEN_ERROR;
  }
  long lines = 0;
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      lines++;
    }
  }
  lines++; // Count last line
  return lines;
  fclose(file);
}

long calculate_words(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
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

long calculate_chars(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file == NULL) {
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
