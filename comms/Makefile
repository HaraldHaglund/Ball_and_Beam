CC = gcc
CFLAGS = -Wall -Wextra

# Targets
PROGS = setRef

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking
#setInnerParameters.c: setInnerParameters.o #regul.o or similar to be able to set parameters
#setOuterParameters.c: setOuterParameters.o #regul.o or similar to be able to set parameters
#setMode.c: setMode.o #ModeMonitor.o or similar to be able to set
setRef.c: setRef.o ../test.o #RefgenMonitor.o or similar to be able to set
#shutDown.c: shutDown.o #regul.o or similar to be able to set parameters

# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	rm -f *.o *.d