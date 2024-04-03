IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = comms.h modeMonitor.h PI.h PID.h ReferenceGenerator.h regulator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = comms.o ModeMonitor.o PI.o PID.o receiver.o ReferenceGenerator.o regulator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

receiver.os: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 