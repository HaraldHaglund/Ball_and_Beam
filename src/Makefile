CC=gcc
CFLAGS=-Wall -Wextra -pthread

OBJ_DIR = ../obj
BIN_DIR = ../bin
SRC_DIR = ./c

#source files
SRC := $(wildcard $(SRC_DIR)/*.c)
#SRC_MAIN = fill in for when main is done 

#object files
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


#executables
RECEIVER = $(BIN_DIR)/receiver.so
#MAIN = main

#Rule for making .o files from .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c "$<" -o "$@"

#Rule to build executables
$(RECEIVER): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(RECEIVER)

#$(MAIN): $(OBJS_MAIN)
#    $(CC) $(CFLAGS) $(OBJS_MAIN) -o $(MAIN)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*

all: $(RECEIVER)