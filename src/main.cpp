#include "helpers.h"
#include <chrono>
int main() {
	//Vector<MyInt> v;
	//v.push_back(10);
	//v.push_back(10);
	//v.push_back(10);
	//v.push_back(10);
	//v.push_back(10);
	//printVector(v);



//#if 0
	int n;
	std::vector<int> v;
	std::cin >> n;
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < n; i++)
	{
		v.push_back(i);
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";	

	Vector<int> v2;
	auto start2 = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < n; i++)
	{
		v2.push_back(i);
	}
	auto finish2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::cout << "Elapsed time: " << elapsed2.count() << " s\n";
#if 0
	using Vector_mi = Vector<MyInt>;
	Vector_mi v1{ 1,2,3 };
	// initialisation
	Vector_mi v2(10, 20);
	std::vector<MyInt> stdvec{ 42,42,42 };
	Vector_mi v3(stdvec.begin(), stdvec.end());
	// add value
	v1.push_back(10);
	MyInt mi1(11);
	v1.push_back(mi1);
	v1.emplace_back(12);
	//v1.insert(v1.begin() + 1, 13);
	//v1.insert(v1.end(), stdvec.begin(), stdvec.end());
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
#endif
}