
CC=gcc

CFLAGS=-Wall

all: shell

shell: shell.o commands.o misc.o
	$(CC) shell.o commands.o misc.o -o shell

shell.o: shell.c
	$(CC) -c $(CFLAGS) shell.c
commands.o: commands.c
	$(CC) -c $(CFLAGS) commands.c
misc.o: misc.c
	$(CC) -c $(CFLAGS) misc.c
