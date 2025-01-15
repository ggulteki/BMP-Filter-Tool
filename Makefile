# Compiler selection
CC = gcc

# Compiler flags split for readability
# -ggdb3: Generate debug info for GDB
# -gdwarf-4: Use DWARF version 4 debugging format
# -O0: No optimization (better for debugging)
# -std=c11: Use C11 standard
# Various warning flags for better code quality
CFLAGS = -ggdb3 -gdwarf-4 -O0 -std=c11 -Wall -Werror -Wextra \
         -Wno-gnu-folding-constant -Wno-sign-compare \
         -Wno-unused-parameter -Wno-unused-variable -Wshadow

# Linker flags (-lm links the math library)
LDFLAGS = -lm

# Output executable name
NAME = bmpfilter

# Directory structure
SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIR = include/

# Find all .c files in source directory
SRC_FILES = $(wildcard $(SRC_DIR)*.c)

# Convert source files (.c) to object files (.o) in object directory
OBJ = $(SRC_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Generate dependency file names (.d) from object files
# These files track header dependencies
DEPS = $(OBJ:.o=.d)

# Command for removing files/directories
RM = rm -rf

# Clear default suffixes and define our own
# This makes the Makefile more explicit about which file types it handles
.SUFFIXES:
.SUFFIXES: .c .o .d

# Default target
all: $(NAME)

# Include dependency files if they exist
# The '-' prefix means "don't error if files don't exist"
# This ensures proper recompilation when headers change
-include $(DEPS)

# Rule to compile source files to object files
# @mkdir -p: Create object directory if it doesn't exist
# -MMD: Generate dependency files automatically
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -MMD -c $< -o $@

# Link object files to create the executable
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean object files and dependencies
clean:
	$(RM) $(OBJ_DIR)

# Full clean: remove objects and executable
fclean: clean
	$(RM) $(NAME)

# Complete rebuild
re: fclean all

# Declare phony targets (targets that don't create files)
# This prevents conflicts with files named clean, all, etc.
.PHONY: all clean fclean re
