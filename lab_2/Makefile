CC = gcc
CFLAGS = -g -Wall -std=c17 -O0
BUILD_DIR=build
LIB_DIR = $(BUILD_DIR)/lib
all : client_static client_shared

client_static : main.c
	mkdir -p $(BUILD_DIR)
	make -C collatz collatz_static.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -L$(LIB_DIR)/collatz -lcollatz

client_shared : main.c
	mkdir -p $(BUILD_DIR)
	make -C collatz collatz_shared.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -L$(LIB_DIR)/collatz -lcollatz_shared -Wl,-rpath=./build/lib/collatz

client_dynamic : main.c
	mkdir -p $(BUILD_DIR)
	make -C collatz collatz_shared.o
	$(CC) $(CFLAGS) -D DYNAMIC -o $(BUILD_DIR)/$@ $< -L$(LIB_DIR)/collatz -lcollatz_shared -Wl,-rpath=./build/lib/collatz

run_client_static :
	make client_static
	./$(BUILD_DIR)/client_static $(ARGS)
run_client_shared :
	make client_shared
	./$(BUILD_DIR)/client_shared $(ARGS)
run_client_dynamic :
	make client_dynamic
	./$(BUILD_DIR)/client_dynamic $(ARGS)
clean :
	-rm -rf $(BUILD_DIR)
.PHONY : all client_static clean