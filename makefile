all:
	g++ -std=c++20 -o accumulator -Werror main.cpp accumulator.hpp accumulator.cpp

run:
	g++ -std=c++20 -o accumulator -Werror main.cpp accumulator.hpp accumulator.cpp
	./accumulator

accumulator:
	touch accumulator

clean:
	rm -f accumulator