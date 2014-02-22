all: array

array: Array.cpp Main.cpp Array.hpp
	rm -f *.o array
	g++ Array.cpp Main.cpp -o array -std=c++11 -g -gdwarf-2

clean:
	rm -f *.o array

