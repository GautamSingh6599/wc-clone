#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *test_name;
  int (*test_func)(void);
} TestCase;

char *execute_command(const char *command) {
  FILE *fp;
  char *output = malloc(1035 * sizeof(char));
  if (output == NULL) {
    perror("Failed to allocate memory\n");
    exit(EXIT_FAILURE);
  }
  char buffer[1035];

  fp = popen(command, "r");
  if (fp == NULL) {
    perror("Failed to run command\n");
    free(output);
    exit(EXIT_FAILURE);
  }

  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    strcpy(output, buffer);
  } else {
    output[0] = '\0';
  }

  pclose(fp);
  return output;
}

int test_ccwc_file_default(void) {
  const char *file_path = "./test/test.txt";
  char command_wc[1035];
  char command_ccwc[1035];

  snprintf(command_wc, sizeof(command_wc), "wc -l %s", file_path);
  snprintf(command_ccwc, sizeof(command_ccwc), "./ccwc -l %s", file_path);

  char *wc_output = execute_command(command_wc);
  char *ccwc_output = execute_command(command_ccwc);

  if (wc_output == NULL || ccwc_output == NULL) {
    fprintf(stderr, "Failed to run command\n");
    free(wc_output);
    free(ccwc_output);
    return 1;
  }

  int result = (strcmp(wc_output, ccwc_output) == 0);

  free(wc_output);
  free(ccwc_output);

  return result;
}

void run_test(TestCase *test_case, size_t num_tests) {
  int num_passed = 0;
  for (size_t i = 0; i < num_tests; i++) {
    printf("Running test %s\n", test_case[i].test_name);
    if (test_case[i].test_func() == 0) {
      num_passed++;
    } else {
      printf("Test %s failed\n", test_case[i].test_name);
    }
  }
  printf("Passed %d/%zu tests\n", num_passed, num_tests);
}

int main(void) {
  TestCase test_cases[] = {
      {"test_ccwc_file_default", test_ccwc_file_default},
  };

  size_t num_tests = sizeof(test_cases) / sizeof(TestCase);
  run_test(test_cases, num_tests);

  return 0;
}
