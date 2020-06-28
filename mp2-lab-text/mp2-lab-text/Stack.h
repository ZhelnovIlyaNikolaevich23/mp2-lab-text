#pragma once
template <class T>
class TStack {
	T *arr;
	int size;
	int num;
public:
	TStack(int _size = 10000) {
		if (_size <= 0) throw _size;
		size = _size;
		arr = new T[size];
		num = -1;
	}
	~TStack() {
		delete[] arr;
	}

	TStack(const TStack& st) {
		size = st.size;
		arr = new T[size];
		num = st.num;
		for (int i = 0; i <= num; i++) arr[i] = st.arr[i];
	}

	TStack& operator= (const TStack& st) {
		if (this != &st) {
			delete[] arr;
			size = st.size;
			arr = new T[size];
			num = st.num;
			for (int i = 0; i <= num; i++) arr[i] = st.arr[i];
		}
		return *this;
	}

	int GetSize() const {
		return num;
	}
	void Clear() {
		num = -1;
	}

	bool IsEmpty() const {
		return (num == -1);
	}

	bool IsFull() const {
		return (num == (size - 1));
	}

	void Push(T el) {
		if (IsFull())
			throw el;
		else
			arr[++num] = el;
	}

	T Pop() {
		if (num == -1)
			throw num;
		return arr[num--];
	}

	T Top() {
		if (num == -1)
			throw num;
		return arr[num];
	}
};