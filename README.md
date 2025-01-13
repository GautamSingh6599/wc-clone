# CCWC

CCWC is a C implementation of the Linux command-line tool `wc`. It counts the number of lines, words, and characters in a given file, similar to the `wc` tool found in Unix-based systems.

## Features

- **Line count**: Calculates the number of lines in the file.
- **Word count**: Calculates the number of words in the file.
- **Character count**: Calculates the number of characters in the file.

## Installation

To compile and use CCWC, follow the steps below:

### 1. Clone the repository

```bash
git clone <repository_url>
cd ccwc
```

### 2. Build the project

Use `make` to compile the program:

```bash
make
```

This will generate the executable `ccwc`.

### 3. Run the program

To count the number of lines, words, and characters in a file, use the following command:

```bash
./ccwc <filename>
```

### Example:

```bash
$ ./ccwc test.txt
8852 58164 345604 test.txt
```

This will display the count of lines, words, and characters in the file `test.txt`.

## Testing

The project includes unit tests to verify the correctness of the program. You can run the tests using the following command:

```bash
make test
```

This will build and execute the test suite, comparing the output of `ccwc` with the standard `wc` command.

## Cleanup

To clean up the compiled files, run:

```bash
make clean
```

This will remove the executables and object files.

## Contributing

If you'd like to contribute to this project, feel free to fork the repository and submit a pull request. Please make sure to include tests for any new features or bug fixes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
