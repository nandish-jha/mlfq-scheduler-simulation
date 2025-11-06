# Nandish Jha NAJ474 11282001

CC = gcc
CFLAGS = -g -std=gnu90 -Wall -pedantic
LIB_PATH = /student/cmpt332/pthreads/lib/Linuxx86_64/
LIBS = -lpthreads
CPPFLAGS += -I/student/cmpt332/pthreads -I.

.PHONY: all clean

all: my_sim

my_sim: sim.o liblist.a
	$(CC) $(CFLAGS) $(CPPFLAGS) -o my_sim sim.o liblist.a -L$(LIB_PATH) $(LIBS)

sim.o: sim.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c sim.c

liblist.a: list_movers.o list_adders.o list_removers.o
	ar rcs liblist.a list_movers.o list_adders.o list_removers.o

list_movers.o: list_movers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_movers.c

list_adders.o: list_adders.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_adders.c

list_removers.o: list_removers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_removers.c

clean:
	clear
	rm -f *.o *.a my_sim