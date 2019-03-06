CC = g++
DEBUG_FLAGS = -Wall -Wextra -g
RESULT = -DRESULT
CFLAGS = $(DEBUG_FLAGS) -lrt -O3 $(RESULT)
CCFLAGS = -lginac -lcln
SOURCE = generic.cpp
TARGET = generic

subsystem:
	$(CC) -o $(TARGET) $(CFLAGS) $(SOURCE) $(CCFLAGS)

clean:
	$(RM) $(TARGET)
