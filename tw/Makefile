CC=clang++
LDFLAGS=-g
# yes I NEED curses AND sdl2 BOTH at all times
LDLIBS=-pthread -lncurses -lSDL2main -lSDL2

OBJS=build

tw: $(OBJS)/tw.o
	$(CC) $(LDFLAGS) -o tw $(OBJS)/tw.o $(LDLIBS)

$(OBJS)/tw.o: tw.cpp
	mkdir -p ./$(OBJS)
	$(CC) -c tw.cpp -o $(OBJS)/tw.o

