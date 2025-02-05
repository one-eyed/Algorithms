#pragma once
#include <deque>
#include <iostream>
#include <type_traits>
extern unsigned int  counter;
extern int value;
using namespace std;

template<typename C>
using Iter = typename C::iterator;

using Elem = short;
using Number = deque<Elem>;
enum class Ratio { greater, less, undefined };
enum Sign { negative, positive };

inline Sign operator!(const Sign& s) {
	return s == negative ? positive : negative;
}

class BigInt
{
public:

	BigInt() 
		: number{}
		, sign{positive}
	{}

	
	template<typename T>
	BigInt(const T num);
	
	BigInt(size_t len, Elem val, Sign s = positive)
		: number(Number(len, val))
		, sign(s)
	{}
	
	BigInt(const string& num);
	BigInt(const char* num);
	BigInt(const Number& num, Sign s=positive)
		: number(num)
		, sign{s}
	{}
	
	BigInt(const BigInt& other)
		: number(other.number)
		, sign{other.sign}
	{}

	BigInt(BigInt&& other)
		: number(move(other.number))
		, sign{other.sign}
	{}

	template<typename Iter>
	BigInt(Iter first, Iter last, Sign s)
		: number(first, last)
		, sign(s)
	{}

	BigInt& operator=(const BigInt& other);
	BigInt& operator=(BigInt&& other);


	~BigInt();

	friend const BigInt operator+(const BigInt& arg);
	friend const BigInt operator-(const BigInt& arg);

	friend BigInt operator+(BigInt left, const BigInt& right);
	friend BigInt operator-(BigInt left, const BigInt& right);
	friend BigInt operator*(BigInt left, const BigInt& right);

	friend Ratio compare(const BigInt& left, const BigInt& right);

	friend bool operator<(const BigInt& left, const BigInt& right);
	friend bool operator>(const BigInt& left, const BigInt& right);
	friend bool operator==(const BigInt& left, const BigInt& right);
	friend bool operator!=(const BigInt& left, const BigInt& right);
	friend bool operator<=(const BigInt& left, const BigInt& right);
	friend bool operator>=(const BigInt& left, const BigInt& right);

	friend ostream& operator<<(ostream& os, BigInt& val);
	friend istream& operator>>(istream& is, BigInt& val);

	friend size_t fit(BigInt& left, BigInt& right);

	template<typename T>
	BigInt add_trail_zero(T count);

	BigInt operator+=(const BigInt& right);
	BigInt operator-=(const BigInt& right);
	BigInt operator*=(const BigInt& right);

	Number::const_iterator begin() const { return number.begin(); }
	Number::const_iterator end() const { return number.end(); }

	Number::iterator begin() { return number.begin(); }
	Number::iterator end() { return number.end(); }

	Elem& operator[](size_t index) { return number[index]; }
	const Elem& operator[](size_t index) const { return number[index]; }

	size_t size() const { return number.size(); }

	friend BigInt abs(const BigInt& arg);
	friend BigInt subtrct(const BigInt& left, const BigInt& right);
private:
	template<typename T>
	Number parse_numeric(T num);

	// Base operations
	BigInt add(const BigInt& right);
	BigInt sub(const BigInt& right);
	BigInt mult(BigInt right);
	BigInt change_sign(){ sign = !sign; return *this; }
	BigInt make_opposite() const { return BigInt(number, !sign); }
	
	template<typename T>
	BigInt pow10(T exp);
	bool is_zero();
	BigInt rm_zeros();

	Number number;
	Sign sign;
};

// Template methods

template<typename T>
BigInt::BigInt(const T num) {
	static_assert(is_integral<T>::value, "Required integer type of argument !!");
	sign = num < 0 ? negative : positive;
	number = parse_numeric(abs(num));
}

template<typename T>
Number BigInt::parse_numeric(T num) {
	static_assert(is_integral<T>::value, "Required integer type of argument !!");
	Number n;
	do {
		n.push_back(num % 10);
		num /= 10;
	} while (num);
	return n;
}

template<typename T>
BigInt BigInt::pow10(T exp){
	static_assert(is_integral<T>::value, "Required integer type of argument !!");
	if (this->is_zero())
		return 0;
	if (exp < 0)
		throw("Exponent is < 0");
	for (size_t i = 0; i < exp; i++)
		number.push_front(0);
	return *this;
}

template<typename T>
BigInt BigInt::add_trail_zero(T count){
	static_assert(is_integral<T>::value, "Required integer type of argument !!");
	if (count < 0)
		throw("Count is < 0");
	for (size_t i = 0; i < count; i++)
		number.push_back(0);
	return *this;
}