# user | random
MODE = random
RANDOM_INTERVAL_MILLIS = 1
GRID_SIZE = 50

TARGET = perco.exe
CC = gcc
CFLAGS = -Wall -Werror
SRC = main.c percolation.c rendering.c
LIBS = -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2

all:
	$(CC) $(SRC) $(CLFAGS) $(LIBS) -o $(TARGET)

run: 
	./$(TARGET) $(MODE) $(RANDOM_INTERVAL_MILLIS) $(GRID_SIZE)