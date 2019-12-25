CXX=g++
CXXFLAGS= -std=c++17 #-Wall -Werror
POSITION_OBJS = treeset.o

all: position

position: $(POSITION_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

doc: Doxyfile
	doxygen

clean:
	rm -f position *.o
	rm -rf docs

.PHONY: all clean doc
