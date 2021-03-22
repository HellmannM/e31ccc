CXXFLAGS = -O3 -std=c++11

all: converter

debug: CXXFLAGS = -g -std=c++11
debug: converter

converter: main.cpp event.h read.h translator.h write.h
	$(CXX) -o $@ $(CXXFLAGS) -I. main.cpp

clean:
	rm -f *.o converter
