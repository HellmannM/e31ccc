converter: main.cpp event.h read.h translator.h write.h
	c++ -o $@ -std=c++11 -I. main.cpp
