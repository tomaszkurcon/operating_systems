CC = gcc
CFLAGS = -g -Wall -std=c17
BUILD_DIR =build


all : main

main : main.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean

