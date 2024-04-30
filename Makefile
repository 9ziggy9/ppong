EXE=./run
CC=g++
CXXFLAGS=-Wall -Wextra -pedantic -Wconversion
RAYFLAGS=`pkg-config --cflags --libs raylib`


all: clean main
	$(EXE) 30

main: main.cpp
	$(CC) $(CXXFLAGS) -o $(EXE) $@.cpp $(RAYFLAGS)

ass: main.c
	gcc $(CXXFLAGS) -o $(EXE).asm -S $<

main_c: main.c
	gcc $(CXXFLAGS) -o $(EXE) $<

clean:
	rm -rf $(EXE) *.o *.i *.asm
