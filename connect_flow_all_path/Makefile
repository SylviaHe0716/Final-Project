CXX := g++
#CXXFLAGS = -I./common -O2 -std=c++17 -Wall -lpthread
CXXFLAGS = -O2 -std=c++17 -Wall
all: connect_flow

connect_flow: connect_flow.cpp
	$(CXX) connect_flow.cpp $(CXXFLAGS) -o ./connect_flow.out

clean:
	rm -f *.out connect_flow

.PHONY : clean