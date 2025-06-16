# compiler
CC = gcc
# source file
SRC = main.c
# bin file
TARGET = main

# Compiler flags
CFLAGS = -Wall    # for warnings

# library paths for raylib (built with cmake)
LDFLAGS = -L/usr/local/lib

# libraries to link and dependencies
LIBS = -lraylib -lm

default: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFALGS) $(LIBS)
clean:
	rm -f $(TARGET) *.o
