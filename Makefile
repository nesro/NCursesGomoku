# @file Makefile
# @author Tomas Nesrovnal nesrotom@fit.cvut.cz

################################################################################

CXX=g++
CXXFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb
CXXLIBS=-lcurses
OBJECTS=./main.o ./ai.o ./board.o ./client.o ./game.o ./local.o ./network.o ./network_exception.o ./player.o ./server.o
DOXYFILE=./Doxyfile
BINARY=./nesrotom
SOURCE_DIR=./src/

################################################################################

all: $(OBJECTS) $(DOXYFILE)
	make compile
	make doc

compile: $(BINARY)

run: $(BINARY)
	$(BINARY)

clean:
	rm -rf ./doc $(BINARY) ./*.o

doc: $(DOXYFILE)
	doxygen $(DOXYFILE)
	
################################################################################

$(BINARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BINARY) $(CXXLIBS)

main.o: $(SOURCE_DIR)main.cpp $(SOURCE_DIR)game.hpp $(SOURCE_DIR)network_exception.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

ai.o: $(SOURCE_DIR)ai.cpp $(SOURCE_DIR)ai.hpp $(SOURCE_DIR)player.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

board.o: $(SOURCE_DIR)board.cpp $(SOURCE_DIR)board.hpp $(SOURCE_DIR)game.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

client.o: $(SOURCE_DIR)client.cpp $(SOURCE_DIR)client.hpp $(SOURCE_DIR)network.hpp $(SOURCE_DIR)player.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

game.o: $(SOURCE_DIR)game.cpp $(SOURCE_DIR)game.hpp $(SOURCE_DIR)board.hpp $(SOURCE_DIR)player.hpp $(SOURCE_DIR)ai.hpp $(SOURCE_DIR)client.hpp $(SOURCE_DIR)local.hpp $(SOURCE_DIR)server.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

local.o: $(SOURCE_DIR)local.cpp $(SOURCE_DIR)local.hpp $(SOURCE_DIR)player.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

network.o: $(SOURCE_DIR)network.cpp $(SOURCE_DIR)network.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

network_exception.o: $(SOURCE_DIR)network_exception.cpp $(SOURCE_DIR)network_exception.hpp $(SOURCE_DIR)network.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

player.o: $(SOURCE_DIR)player.cpp $(SOURCE_DIR)player.hpp $(SOURCE_DIR)board.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)

server.o: $(SOURCE_DIR)server.cpp $(SOURCE_DIR)server.hpp $(SOURCE_DIR)network.hpp $(SOURCE_DIR)player.hpp $(SOURCE_DIR)game.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CXXLIBS)
