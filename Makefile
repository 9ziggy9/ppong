EXE=./run
CC=g++
CXXFLAGS=-Wall -Wextra -pedantic -Wconversion
RAYFLAGS=`pkg-config --cflags --libs raylib`
OBJS=session.o

all: clean main

main: $(OBJS) main.cpp
	$(CC) $(CXXFLAGS) -o $(EXE) main.cpp $(RAYFLAGS) $(OBJS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(RAYFLAGS)

clean:
	rm -rf $(EXE) *.o *.i *.asm
