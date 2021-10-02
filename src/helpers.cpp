#include "helpers.h"

MyInt::MyInt() {
		std::cout << "constructed\n";
		x_ = new int(10);
	}
MyInt::MyInt(int x) {
		std::cout << "constructed\n";
		x_ = new int(x);
	}
MyInt::MyInt(const MyInt& mi) {
		x_ = new int(*mi.x_);
		std::cout << "copied\n";
	}
MyInt& MyInt::operator=(const MyInt& mi) {
		x_ = new int(*mi.x_);
		std::cout << "copied =\n";
		return *this;
	}
MyInt::MyInt(MyInt&& mi) noexcept {
		swap(*this, mi);
		std::cout << "moved\n";
	}
MyInt& MyInt::operator=(MyInt&& mi)noexcept {
		swap(*this, mi);
		std::cout << "moved =\n";
		return *this;
	}
MyInt::~MyInt() {
		std::cout << "destructed\n";
		delete x_;
	}

void swap(MyInt& lhs, MyInt& rhs) {
	std::swap(lhs.x_, rhs.x_);
}
std::ostream& operator <<(std::ostream& out, const MyInt& mi) {
	out << *mi.x_ << " ";
	return out;
}

