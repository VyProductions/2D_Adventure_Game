OBJECTS=action_impl.o advent.o input_map.o input.o system.o text.o player.o map.o
CXX=g++
INCLUDE=-I./SDL2/include/SDL2 -Dmain=SDL_main
CXXFLAGS=-std=c++2a -Wall -Wextra -pedantic -Wshadow -Wconversion -Werror=vla -Werror=return-type -Wno-deprecated-copy -Wno-multichar -g
LINK=-L./SDL2/lib -lmingw32 -lSDL2main -lSDL2 -mwindows

all: clean advent
	@clear

advent: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) -o advent $(LINK)

action_impl.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) action_impl.cpp -c

advent.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) advent.cpp -c

input_map.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) input_map.cpp -c

input.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) input.cpp -c

system.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) system.cpp -c

text.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) text.cpp -c

player.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) player.cpp -c

map.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) map.cpp -c

clean:
	@rm *.o advent -f