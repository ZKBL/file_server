vpath %.h ./include
vpath %.c ./src
CC=clang
CFLAGS=-I./include -Wall -pthread
SRC=$(wildcard *.c ./src/*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
program:$(OBJ)
	$(CC) -o program $(CFLAGS) $(OBJ)
$(OBJ): %.o:%.c
	$(CC) -c  $< -o $@ $(CFLAGS)
.PYTHON:clean 
clean:
	rm $(OBJ) program -rf
