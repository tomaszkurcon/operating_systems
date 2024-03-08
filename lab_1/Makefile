CC = gcc
CFLAGS = -g -Wall -std=c17


all : countdown
countdown : countdown.c
	$(CC) $(CFLAGS) -o countdown countdown.c

clean :
	-rm countdown

.PHONY : all countdown clean

