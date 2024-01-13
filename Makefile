all:
	gcc main.c -Wall -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2 -o perco.exe

run: 
	./perco.exe 10 10