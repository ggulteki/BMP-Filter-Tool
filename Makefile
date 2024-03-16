# Makefile for bmp-filter-tool
# Author: Gokberk Gultekin
# Date: March 16, 2024

CC = gcc
CFLAGS = -ggdb3 -gdwarf-4 -O0 -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
LDFLAGS = -lm
NAME = bmpfilter
SRC_DIR = src/
OBJ_DIR = obj/
INCLUDE_DIR = include/
SRC_FILES = $(wildcard $(SRC_DIR)*.c)
OBJ = $(SRC_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
RM = rm -rf

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
