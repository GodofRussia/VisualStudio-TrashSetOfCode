#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#define SUCCESS_FLG 0
#define FAIL_FLG -1

class errors_definition {
private:
    std::string error_name;

public:
    errors_definition() = default;
    errors_definition(std::string error_name): error_name(error_name) { }
    ~errors_definition() = default;
    std::string get_error_name();
};

std::string errors_definition::get_error_name() {
    return error_name;
}

class long_setDot_number {
private:
    const size_t divDigits = 1000;
    const size_t sqrtDigits = 100;

	int sign;
    std::vector<int> digits;
    long exponent;

    void initFromString(const std::string& s);
    void removeZeroes();
    void normalize();

public:
    long_setDot_number();
    long_setDot_number(const long_setDot_number& x);
    long_setDot_number(long double value);
    long_setDot_number(const std::string& s);

	bool operator>(const long_setDot_number& x) const;
	bool operator<(const long_setDot_number& x) const;
	bool operator>=(const long_setDot_number& x) const;
	bool operator<=(const long_setDot_number& x) const;
	bool operator==(const long_setDot_number& x) const;
	bool operator!=(const long_setDot_number& x) const;

	long_setDot_number operator-() const;

    long_setDot_number& operator=(const long_setDot_number& x);
	long_setDot_number operator-(const long_setDot_number& x) const;
	long_setDot_number operator+(const long_setDot_number& x) const;
	long_setDot_number operator*(const long_setDot_number& x) const;
    long_setDot_number operator/(const long_setDot_number& x) const;

	long_setDot_number& operator+=(const long_setDot_number& x);
	long_setDot_number& operator-=(const long_setDot_number& x);
	long_setDot_number& operator*=(const long_setDot_number& x);
	long_setDot_number& operator/=(const long_setDot_number& x);

	long_setDot_number operator++(int);
	long_setDot_number operator--(int);

	long_setDot_number& operator++();
	long_setDot_number& operator--();


	bool isInteger() const;
	bool isEven() const;
	bool isOdd() const;
	bool isZero() const;

	long_setDot_number toInteger();

	long_setDot_number abs() const;
	long_setDot_number inverse() const;
	long_setDot_number div(const long_setDot_number& r) const;

    friend std::ostream& operator<<(std::ostream& os, const long_setDot_number& value);
    friend std::istream& operator>>(std::istream& is, long_setDot_number& value);
};

void long_setDot_number::initFromString(const std::string& s) {
	size_t index;

	if (s[0] == '-') {
		sign = -1;
		index = 1;
	}
	else {
		sign = 1;
		index = 0;
	}

	exponent = s.length() - index;

	while (index < s.length()) {
		if (s[index] == '.')
			exponent = sign == 1 ? index : index - 1;
		else
			digits.push_back(s[index] - '0');

		index++;
	}
}

void long_setDot_number::removeZeroes() {
	size_t n = std::max((long)1, exponent);

	while (digits.size() > n && digits[digits.size() - 1] == 0)
		digits.erase(digits.end() - 1);

	while (digits.size() > 1 && digits[0] == 0) {
		digits.erase(digits.begin());
		exponent--;
	}

	while (digits.size() > 1 && digits[digits.size() - 1] == 0)
		digits.erase(digits.end() - 1);

	if (isZero()) {
		exponent = 1;
		sign = 1;
	}

	normalize();
}

void long_setDot_number::normalize() {
	size_t start = std::max(exponent, (long)0);
	size_t realDigits = digits.size() - start;

	if (realDigits >= divDigits) {
		size_t count = 0;
		size_t maxCount = 0;

		size_t i = start;

		while (i < digits.size()) {
			count = 0;

			while (i < digits.size() && digits[i] == 9) {
				count++;
				i++;
			}

			if (count > maxCount)
				maxCount = count;

			i++;
		}

		if (maxCount > divDigits * 4 / 5) {
			i = digits.size() - 1;

			do {
				count = 0;

				while (i > 0 && digits[i] != 9)
					i--;

				while (i > 0 && digits[i] == 9) {
					count++;
					i--;
				}
			} while (count != maxCount);

			digits.erase(digits.begin() + i + 1, digits.end());
			digits[i]++;
		}
	}
}

long_setDot_number::long_setDot_number() {
	sign = 1;
	digits = std::vector<int>(1, 0);
	exponent = 1;
}

long_setDot_number::long_setDot_number(const long_setDot_number& x) {
	sign = x.sign;
	exponent = x.exponent;
	digits = std::vector<int>(x.digits.size());

	for (size_t i = 0; i < x.digits.size(); i++)
		digits[i] = x.digits[i];
}

long_setDot_number::long_setDot_number(long double value) {
	std::stringstream ss;
	ss << std::setprecision(15) << value;

	initFromString(ss.str());
	removeZeroes();
}

long_setDot_number::long_setDot_number(const std::string& s) {
	initFromString(s);
	removeZeroes();
}

long_setDot_number& long_setDot_number::operator=(const long_setDot_number& x) {
	if (this == &x)
		return *this;

	sign = x.sign;
	exponent = x.exponent;
	digits = std::vector<int>(x.digits.size());

	for (size_t i = 0; i < x.digits.size(); i++)
		digits[i] = x.digits[i];

	return *this;
}

bool long_setDot_number::operator>(const long_setDot_number& x) const {
	if (sign != x.sign)
		return sign > x.sign;

	if (exponent != x.exponent)
		return (exponent > x.exponent) ^ (sign == -1);

	std::vector<int> d1(digits);
	std::vector<int> d2(x.digits);
	size_t size = std::max(d1.size(), d2.size());

	while (d1.size() != size)
		d1.push_back(0);

	while (d2.size() != size)
		d2.push_back(0);

	for (size_t i = 0; i < size; i++)
		if (d1[i] != d2[i])
			return (d1[i] > d2[i]) ^ (sign == -1);

	return false;
}

bool long_setDot_number::operator<(const long_setDot_number& x) const {
	return !(*this > x || *this == x);
}

bool long_setDot_number::operator>=(const long_setDot_number& x) const {
	return *this > x || *this == x;
}

bool long_setDot_number::operator<=(const long_setDot_number& x) const {
	return *this < x || *this == x;
}

bool long_setDot_number::operator==(const long_setDot_number& x) const {
	if (sign != x.sign)
		return false;

	if (exponent != x.exponent)
		return false;

	if (digits.size() != x.digits.size())
		return false;

	for (size_t i = 0; i < digits.size(); i++)
		if (digits[i] != x.digits[i])
			return false;

	return true;
}

bool long_setDot_number::operator!=(const long_setDot_number& x) const {
	return !(*this == x);
}

long_setDot_number long_setDot_number::operator-() const {
	long_setDot_number res(*this);
	res.sign = -sign;

	return res;
}

long_setDot_number long_setDot_number::operator+(const long_setDot_number& x) const {
	if (sign == x.sign) {
		long exp1 = exponent;
		long exp2 = x.exponent;
		long exp = std::max(exp1, exp2);

		std::vector<int> d1(digits);
		std::vector<int> d2(x.digits);

		while (exp1 != exp) {
			d1.insert(d1.begin(), 0);
			exp1++;
		}

		while (exp2 != exp) {
			d2.insert(d2.begin(), 0);
			exp2++;
		}

		size_t size = std::max(d1.size(), d2.size());

		while (d1.size() != size)
			d1.push_back(0);

		while (d2.size() != size)
			d2.push_back(0);

		size_t len = 1 + size;

		long_setDot_number res;

		res.sign = sign;
		res.digits = std::vector<int>(len, 0);

		for (size_t i = 0; i < size; i++)
			res.digits[i + 1] = d1[i] + d2[i];

		for (size_t i = len - 1; i > 0; i--) {
			res.digits[i - 1] += res.digits[i] / 10;
			res.digits[i] %= 10;
		}

		res.exponent = exp + 1;
		res.removeZeroes();

		return res;
	}

	if (sign == -1)
		return x - (-(*this));

	return *this - (-x);
}

long_setDot_number long_setDot_number::operator-(const long_setDot_number& x) const {
	if (sign == 1 && x.sign == 1) {
		bool cmp = *this > x;

		long exp1 = cmp ? exponent : x.exponent;
		long exp2 = cmp ? x.exponent : exponent;
		long exp = std::max(exp1, exp2);

		std::vector<int> d1(cmp ? digits : x.digits);
		std::vector<int> d2(cmp ? x.digits : digits);

		while (exp1 != exp) {
			d1.insert(d1.begin(), 0);
			exp1++;
		}

		while (exp2 != exp) {
			d2.insert(d2.begin(), 0);
			exp2++;
		}

		size_t size = std::max(d1.size(), d2.size());

		while (d1.size() != size)
			d1.push_back(0);

		while (d2.size() != size)
			d2.push_back(0);

		size_t len = 1 + size;

		long_setDot_number res;

		res.sign = cmp ? 1 : -1;
		res.digits = std::vector<int>(len, 0);

		for (size_t i = 0; i < size; i++)
			res.digits[i + 1] = d1[i] - d2[i];

		for (size_t i = len - 1; i > 0; i--) {
			if (res.digits[i] < 0) {
				res.digits[i] += 10;
				res.digits[i - 1]--;
			}
		}

		res.exponent = exp + 1;
		res.removeZeroes();

		return res;
	}

	if (sign == -1 && x.sign == -1)
		return (-x) - (-(*this));

	return *this + (-x);
}

long_setDot_number long_setDot_number::operator*(const long_setDot_number& x) const {
	size_t len = digits.size() + x.digits.size();

	long_setDot_number res;

	res.sign = sign * x.sign;
	res.digits = std::vector<int>(len, 0);
	res.exponent = exponent + x.exponent;

	for (size_t i = 0; i < digits.size(); i++)
		for (size_t j = 0; j < x.digits.size(); j++)
			res.digits[i + j + 1] += digits[i] * x.digits[j];

	for (size_t i = len - 1; i > 0; i--) {
		res.digits[i - 1] += res.digits[i] / 10;
		res.digits[i] %= 10;
	}

	res.removeZeroes();

	return res;
}

long_setDot_number long_setDot_number::operator/(const long_setDot_number& x) const {
	long_setDot_number res = *this * x.inverse();

	size_t intPart = std::max((long)0, exponent);

	if (intPart > res.digits.size() - 1)
		return res;

	size_t i = res.digits.size() - 1 - intPart;
	size_t n = std::max((long)0, res.exponent);

	if (i > n && res.digits[i] == 9) {
		while (i > n && res.digits[i] == 9)
			i--;

		if (res.digits[i] == 9) {
			res.digits.erase(res.digits.begin() + n, res.digits.end());
			res = res + res.sign;
		}
		else {
			res.digits.erase(res.digits.begin() + i + 1, res.digits.end());
			res.digits[i]++;
		}
	}

	return res;
}

long_setDot_number& long_setDot_number::operator+=(const long_setDot_number& x) {
	return (*this = *this + x);
}

long_setDot_number& long_setDot_number::operator-=(const long_setDot_number& x) {
	return (*this = *this - x);
}

long_setDot_number& long_setDot_number::operator*=(const long_setDot_number& x) {
	return (*this = *this * x);
}

long_setDot_number& long_setDot_number::operator/=(const long_setDot_number& x) {
	return (*this = *this / x);
}

long_setDot_number long_setDot_number::operator++(int) {
	long_setDot_number res(*this);
	*this = *this + 1;

	return res;
}

long_setDot_number long_setDot_number::operator--(int) {
	long_setDot_number res(*this);
	*this = *this - 1;

	return res;
}

long_setDot_number& long_setDot_number::operator++() {
	return (*this = *this + 1);
}

long_setDot_number& long_setDot_number::operator--() {
	return (*this = *this - 1);
}

long_setDot_number long_setDot_number::inverse() const {
	if (isZero())
		throw std::string("LongDouble LongDouble::inverse() - division by zero!");

	long_setDot_number x(*this);
	x.sign = 1;

	long_setDot_number d("1");

	long_setDot_number res;
	res.sign = sign;
	res.exponent = 1;
	res.digits = std::vector<int>();

	while (x < 1) {
		x.exponent++;
		res.exponent++;
	}

	while (d < x)
		d.exponent++;

	res.exponent -= d.exponent - 1;

	size_t numbers = 0;
	size_t intPart = std::max((long)0, res.exponent);
	size_t maxNumbers = divDigits + intPart;

	do {
		int div = 0;

		while (d >= x) {
			div++;
			d -= x;
		}

		d.exponent++;
		d.removeZeroes();

		res.digits.push_back(div);
		numbers++;
	} while (!d.isZero() && numbers < maxNumbers);

	return res;
}


long_setDot_number long_setDot_number::abs() const {
	long_setDot_number res(*this);
	res.sign = 1;

	return res;
}

bool long_setDot_number::isInteger() const {
	if (exponent < 0)
		return false;

	return digits.size() <= (size_t)exponent;
}

bool long_setDot_number::isEven() const {
	if (!isInteger())
		return false;

	if (digits.size() == (size_t)exponent)
		return digits[digits.size() - 1] % 2 == 0;

	return true;
}

bool long_setDot_number::isOdd() const {
	if (!isInteger())
		return false;

	if (digits.size() == (size_t)exponent)
		return digits[digits.size() - 1] % 2 == 1;

	return false;
}

bool long_setDot_number::isZero() const {
	return digits.size() == 1 && digits[0] == 0;
}

long_setDot_number long_setDot_number::toInteger() {
	if (this->exponent <= 0)
		return long_setDot_number(0);

	long_setDot_number res;
	res.exponent = this->exponent;

	size_t i = 0;
	size_t e = this->exponent;
	res.digits.resize(this->exponent);

	while (i < this->digits.size() && i < e) {
		res.digits[i] = this->digits[i];
		i++;
	}

	while (i < e) {
		res.digits[i++] = 0;
	}

	res.digits.resize(i);

	if (i <= this->digits.size() && this->digits[i] >= 5) {
		res.digits[i - 1] += 1;
	}

	return res;
}

long_setDot_number long_setDot_number::div(const long_setDot_number& r) const {
	long_setDot_number de = (*this) / r;

	return de.toInteger();
}

std::ostream& operator<<(std::ostream& os, const long_setDot_number& value) {
	if (value.sign == -1) {
		os << '-';
		if (os.bad() || os.fail())
			throw errors_definition("Error writing in file");
	}

	if (value.exponent > 0) {
		size_t i = 0;
		size_t e = value.exponent;

		while (i < value.digits.size() && i < e) {
			os << value.digits[i++];
			if (os.bad() || os.fail())
				throw errors_definition("Error writing in file");
		}
		while (i < e) {
			os << "0";
			if (os.bad() || os.fail())
				throw errors_definition("Error writing in file");

			i++;
		}

		if (i < value.digits.size()) {
			os << ".";
			if (os.bad() || os.fail())
				throw errors_definition("Error writing in file");

			while (i < value.digits.size()) {
				os << value.digits[i++];
				if (os.bad() || os.fail())
					throw errors_definition("Error writing in file");
			}
		}
	}
	else if (value.exponent == 0) {
		os << "0.";
		if (os.bad() || os.fail())
			throw errors_definition("Error writing in file");

		for (size_t i = 0; i < value.digits.size(); i++) {
			os << value.digits[i];
			if (os.bad() || os.fail())
				throw errors_definition("Error writing in file");
		}
	}
	else {
		os << "0.";
		if (os.bad() || os.fail())
			throw errors_definition("Error writing in file");

		for (long i = 0; i < -value.exponent; i++) {
			os << "0";
			if (os.bad() || os.fail())
				throw errors_definition("Error writing in file");
		}

		for (size_t i = 0; i < value.digits.size(); i++) {
			os << value.digits[i];
			if (os.bad() || os.fail())
				throw errors_definition("Error writing in file");
		}
	}

	return os;
}

std::istream& operator>>(std::istream& is, long_setDot_number& value) {
	std::string str;
	is >> str;

	if (is.bad() || is.fail())
		throw errors_definition("Error reading file");

	value.initFromString(str);

	return is;
}


int main(int argc, char* argv[])
{
    try {
        if (argc != 4) {
            throw errors_definition("Unexpected number of arguments");
            return FAIL_FLG;
        }

		const char* in_file_name = argv[1];

		std::ifstream input_file;
		input_file.open(in_file_name, std::ios::in);

		if (!input_file.is_open()) {
			throw errors_definition("Cannot read a file");
			return FAIL_FLG;
		}

		long_setDot_number a, b;
		input_file >> a >> b;

		input_file.close();

		long_setDot_number c;
		
		c = a.div(b);

		const char* out_file_name = argv[2];

		std::ofstream output_file;
		output_file.open(out_file_name);

		output_file << c << std::endl;

		output_file.close();
        
    }

    catch(errors_definition& ob) {
		const char* file_name = argv[3];

		std::ofstream output_file;
		output_file.open(file_name, std::ios::app);

        output_file << ob.get_error_name() << std::endl;
    }
    
    return 0;
}
