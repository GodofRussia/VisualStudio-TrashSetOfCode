#include <iostream>
#include <cmath>
#include <cstring>

class MyString {
private:
	char* buf = nullptr;
	unsigned int size = 0;

public:
	MyString() : buf(nullptr), size(0) {}  // default constructor

	MyString(const char* buffer);  // constructor
	MyString(const MyString& obj);  // copy constructor
	MyString(MyString&& dyingObj);  // move constructor

	~MyString() {  // destructor
		__cleanup__();
	}

	MyString& operator=(const MyString& obj);  // copy assignment
	MyString& operator=(MyString&& dyingObj);  // move assignment

	MyString operator+(const MyString& obj) const;  // concatenation

	MyString& operator+=(const MyString& obj); // assignment concatenation

	bool operator==(const MyString& obj) const; // equivalent operator
	bool operator!=(const MyString& obj) const; // non-equivalent operator

	char& operator[](int i) const {
		return buf[i];
	}

	unsigned int length() const {
		return size;
	}

	const char* c_str() const {
		return buf;
	}

	MyString substr(int count, int pos = 0) const;
	int find(const MyString& obj, int pos = 0) const;
	MyString& insert(int index, const MyString& obj);

	friend
		std::ostream& operator<<(std::ostream& os, const MyString& obj);

private:
	void __cleanup__() {
		if (buf != nullptr) {
			delete[] buf;
		}

		size = 0;
	}
};

MyString::MyString(const char* buffer) {
	size = strlen(buffer);

	buf = new char[size + 1]; // + 1 for the keeping the null character
	strncpy_s(buf, size + 1, buffer, size); // copy from the incoming buffer to character buffer of the new object
}

MyString::MyString(const MyString& obj) {
	size = obj.size;
	buf = new char[size + 1]; // + 1 for the keeping the null character
	strncpy_s(buf, size + 1, obj.buf, size); // copy from the incoming buffer to character buffer of the new object
}

MyString::MyString(MyString&& dyingObj) {
	// && is a reference operator defined in the C++11 standard 
	// which means "dyingObj" is an r-value reference.
	// Compiler calls this constructor when the object passed in the argument
	// is about to die due to scope end or such

	__cleanup__(); // cleanup any existing data

	// Copy data from the incoming object
	size = dyingObj.size;

	// Transfer ownership of underlying char buffer from incoming object to this object
	buf = dyingObj.buf;
	dyingObj.buf = nullptr;
}

MyString& MyString::operator=(const MyString& obj) {
	__cleanup__(); // cleanup any existing data

	// Copy data from the newly assigned my_string object
	size = obj.size;
	buf = new char[size + 1]; // + 1 for the keeping the null character
	strncpy_s(buf, size + 1, obj.buf, size); // copy from the incoming buffer to character buffer of the new object
	return *this;
}

MyString& MyString::operator=(MyString&& dyingObj) {  // move assignment
	__cleanup__(); // cleanup any existing data

	// Copy data from the incoming object
	size = dyingObj.size;

	// Transfer ownership of underlying char buffer from incoming object to this object
	buf = dyingObj.buf;
	dyingObj.buf = nullptr;

	return *this;
}

MyString MyString::operator+(const MyString& obj) const{  // concatenation
	MyString s; // create a new string named 's'
	s.size = this->size + obj.size;

	s.buf = new char[s.size + 1]; // allocate memory to keep the concatenated string
	strncpy_s(s.buf, this->size + 1, this->buf, this->size); // copy the 1st string
	strncpy_s(s.buf + this->size, obj.size + 1, obj.buf, obj.size); // copy the 2nd string in buf

	return s;
}

MyString& MyString::operator+=(const MyString& obj) {
	*this = *this + obj;

	return *this;
}

bool MyString::operator==(const MyString& obj) const {
	if (this->size != obj.size)
		return false;

	if (strcmp(this->c_str(), obj.c_str()))
		return false;

	return true;
}

bool MyString::operator!=(const MyString& obj) const {
	if (*this == obj)
		return false;

	return true;
}
MyString MyString::substr(int count, int pos) const {
	if (pos > this->length()) {
		MyString obj;
		return obj;
	}

	int subSize = count;
	if (pos + count > this->length())
		subSize = this->length() - (pos + 1);

	char* buffer = new char[subSize + 1];
	strncpy_s(buffer, subSize + 1, this->buf + pos, subSize);

	MyString s(buffer);

	return s;
}

int MyString::find(const MyString& obj, int pos) const {
	if (pos > this->length() || obj.length() > this->length() - (pos + 1))
		return std::numeric_limits<int>::max();

	int subSize = obj.length();
	int result = std::numeric_limits<int>::max();

	for (int i = pos; i < this->length() - (subSize - 1); ++i) {
		MyString subs = this->substr(subSize, i);

		if (subs == obj) {
			result = i;
			break;
		}
	}

	return result;
}

MyString& MyString::insert(int index, const MyString& obj) {
	if (index > this->length())
		return *this;

	MyString s;
	s.size = this->size + obj.size;
	s.buf = new char[s.size + 1];

	strncpy_s(s.buf, index + 1, this->buf, index);
	strncpy_s(s.buf + index, obj.size + 1, obj.buf, obj.size);
	strncpy_s(s.buf + index + obj.size, this->size - index + 1, this->buf + index, this->size - index);

	*this = s;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const MyString& obj) {
	os << obj.c_str();

	return os;
}


int main() {
	MyString a("NameFidggg Nars f4f gtNamefef3r33ff3 3t44ameName");
	MyString b("Name");

	int i = 0;
	int kol = 0;

	while (i < a.length()) {
		int pos = a.find(b, i);

		if (pos == std::numeric_limits<int>::max())
			break;

		i = pos + 1;
		kol++;
	}

	std::cout << "In string: " << a << std::endl << "Substring: " << b << std::endl;
	std::cout << "Number of substring in strings = " << kol << std::endl;
	return 0;
}
