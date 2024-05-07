EXE=./run
CC=g++
STD=-std=c++20
CXXFLAGS=-Wall -Wextra -pedantic -Wconversion
RAYFLAGS=`pkg-config --cflags --libs raylib`
OBJS=session.o

all: clean main

main: $(OBJS) main.cpp
	$(CC) $(CXXFLAGS) -o $(EXE) main.cpp $(RAYFLAGS) $(OBJS) $(STD)

debug: clean $(OBJS) main.cpp
	$(CC) $(CXXFLAGS) -o $(EXE) main.cpp \
		$(RAYFLAGS) $(OBJS) -D_PPONG_DEBUG_ $(STD)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(RAYFLAGS)

clean:
	rm -rf $(EXE) *.o *.i *.asm test
