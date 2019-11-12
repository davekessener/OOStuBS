CC=g++
CFLAGS=-Wall -ggdb -O0 -I../src/
SRC=$(wildcard ut_*.cc)
OBJ=$(shell find ../bin/ -name "*.cc.o")
DEP=$(shell find .. -name "*.h")

.PHONY: all clean

all:
	$(MAKE) -C .. objects CXXFLAGS="-Wall -ggdb -O0 -Isrc/ -DNOKERNEL" VERBOSE=
	$(MAKE) -C . test
	./test

test: main.cc $(OBJ) $(DEP)
	$(CC) -c main.cc $(CFLAGS) -o main.o
	$(CC) main.o $(OBJ) -o test

main.cc: $(SRC) $(DEP) build.rb
	./build.rb

clean:
	$(MAKE) -C .. clean
	rm -rf main.cc test main.o

