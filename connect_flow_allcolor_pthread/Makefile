CXX := g++ -m64
#CXXFLAGS = -I./common -O2 -std=c++17 -Wall -lpthread
CXXFLAGS = -O2 -std=c++17 -Wall -pthread

#all: connect_flow
thread_mode: connect_flow_thread_mode

OBJS=connect_flow_thread_pool.o Puzzle.o

connect_flow_thread_mode: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

#connect_flow: connect_flow.cpp
#	$(CXX) connect_flow.cpp $(CXXFLAGS) -o ./connect_flow.out
#	$(CXX) connect_flow_thread_pool.cpp $(CXXFLAGS) -o ./connect_flow_thread_pool.out

%.o: %.cpp %.h
		$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	rm -f *.out $(OBJS) connect_flow connect_flow_thread_mode

.PHONY : clean
