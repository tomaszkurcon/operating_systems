CC = gcc
CFLAGS = -g -Wall
BUILD_DIR =build

all : zad_1 zad_2
zad_1 : zad_1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
zad_2 : zad_2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean
