all: project1

project1: main.cpp proj1.cpp proj1.h
	g++ -std=c++11 main.cpp proj1.cpp -o predictors

.PHONY: clean

clean:
	rm -f *o predictors