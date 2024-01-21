# user | random
MODE = random
# Can be set to zero for fastest possible simulation
RANDOM_INTERVAL_MILLIS = 1
# Warning: The grid size cannot be above 1/2 of the window size, otherwise the grid will not render.
# If you wish to test a larger grid, please increase the window size in the rendering.c file.
GRID_SIZE = 100

ifeq ($(MODE), random)
	EXEC_ARGS = $(MODE) $(RANDOM_INTERVAL_MILLIS) $(GRID_SIZE) $(WINDOW_SIZE)
else
	EXEC_ARGS = $(MODE) $(GRID_SIZE) $(WINDOW_SIZE)
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