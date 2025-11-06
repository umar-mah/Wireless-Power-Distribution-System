CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = wpower.o

mytest: $(OBJECTS) mytest.cpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) mytest.cpp -o mytest


wpower.o: wpower.cpp wpower.h
	$(CXX) $(CXXFLAGS) -c wpower.cpp

clean:
	rm *.o*
	rm *~ 

valgrind:
	valgrind ./mytest

run:
	./mytest

