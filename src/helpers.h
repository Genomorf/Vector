#pragma once

#include "Vector.h"

struct MyInt {
	int* x_ = nullptr;
	MyInt();
	MyInt(int x);
	MyInt(const MyInt& mi);
	MyInt& operator=(const MyInt& mi);
	MyInt(MyInt&& mi) noexcept;
	MyInt& operator=(MyInt&& mi)noexcept;
	~MyInt();
	friend void swap(MyInt& lhs, MyInt& rhs);
};
void swap(MyInt& lhs, MyInt& rhs);
std::ostream& operator <<(std::ostream& out, const MyInt& mi);

template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T>& vec) {
	for (const auto& i : vec) {
		out << i;
		if (i != vec.back())
			out << " ";
	}
	out << std::endl;
	return out;
}
template <typename T>
void printVector(Vector<T>& sv) {
	static int count = 0;
	++count;
	cout << count << " Capacity: " << sv.capacity() << " Size: " << sv.size() << '\n';

	for (auto it = sv.begin(), it_end = sv.end(); it != it_end; ++it) {
		cout << *it << " ";
	}
	cout << '\n';
}