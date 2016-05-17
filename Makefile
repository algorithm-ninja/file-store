OBJECTS=build/shuriken-cp.o build/file_manager.o
CXX?=g++
CXXFLAGS=-O2 -Wall -std=c++11 -Iheaders -ggdb -I/opt/local/include

LDFLAGS=-lcrypto

.PHONY: all clean

all: build/shuriken-cp

build/shuriken-cp: ${OBJECTS}
	${CXX} ${OBJECTS} ${LDFLAGS} -o build/shuriken-cp

build/%.o: src/%.cpp $(wildcard headers/*hpp)
	${CXX} ${CXXFLAGS} -c -o $@ $<

clean:
	rm -f build/shuriken-cp ${OBJECTS}
