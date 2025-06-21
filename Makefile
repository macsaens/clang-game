# Makefile for raylib (Linux & macOS)
# Platform detection

# --> General Configuration
SRC = main.c
TARGET = game 

# --> Platform-Specific Configuration

# Detect Operating System
# Use 'uname -s | tr '[:upper:]' '[:lower:]'' to get lowercase
# Or just strip whitespace if that's the issue.
UNAME_S := $(shell uname -s) # Linux or Darwin

# Trim potential whitespace from uname output to ensure clean comparison
UNAME_S_STRIPPED := $(strip $(UNAME_S))

ifeq ($(UNAME_S_STRIPPED),Linux) # Note: Linux typically returns "Linux" with a capital L
    # Linux
    CC = gcc
    # Assuming raylib is installed system-wide for headers and libs
    # Adjust if your raylib installation path is different on Linux
    INCLUDE_PATHS =
    LIB_PATHS = -L/usr/local/lib
    LIBS = -lraylib -lm
    FRAMEWORKS = # No frameworks needed on Linux
else ifeq ($(UNAME_S_STRIPPED),Darwin) # Note: macOS returns "Darwin" with a capital D
    # macOS
    CC = clang
    # Assuming local project structure for headers and libs
    INCLUDE_PATHS = -Iinclude
    LIB_PATHS = -Llib
    LIBS = -lraylib
    # macOS Frameworks needed by raylib
    FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    # Default or unsupported OS
    $(error "Unsupported operating system: $(UNAME_S_STRIPPED)") # Use stripped variable here for clarity
endif

# --> Common Configuration (Applies to both platforms)

# Compiler flags
CFLAGS = -Wall $(INCLUDE_PATHS) # Add common flags + platform-specific include paths

# Linker flags
LDFLAGS = $(LIB_PATHS) # Add platform-specific lib paths

# --> Build Rules

.PHONY: default all
default: $(TARGET)  # both `make`, `make default` and `make all` build the executable `game`
all: $(TARGET)      

# build the executable 'game', TARGET
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(LIBS) $(FRAMEWORKS)

.PHONY: clean
clean:
	rm -f $(TARGET) *.o

# Runs the `game` executable
.PHONY: run
run: $(TARGET)
	./$(TARGET)
