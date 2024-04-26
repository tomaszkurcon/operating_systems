CC = gcc
CFLAGS = -g -Wall
BUILD_DIR =build

all : zad1 catcher sender
zad1 : zad1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

catcher : zad_2/catcher.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

sender : zad_2/sender.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean
