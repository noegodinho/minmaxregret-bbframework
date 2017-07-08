CC = g++
DEBUG_FLAGS = -Wall -Wextra -g
RESULT_BB = -DRESULT_B_B
#SORT = -DSORT
THREAD = -DTHREAD
LTHREAD = -lpthread
INITIAL = -DINITIAL
BOUNDS = -DBOUND1 -DBOUND2
CFLAGS = $(DEBUG_FLAGS) -std=c++11 -lrt -O3
CFLAGS += $(RESULT_BB) $(BOUNDS) $(INITIAL) $(SORT) $(THREAD)
SOURCE = src/*
DEPS = lib/*
BIN = ./bin/
TARGET = $(BIN)main

subsystem:
	$(CC) $(CFLAGS) $(SOURCE) $(DEPS) -o $(TARGET) $(LTHREAD)

run:
	$(TARGET)

clean:
	$(RM) $(TARGET)
