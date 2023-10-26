CC=gcc
CFLAGS=-g -Wall -Wextra -O -pthread
LDLIBS=-lm -pthread

EXECS=main client

all: $(EXECS)

main: main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c $<


client: client.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

client.o: client.c
	$(CC) $(CFLAGS) -c $<
