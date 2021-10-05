#include "helpers.h"

MyInt::MyInt() {
#ifdef _DEBUG
	std::cout << "constructed\n";
#endif
	x_ = new int(10);
	}
MyInt::MyInt(int x) {
#ifdef _DEBUG
	std::cout << "constructed\n";
#endif
	x_ = new int(x);
	}
MyInt::MyInt(const MyInt& mi) {
	x_ = new int(*mi.x_);
#ifdef _DEBUG
	std::cout << "copied\n";
#endif
	}
MyInt& MyInt::operator=(const MyInt& mi) {
	x_ = new int(*mi.x_);
#ifdef _DEBUG
	std::cout << "copied =\n";
#endif
	return *this;
	}
MyInt::MyInt(MyInt&& mi) noexcept {
	swap(*this, mi);
#ifdef _DEBUG
	std::cout << "moved\n";
#endif
	}
MyInt& MyInt::operator=(MyInt&& mi)noexcept {
	swap(*this, mi);
#ifdef _DEBUG
	std::cout << "moved =\n";
#endif
	return *this;
	}
MyInt::~MyInt() {
#ifdef _DEBUG
	std::cout << "destructed\n";
#endif
	delete x_;
	}

void swap(MyInt& lhs, MyInt& rhs) {
	std::swap(lhs.x_, rhs.x_);
}
std::ostream& operator <<(std::ostream& out, const MyInt& mi) {
	out << *mi.x_ << " ";
	return out;
}

