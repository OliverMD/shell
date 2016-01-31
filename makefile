
CC=gcc

CFLAGS=-Wall

all: shell

shell: shell.o commands.o
	$(CC) shell.o commands.o -o shell

shell.o: shell.c
	$(CC) -c $(CFLAGS) shell.c
commands.o: commands.c
	$(CC) -c $(CFLAGS) commands.c
