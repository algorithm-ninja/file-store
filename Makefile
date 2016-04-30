OBJECTS=$(patsubst src/%.cpp,build/%.o,$(wildcard src/*cpp))
CC=g++
CXXFLAGS=-O2 -Wall -std=c++11 -Iheaders -ggdb
LDFLAGS=

.PHONY: all clean

all: ${OBJECTS}

build/%.o: src/%.cpp $(wildcard headers/*hpp)
	${CC} ${CXXFLAGS} -c -o $@ $<

clean:
	rm -f ${OBJECTS}
