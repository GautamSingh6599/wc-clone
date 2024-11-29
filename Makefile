CC = gcc
CFLAGS = -Wall -Werror -g
SRC = src/main.c src/utils.c
# Header files
INCLUDES = -I include/
# Output executable
OUTPUT = ccwc

# Default target
all: $(OUTPUT)

# Build executable
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(INCLUDES) -o $(OUTPUT) $(N_FLAGS)

# Clean build files
clean:
	rm -f $(OUTPUT)
	rm -f *.o


