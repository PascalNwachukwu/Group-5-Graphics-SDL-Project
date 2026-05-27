CC = gcc
CFLAGS = -Wall -I/mingw64/include/SDL2
LDFLAGS = -L/mingw64/lib -mwindows -lmingw32 -lSDL2 -lSDL2_image

all:
	$(CC) $(CFLAGS) -o sdl-composition main.c $(LDFLAGS)

clean:
	rm -f sdl-composition.exe