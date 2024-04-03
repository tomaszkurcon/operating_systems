CC = gcc
CFLAGS = -g -Wall -std=c17
BUILD_DIR =build


all : zad1 zad2

zad1 : zad1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
zad2 : zad2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean

