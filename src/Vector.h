#pragma once

#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <type_traits>

using std::cout;
using std::swap;
using std::move;

static const int max_reverse_value = 100'000;
static const int max_resize_value = 100'000;

template<typename T>
class VectorBuffer {
public:

	T* buff = nullptr;
	size_t size_ = 0;
	size_t capacity_ = 0;

	VectorBuffer(size_t capacity = 0, size_t size = 0) : capacity_(capacity), size_(size) {
		void* new_buff = ::operator new(sizeof(T) * capacity);
		buff = static_cast<T*>(new_buff);
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
	template <typename T>
	friend void swap(VectorBuffer<T>& lhs, VectorBuffer<T>& rhs);

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

// TODO const??
template <typename T>
class Vector {
public:
	Vector(const std::initializer_list<T>& in_list) {
		VectorBuffer<T> tmpbuff(in_list.size(), 0);
		auto it = in_list.begin();
		for (size_t i = 0; i < in_list.size(); i++)
		{
			new (tmpbuff.buff + i) T(it[i]);
		}
		tmpbuff.size_ = in_list.size();
		swap(tmpbuff, buff);
	}
	Vector(size_t amount = 0, const T& value = T()) {
		VectorBuffer<T> tmpbuff(amount, 0);
		for (size_t i = 0; i < amount; ++i)
		{
			new (tmpbuff.buff + i) T(value);
		}
		tmpbuff.size_ = amount;
		swap(tmpbuff, buff);
	}
	template<typename Iter, typename = typename std::enable_if<!std::is_integral<Iter>::value>::type>
	Vector(Iter first, Iter last) {
		auto size = std::distance(first, last);
		VectorBuffer<T> tmpbuff(size, 0);
		size_t i = 0;
		for (; first != last; ++first, ++i) {
			new (tmpbuff.buff + i) T(*first);
		}
		tmpbuff.size_ = size;
		swap(tmpbuff, buff);
	}
	T& operator[] (size_t n) {
		return buff[n];
	}
	void reserve(size_t new_capacity) {
		if (new_capacity < buff.capacity_ || new_capacity >= max_reverse_value) return;
		VectorBuffer<T> tmpbuff(new_capacity, 0);
		for (size_t i = 0; i < buff.size_; i++)
		{
			// don't construct values by default, placement new right in memory
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
			VectorBuffer<T> tmpbuff(new_size, new_size);
			move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
			tmpbuff.size_ = new_size;
			swap(tmpbuff, buff);
		}
	}
	void push_back(const T& value) {
		size_t new_capacity = check_and_double_capacity();

		VectorBuffer<T> tmpbuff(new_capacity, buff.size_);
		move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
		new (&tmpbuff.buff[buff.size_]) T(value);
		swap(tmpbuff, buff);
		++buff.size_;
	}
	void push_back(T&& value) {
		size_t new_capacity = check_and_double_capacity();

		VectorBuffer<T> tmpbuff(new_capacity, buff.size_);
		move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
		new (&tmpbuff.buff[buff.size_]) T(std::move_if_noexcept(value));
		swap(tmpbuff, buff);
		++buff.size_;
	}
	template <typename ... Args>
	void emplace_back(Args&&... args) {
		size_t new_capacity = check_and_double_capacity();

		VectorBuffer<T> tmpbuff(new_capacity, buff.size_);
		move_if_noexcept_else_copy(buff.buff, tmpbuff.buff, buff.size_);
		new (&tmpbuff.buff[buff.size_]) T(std::forward<Args>(args)...);
		swap(tmpbuff, buff);
		++buff.size_;
	}

	class Iterator {
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;

		Vector<T>* sv = nullptr;
		int i = 0;
		Iterator(Vector<T>* sv, int i) : sv(sv), i(i) {}

		reference operator*()		 { return (*sv)[i]; }
		pointer   operator->()		 { return &((*sv)[i]); }
		reference operator[] (int n) { return (*sv)[i + n]; }

		Iterator& operator++()		 { ++i; return *this; }
		Iterator& operator--()		 { --i; return *this; }
		Iterator  operator++ (int)   { Iterator tmp(*this); ++i; return tmp; }
		Iterator  operator-- (int)   { Iterator tmp(*this); --i; return tmp; }
		Iterator& operator+= (int n) { i += n; return *this; }
		Iterator& operator-= (int n) { i -= n; return *this; }
		Iterator  operator+  (int n) { Iterator tmp(*this); return tmp += n; }
		Iterator  operator-  (int n) { Iterator tmp(*this); return tmp -= n; }
		difference_type operator- (const Iterator& other) { return i - other.i; }

		bool operator<(const Iterator& other)  const { return i < other.i; }
		bool operator<=(const Iterator& other) const { return i <= other.i; }
		bool operator>(const Iterator& other)  const { return i > other.i; }
		bool operator>=(const Iterator& other) const { return i >= other.i; }
		bool operator!=(const Iterator& other) const { return i != other.i; }
		bool operator==(const Iterator& other) const { return i == other.i; }
	};
	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, size()); }

	size_t capacity() const { return buff.capacity_; }
	size_t size() const { return buff.size_; }

	void insert(typename Vector<T>::Iterator pos, const T& value) {
		size_t new_capacity = check_and_double_capacity();
		Vector<T> tmp;
		tmp.reserve(new_capacity);
		for (auto it = this->begin(); it != pos; ++it) {
			tmp.push_back(*it);
		}
		tmp.push_back(value);
		for (; pos != this->end(); ++pos) {
			tmp.push_back(*pos);
		}
		swap(*this, tmp);
	}
	template <typename Iter>
	void insert(typename Vector<T>::Iterator pos, Iter first, Iter last) {
		auto dist = std::distance(first, last);
		size_t new_capacity = buff.capacity_;
		if (buff.size_ + dist >= buff.capacity_) {
			new_capacity = buff.size_ + dist;
		}
		Vector<T> tmp;
		tmp.reserve(new_capacity);
		for (auto it = this->begin(); it != pos; ++it) {
			tmp.push_back(*it);
		}
		for (; first != last; ++first) {
			tmp.push_back(*first);
		}
		for (; pos != this->end(); ++pos) {
			tmp.push_back(*pos);
		}
		swap(*this, tmp);
	}
private:
	VectorBuffer<T> buff;
	size_t  check_and_double_capacity() {
		return buff.capacity_ == buff.size_ ? buff.capacity_ * 2 + 1 : buff.capacity_;
	}
	template <typename T>
	friend void swap(Vector<T>& lhs, Vector<T>& rhs);
};

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) {
	swap(lhs.buff, rhs.buff);
}