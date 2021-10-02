#include "helpers.h"

int main() {
	Vector<MyInt> v{ 1,2,3 };
	Vector<MyInt> v2(10, 0);
	std::vector<int> stdvec{ 1,2,3,4,5 };
	Vector<MyInt> v3(stdvec.begin(), stdvec.end());
}