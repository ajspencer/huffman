CXX=g++
CFLAGS=-std=c++11

EXECUTE=huffman

all: $(EXECUTE)

$(EXECUTE): main.cpp huffman.cpp
	$(CXX) -o $(EXECUTE) $(CFLAGS) main.cpp huffman.cpp

clean:
	rm -f $(EXECUTE)
