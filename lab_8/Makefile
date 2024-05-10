CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lrt -lpthread
BUILD_DIR =build

all : user printer
user : user.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< $(LDFLAGS)
printer : printer.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< $(LDFLAGS)

clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean
