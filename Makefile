EXE=./run
CC=g++
STD=-std=c++20
CXXFLAGS=-Wall -Wextra -pedantic -Wconversion
RAYFLAGS=`pkg-config --cflags --libs raylib`
OBJS=session.o

all: clean main

main: $(OBJS) main.cpp
	$(CC) $(CXXFLAGS) -o $(EXE) main.cpp $(RAYFLAGS) $(OBJS)

test: test.cpp
	$(CC) $(CXXFLAGS) -o ./test test.cpp $(STD)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(RAYFLAGS)

clean:
	rm -rf $(EXE) *.o *.i *.asm
