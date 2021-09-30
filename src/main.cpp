#include "Vector.h"

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
struct MyInt {
	int* x_ = nullptr;
	MyInt() {
		cout << "constructed\n";
		x_ = new int(10);
	}
	MyInt(int x) {
		cout << "constructed\n";
		x_ = new int(x);
	}
	MyInt(const MyInt& mi) {
		x_ = new int(*mi.x_);
		cout << "copied\n";
	}
	MyInt& operator=(const MyInt& mi) {
		x_ = new int(*mi.x_);
		cout << "copied =\n";
		return *this;
	}
	MyInt(MyInt&& mi) noexcept {
		swap(*this, mi);
		cout << "moved\n";
	}
	MyInt& operator=(MyInt&& mi)noexcept {
		swap(*this, mi);
		cout << "moved =\n";
		return *this;
	}
	~MyInt() {
		cout << "destructed\n";
		delete x_;
	}
	friend void swap(MyInt& lhs, MyInt& rhs);
};
void swap(MyInt& lhs, MyInt& rhs) {
	swap(lhs.x_, rhs.x_);
}
std::ostream& operator <<(std::ostream& out, const MyInt& mi) {
	out << *mi.x_ << " ";
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
int main() {
	//std::vector<int> v{ 1,2,3 };

	Vector<MyInt> v3{ 1, 2, 3 };
	auto it = v3.begin();
	cout << *(it + 2);
	//printVector(v3);

}