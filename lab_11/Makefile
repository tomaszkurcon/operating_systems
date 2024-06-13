CC = gcc
CFLAGS = -g -Wall -pthread
BUILD_DIR =build
all : server client
server : server.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
client : client.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
clean :
	-rm -rf $(BUILD_DIR)
.PHONY : all clean

