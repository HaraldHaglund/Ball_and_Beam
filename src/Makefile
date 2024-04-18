CC = gcc
CFLAGS = -Wall -Wextra -pthread
LIBS = -lmoberg

OBJ_DIR = ../obj
BIN_DIR = ../bin
SRC_DIR = ./c

#source files
SRC_REGULATOR := $(SRC_DIR)/regulator.c $(SRC_DIR)/comms.c $(SRC_DIR)/DataMonitor.c
SRC_RECEIVER := $(SRC_DIR)/receiver.c $(SRC_DIR)/PI.c $(SRC_DIR)/PID.c $(SRC_DIR)/ModeMoniotor.c $(SRC_DIR)/ReferenceGenerator.c $(SRC_DIR)/DataMonitor.c $(SRC_DIR)/refgulator.c
SRC_MAIN := $(SRC_DIR)/Main.c $(SRC_DIR)/DataMonitor.c $(SRC_DIR)/ModeMonitor.c $(SRC_DIR)/PI.c $(SRC_DIR)/PID.c $(SRC_DIR)/ReferenceGenerator.c $(SRC_DIR)/regulator.c $(SRC_DIR)/comms.c

#object files
OBJ_REGULATOR := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_REGULATOR))
OBJ_RECEIVER := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_RECEIVER))
OBJ_MAIN := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_MAIN))

#executables
EXEC_RECEIVER = $(BIN_DIR)/receiver.so
EXEC_MAIN = $(BIN_DIR)/Main.out

#Rule for making .o files from .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#Rule to build executables
$(EXEC_RECEIVER): $(OBJ_RECEIVER)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(EXEC_MAIN): $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*

all: $(EXEC_RECEIVER) $(EXEC_MAIN)
# Ensure that the output directories exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))