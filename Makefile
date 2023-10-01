OBJECTS=action_impl.o advent.o input_map.o input.o system.o text.o player.o map.o
CXX=g++
LIBS=./SDL2/lib
CXXFLAGS=-std=c++2a -Wall -Wextra -pedantic -Wshadow -Wconversion -Werror=vla -Werror=return-type -Wno-deprecated-copy -Wno-multichar -g
LINK=-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

all: clean advent
	@clear

advent: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -L $(LIBS) $(OBJECTS) -o advent $(LINK)

action_impl.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) action_impl.cpp -c

advent.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) advent.cpp -c

input_map.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) input_map.cpp -c

input.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) input.cpp -c

system.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) system.cpp -c

text.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) text.cpp -c

player.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) player.cpp -c

map.o:
	$(CXX) $(CXXFLAGS) -L $(LIBS) map.cpp -c

clean:
	@rm *.o advent -f