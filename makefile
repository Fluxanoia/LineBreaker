CC = clang
CFLAGS = -std=c11 -Wall -pedantic -g
  
LFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lm -fsanitize=undefined

FILES = *.c $\
states/*.c $\
util/*.c $\
aesthetics/*.c
  
default: main.c
	$(CC) -I/usr/include/SDL2 -D_REENTRANT $(CFLAGS) -o game $(FILES) $(LFLAGS)
