CC=clang++
LDFLAGS=-g
# yes I NEED curses AND sdl2 BOTH at all times
LDLIBS=-pthread -lncurses -lSDL2main -lSDL2

OBJS=build
BINS=bin

$(BINS)/byte: $(OBJS)/byte.o
	mkdir -p ./$(BINS)
	$(CC) $(LDFLAGS) -o $(BINS)/byte $(OBJS)/byte.o $(LDLIBS)

$(OBJS)/byte.o: byte.cpp
	mkdir -p ./$(OBJS)
	$(CC) -c byte.cpp -o $(OBJS)/byte.o
