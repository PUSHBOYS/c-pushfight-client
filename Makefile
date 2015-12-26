CC=gcc
TARGET=pushfight-client

OBJ:=$(patsubst %.c,%.o,$(wildcard *.c))

$(TARGET): bin $(OBJ)
	$(CC) -lncurses $(OBJ) -o bin/$@

bin:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) $(OBJ)
	$(RM) bin/$(TARGET)
