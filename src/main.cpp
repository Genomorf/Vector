#include "helpers.h"

int main() {
	using Vector_mi = Vector<MyInt>;
	// initialisation
	Vector_mi v1{ 1,2,3 };
	Vector_mi v2(10, 20);
	std::vector<MyInt> stdvec{ 42,42,42 };
	Vector_mi v3(stdvec.begin(), stdvec.end());
	// add value
	v1.push_back(10);
	MyInt mi1(11);
	v1.push_back(mi1);
	v1.emplace_back(12);
	v1.insert(v1.begin() + 1, 13);
	v1.insert(v1.end(), stdvec.begin(), stdvec.end());
	// resize, reserve
	v2.reserve(50);
	v2.resize(30);
	// iterators
	for (const auto& it : v1) {
		cout << it << " ";
	}
	for (auto it = v1.begin(); it != v1.end(); ++it) {
		cout << *it << " ";
	}
}