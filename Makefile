# # Compiler
# CC = gcc

# # Compiler flags
# CFLAGS = -Wall -O2 -Iincludes

# # Linker flags
# LDFLAGS = -lm

# INCLUDES = -I/usr/local/include

# # Source files
# SRC = main.c filter_lp.c audio.c filter_hp.c ui.c equalizer.c pre_processing.c global_values.c
# OBJ = $(SRC:.c=.o)

# # Executable name
# TARGET = main

# # Default rule
# all: $(TARGET)

# # Rule to build the executable
# $(TARGET): $(OBJ)
# 	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OBJ) $@

# # Rule to compile source files
# %.o: %.c
# 	$(CC) $(CFLAGS) -c -o $@ $<

# print:
# 	echo $(CC) $(CFLAGS) $(LDFLAGS) -o $(OBJ) $^
# # Clean rule
# clean:
# 	rm -f $(OBJ) $(TARGET)

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/audio_equalizer
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude
CFLAGS   := -Wall -O2
LDFLAGS  := -Linclude
LDLIBS   := -lm -lfftw3-3

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
