# user | random
MODE = user
# Can be set to zero for a raw performance test
RANDOM_INTERVAL_MILLIS = 0
GRID_SIZE = 100

ifeq ($(MODE), random)
	EXEC_ARGS = $(MODE) $(RANDOM_INTERVAL_MILLIS) $(GRID_SIZE)
else
	EXEC_ARGS = $(MODE) $(GRID_SIZE)
endif

TARGET = perco.exe
CC = gcc
CFLAGS = -Wall -Werror
SRC = main.c percolation.c rendering.c
LIBS = -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2

all:
	$(CC) $(SRC) $(CLFAGS) $(LIBS) -o $(TARGET)

run: 
	./$(TARGET) $(EXEC_ARGS)