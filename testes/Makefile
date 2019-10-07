CC=gcc
LIB_DIR=../lib

all: main

main: main.c $(LIB_DIR)/libcthread.a
	$(CC) -m32 -o teste main.c -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -rf main *.o *~
