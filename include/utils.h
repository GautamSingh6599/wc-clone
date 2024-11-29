#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_
#include <stdio.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define VERSION "0.0.0"
#define PRINT_HELP 25
#define PRINT_VERSION 26
#define BYTE 1
#define CHARS 2
#define LINES 4
#define WORDS 8

void parse_args(int argc, char *argv[]);
int file_exists(const char *filename);
void print_help();
void print_version();
long calculate_bytes(char *filename);
long calculate_lines(char *filename);
long calculate_words(char *filename);
long calculate_chars(char *filename);
long calculate_bytes_stdin(FILE *file);
long calculate_lines_stdin(FILE *file);
long calculate_words_stdin(FILE *file);
long calculate_chars_stdin(FILE *file);

#endif // INCLUDE_UTILS_H_
