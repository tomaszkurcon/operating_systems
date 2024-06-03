CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lrt -lpthread
BUILD_DIR =build

all : main
main : main.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< $(LDFLAGS)
clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean
