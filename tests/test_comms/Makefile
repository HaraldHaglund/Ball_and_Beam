CC = gcc
CFLAGS = -Wall -Wextra

# Targets
PROGS = recieve_c send_c

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking
recieve_c: recieve_c.o
send_c: send_c.o

# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	rm -f *.o *.d