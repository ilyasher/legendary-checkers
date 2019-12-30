CXX=g++
CXXFLAGS= -std=c++17 #-Wall -Werror
POSITION_OBJS = position.o
ALL_OBJS = position.o rulebook.o ui.o legend_AI.o

all: checkers

run: checkers
	./checkers

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
