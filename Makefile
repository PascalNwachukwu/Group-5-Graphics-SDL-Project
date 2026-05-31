CC = gcc
CFLAGS = -Wall -I/mingw64/include/SDL3
LDFLAGS = -L/mingw64/lib -lSDL3 -lSDL3_image

all:
	$(CC) $(CFLAGS) -o sdl-composition main.c $(LDFLAGS)

clean:
	rm -f sdl-composition.exe