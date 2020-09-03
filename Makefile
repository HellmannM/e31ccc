converter: main.cpp wheel.h
	c++ -o $@ -std=c++11 -I. main.cpp
