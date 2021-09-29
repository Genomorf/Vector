#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>
//using namespace std;
using std::cout;
using std::swap;
using std::move;

static const int max_reverse_value = 100'000;
static const int max_resize_value = 100'000;


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
class VectorBuffer;

template <typename T>
void swap(VectorBuffer<T>& lhs, VectorBuffer<T>& rhs);

template<typename T>
class VectorBuffer {
public:
	T* buff = nullptr;
	size_t size_ = 0;
	size_t capacity_ = 0;

	VectorBuffer(size_t capacity = 0, size_t size = 0) : capacity_(capacity), size_(size) {
		void* new_buff = ::operator new(sizeof(T) * capacity);
		buff = reinterpret_cast<T*>(new_buff);
		for (size_t i = 0; i < size; i++)
		{
			new (buff + i) T();
		}
	}
	T& operator[](size_t n) {
		return buff[n];
	}
	VectorBuffer(const VectorBuffer<T>& other) {
		VectorBuffer<T> tmp(other.capacity_, other.size_);
		for (size_t i = 0; i < other.size_; i++)
		{
			tmp.buff[i] = other.buff[i];
		}
		swap(*this, tmp);
	}

	VectorBuffer& operator=(VectorBuffer<T>& other) {
		if (this == &other) return *this;
		VectorBuffer<T> tmp(other);
		swap(*this, other);
		return *this;
	}

	VectorBuffer(VectorBuffer<T>&& other) noexcept {
		swap(other, *this);
	}

	VectorBuffer& operator=(VectorBuffer<T>&& other) noexcept {
		if (this == &other) return *this;
		swap(*this, other);
		return *this;
	}

	void destructElements() {
		for (size_t i = 0; i < size_; i++)
		{
			buff[size_ - 1 - i].~T();
		}
	}

	~VectorBuffer() {
		destructElements();
		::operator delete(buff);
	}
};

template <typename T>
void swap(VectorBuffer<T>& lhs, VectorBuffer<T>& rhs) {
	swap(lhs.buff, rhs.buff);
	swap(lhs.capacity_, rhs.capacity_);
	swap(lhs.size_, rhs.size_);
}


template <typename T>
typename std::enable_if<std::is_nothrow_move_constructible<T>::value>::type
move_if_noexcept_else_copy(T* src, T* dst, int size) {
	std::move(src, src + size, dst);
}

template <typename T>
typename std::enable_if<!std::is_nothrow_move_constructible<T>::value>::type
move_if_noexcept_else_copy(T* src, T* dst, int size) {
	std::copy(src, src + size, dst);
}

template <typename T>
class Vector {
public:
	VectorBuffer<T> buff;
	Vector(const std::initializer_list<T>& in_list) {
		VectorBuffer<T> tmpbuff(in_list.size(), in_list.size());
		size_t counter = 0;
		for (auto it = in_list.begin(); it != in_list.end(); ++it, ++counter)
		{
			tmpbuff[counter] = *it; // from std::initializer_list can only copy objects
		}
		swap(tmpbuff, buff);
	}
	Vector(size_t amount = 0, const T& value = T()) {
		VectorBuffer<T> tmpbuff(amount, amount);
		for (size_t i = 0; i < tmpbuff.size_; ++i)
		{
			tmpbuff[i] = value;
		}
		swap(tmpbuff, buff);
	}
	T& operator[] (size_t n) {
		return buff[n];
	}
	//void copyToBuff(VectorBuffer<T>& otherbuff) {
	//	for (size_t i = 0; i < buff.size_; i++)
	//	{
	//		otherbuff[i] = std::move_if_noexcept(buff[i]);
	//	}
	//}
	void reserve(size_t new_capacity) {
		if (new_capacity < buff.capacity_ || new_capacity >= max_reverse_value) return;
		VectorBuffer<T> tmpbuff(new_capacity, 0);
		for (size_t i = 0; i < buff.size_; i++)
		{
			new (tmpbuff.buff + i) T(std::move_if_noexcept(buff[i]));
		}
		tmpbuff.size_ = buff.size_;
		swap(tmpbuff, buff);
	}	
	void resize(size_t new_size) {
		if (new_size > max_resize_value) return;
		if (new_size < buff.size_) {
			buff.size_ = new_size;
		}
		else if (new_size > buff.size_) {
			if (new_size > buff.capacity_) {
				reserve(new_size);
			}
			VectorBuffer<T> tmpbuff(buff.capacity_, new_size);
			//copyToBuff(tmpbuff);
			move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
			for (size_t i = tmpbuff.size_; i < new_size; i++)
			{
				tmpbuff[i] = T();
			}
			tmpbuff.size_ = new_size;
			swap(tmpbuff, buff);
		}
	}
	// TODO:
	void pushBack(const T& value) {
		cout << "1";
		if (buff.capacity_ == buff.size_) {
			reserve(buff.capacity_ * 2);
		}
		VectorBuffer<T> tmpbuff(buff.capacity_, buff.size_);
		move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
		new (&tmpbuff.buff[buff.size_]) T(value);
		swap(tmpbuff, buff);
		++buff.size_;
	}
	void pushBack(T&& value) {
		cout << "2";
		if (buff.capacity_ == buff.size_) {
			reserve(buff.capacity_ * 2);
		}
		VectorBuffer<T> tmpbuff(buff.capacity_, buff.size_);
		move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
		new (&tmpbuff.buff[buff.size_]) T(std::move_if_noexcept(value));
		swap(tmpbuff, buff);
		++buff.size_;
	}
	/*
		template<typename T>
	void emplaceBack(T&& t) {
		if (buff.capacity_ == buff.size_) {
			MyVectorBuff tmpbuff(buff.size_ * 2, buff.size_);
			// TODO: if moveable - move, else copy
			buff.copyFromBuffToOtherBuff(tmpbuff);
			swap(tmpbuff, buff);
		}
		buff[buff.size_] = forward<T>(t);
		++buff.size_;
	}
	*/
	class Iterator {
	public:
		Vector<T>* sv = nullptr;
		int i = 0;
		Iterator(Vector<T>* sv, int i) : sv(sv), i(i) {}
		T& operator*() { return (*sv)[i]; }
		Iterator& operator++() { ++i; return *this; }
		//Iterator operator++(int) { Iterator tmp(*this); ++ptr; return tmp; }
		friend bool operator== (const Iterator& lhs, const Iterator& rhs) { return lhs.i == rhs.i; }
		friend bool operator!= (const Iterator& lhs, const Iterator& rhs) { return lhs.i != rhs.i; }
	};
	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, size()); }
	size_t capacity() const { return buff.capacity_; }
	size_t size() const { return buff.size_; }
};


struct MyInt {
	int* x_ = nullptr;
	MyInt() {
		cout << "constructed\n";
		x_ = new int(10);
	}
	MyInt(int x) {
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
struct MyIntWithNoDefaultConstr {
	int x = 10;
	MyIntWithNoDefaultConstr() = delete;
};
std::ostream& operator <<(std::ostream& out, const MyInt& mi) {
	out << *mi.x_ << " ";
	return out;
}
#include <type_traits>
template <typename T>
void printVector(Vector<T>& sv) {
	static int count = 0;
	++count;
	cout << count <<  " Capacity: " << sv.capacity() << " Size: " << sv.size() << '\n';

	for (auto it = sv.begin(), it_end = sv.end(); it != it_end; ++it) {
		cout << *it << " ";
	}
	cout << '\n';

}








//template <typename Iter, typename T>
//typename std::enable_if<!std::is_nothrow_move_constructible<T>::value>::type
//copyFromInitList(Iter first, Iter last, T* dst) {
//	std::copy(first, last, dst);
//}
//
//template <typename Iter, typename T>
//typename std::enable_if<std::is_nothrow_move_constructible<T>::value>::type
//copyFromInitList(Iter first, Iter last, T* dst) {
//	std::move(first, last, dst);
//}

int main() {
	Vector<MyInt> v{ 1,2,3 };
	printVector(v);
	//MyInt x = 1;
	//MyInt x2 = move(x);
	MyInt x = 11;
	v.pushBack(move(x));
	printVector(v);
	//copy_if_moveable(&il, x, 3);
	//for (size_t i = 0; i < 3; i++)
	//{
	//	x[i] = i;
	//}
	//MyInt* x2 = new MyInt[3];
	//std::move(x, x + 3, x2);
	//sv.resize(20);
	//printVector(sv);
	//printVectorBuffer(sv);
	//VectorBuffer<MyInt> sv2(move(sv));
	//printVectorBuffer(sv2);
	//printVectorBuffer(sv);
}