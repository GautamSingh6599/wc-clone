CC = gcc
CFLAGS = -Wall -Werror -g
INCLUDE =
LDFLAGS =

SRC = main.c
OBJ = $(SRC:.c=.o)
TEST_SRC = test/test_ccwc.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Output executable
OUTPUT = ccwc
TEST_OUTPUT = make_test

# Default target: Build the main executable
all: $(OUTPUT)

# Build the test executable
make_test: $(TEST_OBJ)
	$(CC) $(TEST_OBJ) $(LDFLAGS) -o $(TEST_OUTPUT)

# Run the tests
test: make_test
	./$(TEST_OUTPUT)

# Build the main executable
$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUTPUT)

# Compile main program
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Compile test files
$(TEST_OBJ): $(TEST_SRC)
	$(CC) $(CFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)

# Clean build files
clean:
	rm -f $(OUTPUT) $(TEST_OUTPUT)
	rm -f *.o test/*.o
