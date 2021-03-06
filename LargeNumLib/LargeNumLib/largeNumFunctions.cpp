#include "largeNumFunctions.h"

int resolveChar(char c_input) {
	switch (c_input) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: throw "Number contains non-numeric values!\n";
	}
}

int findFirstNonZero(std::vector<int> x) {
	int counter = 0;
	for (int i = x.size() - 1; i >= 0; i--) {
		if (x.at(i) != 0) break;
		counter++;
	}
	return counter;
}

int findIntLength(int i_input) {
	int lengthCount = 0;
	for (; i_input != 0; i_input /= 10, lengthCount++);
	return lengthCount;

}

largeInt abs(largeInt& value)
{
	value.toPositive();
	return value;
}
