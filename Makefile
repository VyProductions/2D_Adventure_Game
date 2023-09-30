OBJECTS=action_impl.o advent.o input_map.o input.o system.o text.o player.o map.o
CXX=g++
CXXFLAGS=-std=c++2a -Wall -Wextra -pedantic -Wshadow -Wconversion -Werror=vla -Werror=return-type -Wno-deprecated-copy -Wno-multichar -g

all: clean advent
	@rm *.o -f
	@clear

advent: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -lcurses $(OBJECTS) -o advent -lncursesw

action_impl.o:
	$(CXX) $(CXXFLAGS) -lcurses action_impl.cpp -c

advent.o:
	$(CXX) $(CXXFLAGS) -lcurses advent.cpp -c

input_map.o:
	$(CXX) $(CXXFLAGS) -lcurses input_map.cpp -c

input.o:
	$(CXX) $(CXXFLAGS) -lcurses input.cpp -c

system.o:
	$(CXX) $(CXXFLAGS) -lcurses system.cpp -c

text.o:
	$(CXX) $(CXXFLAGS) -lcurses text.cpp -c

player.o:
	$(CXX) $(CXXFLAGS) -lcurses player.cpp -c

map.o:
	$(CXX) $(CXXFLAGS) -lcurses map.cpp -c

clean:
	@rm *.o advent -f