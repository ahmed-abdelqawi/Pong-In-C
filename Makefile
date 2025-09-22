# Name of the game
TARGET = pong

# Source files
SRC = main.c

# Compiler
CC = gcc

# Default standard (C23)
CSTD = c23

# Common flags
CFLAGS = -Wall -std=$(CSTD)

# Detect OS
ifeq ($(OS),Windows_NT)
    # ---- Windows (MinGW) ----
    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    RM = del /Q
    EXT = .exe
else
    # ---- Linux (Arch) ----
    LDLIBS = -lraylib -lm -ldl -lpthread -lGL -lX11
    RM = rm -f
    EXT =
endif

# Build rule
$(TARGET)$(EXT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)$(EXT) $(LDLIBS)

# Run rule
run: $(TARGET)$(EXT)
	./$(TARGET)$(EXT)

# Clean rule
clean:
	$(RM) $(TARGET)$(EXT)

# ---- Standard switching ----
c11:
	$(MAKE) CSTD=c11

c17:
	$(MAKE) CSTD=c17

c23:
	$(MAKE) CSTD=c23

# ---- Debug build ----
debug:
	$(MAKE) CFLAGS="-Wall -std=$(CSTD) -g -O0"

# ---- Release build ----
release:
	$(MAKE) CFLAGS="-Wall -std=$(CSTD) -O2 -s"