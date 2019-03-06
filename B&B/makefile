CC = g++
DEBUG_FLAGS = -Wall -Wextra -g
RESULT_BB = -DRESULT_B_B
#INITIAL = -DINITIAL
INITIAL_W = -DINITIAL_W
BOUNDS = -DBOUND1 -DBOUND2
CFLAGS = $(DEBUG_FLAGS) -std=c++11 -lrt -O3
CFLAGS += $(RESULT_BB) $(BOUNDS) $(INITIAL) $(INITIAL_W)
SOURCE = src/*
DEPS = lib/*
BIN = ./bin/
TARGET = $(BIN)main

subsystem:
	$(CC) $(CFLAGS) $(SOURCE) $(DEPS) -o $(TARGET)

run:
	$(TARGET)

clean:
	$(RM) $(TARGET)
