CXX := g++ -m64
#CXXFLAGS = -I./common -O2 -std=c++17 -Wall -lpthread
CXXFLAGS = -O2 -std=c++17 -Wall

serial_mode: connect_flow_serial
thread_mode: connect_flow_thread_mode
openmp_mode: connect_flow_thread_openmp

OBJS_THREAD_POOL=connect_flow_thread_pool.o Puzzle.o ThreadPool.o
OBJS_SERIAL=connect_flow_serial.o Puzzle.o
OBJS_OPENMP=connect_flow_thread_openmp.o Puzzle.o

connect_flow_thread_mode: $(OBJS_THREAD_POOL)
	$(CXX) $(CXXFLAGS) -pthread -o $@ $(OBJS_THREAD_POOL)

connect_flow_serial: $(OBJS_SERIAL)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_SERIAL)

connect_flow_thread_openmp: $(OBJS_OPENMP)
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ $(OBJS_OPENMP)

#connect_flow: connect_flow.cpp
#	$(CXX) connect_flow.cpp $(CXXFLAGS) -o ./connect_flow.out
#	$(CXX) connect_flow_thread_pool.cpp $(CXXFLAGS) -o ./connect_flow_thread_pool.out

%.o: %.cpp %.h
		$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	rm -f *.out *.o $(OBJS) connect_flow_serial connect_flow_thread_mode connect_flow_thread_openmp

.PHONY : clean
