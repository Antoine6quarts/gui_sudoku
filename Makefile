#Makefile

CC = g++
TARGET = gui.exe
SRC_DIR =
SRC_FILES = *.cpp
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS = `sdl2-config --libs`

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_FILES) -o $(TARGET) $(LDLIBS)

clean:
	@rm -rf $(TARGET)

#END