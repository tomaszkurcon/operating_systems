
CC = gcc
CFLAGS = -g -Wall -std=c17 -O0
BUILD_DIR=../build
LIB_DIR = $(BUILD_DIR)/lib
all : collatz_static.o

collatz_static.o : collatz.c collatz.h
	mkdir -p $(LIB_DIR)/collatz
	$(CC) $(CFLAGS) -c $< -o $(LIB_DIR)/collatz/$@
	ar rcs $(LIB_DIR)/collatz/libcollatz.a $(LIB_DIR)/collatz/$@

collatz_shared.o : collatz.c collatz.h
	mkdir -p $(LIB_DIR)/collatz
	$(CC) $(CFLAGS) -c $< -o $(LIB_DIR)/collatz/$@ -fPIC
	$(CC) -shared -o $(LIB_DIR)/collatz/libcollatz_shared.so $(LIB_DIR)/collatz/$@

clean :
	-rm -rf $(BUILD_DIR)

.PHONY : all clean