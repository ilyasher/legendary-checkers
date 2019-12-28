CXX=g++
CXXFLAGS= -std=c++17 #-Wall -Werror
POSITION_OBJS = position.o
ALL_OBJS = position.o rulebook.o ui.o

all: checkers

run: checkers
	./checkers

# position.o: position.cpp rulebook.cpp position.h rulebook.h
# 	$(CXX) $(CXXFLAGS) -c position.cpp rulebook.cpp

checkers: $(ALL_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

position: $(POSITION_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

doc: Doxyfile
	doxygen

clean:
	rm -f position checkers *.o
	rm -rf docs

.PHONY: all clean doc run
