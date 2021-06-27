# CC and CFLAGS are variables
CC = g++
CFLAGS = -c

all	: bin/cb
	@echo -n ""

bin/cb	: main.o graph.o
		$(CC) main.o graph.o -o $@
main.o	: src/main.cpp
		$(CC) $(CFLAGS) $< -o $@
graph.o	: src/graph.cpp src/graph.hpp
		$(CC) $(CFLAGS) $< -o $@

test	: test.o graph.o
		$(CC) test.o graph.o -o $@
test.o	: src/graph_test.cpp
		$(CC) $(CFLAGS) $< -o $@

clean	:
		rm -rf *.o bin/*