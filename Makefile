INCLUDE=$(CURDIR)/../
all:
	g++ -g -I $(INCLUDE) -Wall -std=c++11 *.cpp -o main -lpthread

