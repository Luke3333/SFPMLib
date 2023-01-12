// SFPMLIB
// Simple Fixed Point Math Library
//
// Developed by Luca Casagrande

#if !defined(__SFPML_HEADER__)
#define __SFPML_HEADER__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <stdint.h>
#include <exception>
#include <vector>
#include <initializer_list>
#include <type_traits>

namespace sfpmlib
{
	class FPException : public std::exception
	{
		std::string message;

	public:
		FPException(std::string const& message) : message(message) {}
		const char* what() const noexcept { return message.c_str(); }
	};

	template <typename T>
	class FixedPoint
	{
		using TX = typename std::conditional<sizeof(T) == 4, uint16_t, uint32_t>::type;
		using TY = typename std::conditional<sizeof(T) == 2, uint8_t, TX>::type;
		using Fractional = typename std::enable_if<std::is_integral<T>::value, TY>::type;
		using Integer = Fractional;
		using Whole = typename std::make_unsigned<T>::type;

		typedef union
		{
			struct
			{
				Fractional fractional;
				Integer integer;
			};
			Whole fixedPoint;
		} Value;

		typedef struct
		{
			bool sign;
			Value value;
		} FixedPointNumber;

	protected:
		FixedPointNumber number;
		static const uint32_t table[32];

		FixedPoint<T> Log10() const;
		void Split(Whole&, Whole&) const;

	public:
		using IntegerPart = typename Integer;
		using FractionalPart = typename Fractional;
		using Number = typename FixedPointNumber;

		FixedPoint()
		{
			::memset(&number, 0, sizeof(number));
		}
		FixedPoint(FixedPoint<T> const&);
		FixedPoint(bool, Integer, Fractional);
		FixedPoint(bool, Whole);
		FixedPoint(FixedPointNumber);

		FixedPoint<T> operator+(FixedPoint<T> const&);
		FixedPoint<T>& operator+=(FixedPoint<T> const&);
		FixedPoint<T> operator-(FixedPoint<T> const&);
		FixedPoint<T>& operator-=(FixedPoint<T> const&);
		FixedPoint<T> operator*(FixedPoint<T> const&);
		FixedPoint<T>& operator*=(FixedPoint<T> const&);
		FixedPoint<T> operator/(FixedPoint<T> const&);
		FixedPoint<T>& operator/=(FixedPoint<T> const&);
		bool operator==(FixedPoint<T> const&) const;
		bool operator!=(FixedPoint<T> const&) const;
		bool operator<(FixedPoint<T> const&) const;
		bool operator<=(FixedPoint<T> const&) const;
		bool operator>(FixedPoint<T> const&) const;
		bool operator>=(FixedPoint<T> const&) const;
		operator T() const { return number.value.fixedPoint; }
		operator Number() const { return number; }

		std::string ToString() const;

		static FixedPoint<T> Sqrt(FixedPoint<T> const&);
		static FixedPoint<T> Pow(FixedPoint<T> const&, int);
		static FixedPoint<T> Pow(FixedPoint<T> const&, FixedPoint<T> const&);
		static FixedPoint<T> Exp(FixedPoint<T> const&);
		static FixedPoint<T> Log(FixedPoint<T> const&, FixedPoint<T> const& = FixedPoint<T>(false, 10, 0));
		static FixedPoint<T> Sin(FixedPoint<T> const&);
		static FixedPoint<T> Cos(FixedPoint<T> const&);
		static FixedPoint<T> Tan(FixedPoint<T> const&);
		static FixedPoint<T> FromString(std::string const&);
		static bool IsNegative(FixedPoint<T> const&);
		static FixedPoint<T> Abs(FixedPoint<T> const&);
		static FixedPoint<T> Radians(FixedPoint<T> const&);
		static FixedPoint<T> Degrees(FixedPoint<T> const&);
		static FixedPoint<T> Floor(FixedPoint<T> const&);
		static FixedPoint<T> Ceil(FixedPoint<T> const&);
		static FixedPoint<T> Round(FixedPoint<T> const&);
		static FixedPoint<T> Int(FixedPoint<T> const&);
		static FixedPoint<T> Fract(FixedPoint<T> const&);
		static FixedPoint<T> Min(FixedPoint<T> a, FixedPoint<T> b) { return (a < b) ? a : b; }
		static FixedPoint<T> Max(FixedPoint<T> a, FixedPoint<T> b) { return (a > b) ? a : b; }
		static inline FixedPoint<T> FPMin();
		static inline FixedPoint<T> FPMax();
		static inline FixedPoint<T> FPEpsilon() { return T(false, 0, 1); }
	};

	template <typename T>
	const uint32_t
	FixedPoint<T>::table[32]{
		500000000,
		250000000,
		125000000,
		62500000,
		31250000,
		15625000,
		7812500,
		3906250,
		1953125,
		976562,
		488281,
		244140,
		122070,
		61035,
		30517,
		15258,
		7629,
		3814,
		1907,
		953,
		476,
		238,
		119,
		59,
		29,
		14,
		7,
		3,
		1,
		0,
		0,
		0
	};

	template <typename T>
	FixedPoint<T>::FixedPoint(FixedPoint<T> const& other)
	{
		number = other.number;
	}

	template <typename T>
	FixedPoint<T>::FixedPoint(bool sign, Integer integer, Fractional fractional)
	{
		number.sign = sign;
		number.value.integer = integer;
		number.value.fractional = fractional;
	}

	template <typename T>
	FixedPoint<T>::FixedPoint(bool sign, Whole _number)
	{
		number.sign = sign;
		number.value.fixedPoint = _number;
	}

	template <typename T>
	FixedPoint<T>::FixedPoint(FixedPointNumber fixedPointNumber)
	{
		number = fixedPointNumber;
	}

	template <typename T>
	bool
	FixedPoint<T>::IsNegative(FixedPoint<T> const& val)
	{
		return val.number.sign;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Abs(FixedPoint<T> const& val)
	{
		FixedPoint<T> number(val);
		if (IsNegative(number))
		{
			number.sign = false;
		}

		return number;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Radians(FixedPoint<T> const& degrees)
	{
		FixedPoint<T> pi(FixedPoint<T>::FromString("3.141592653"));
		FixedPoint<T> val(degrees);
		val *= pi;
		val /= FixedPoint<T>(false, 180, 0);

		return val;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Degrees(FixedPoint<T> const& radians)
	{
		FixedPoint<T> pi(FixedPoint<T>::FromString("3.141592653"));
		FixedPoint<T> val(radians);
		val *= FixedPoint<T>(false, 180, 0);
		val /= pi;

		return val;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::operator+(FixedPoint<T> const& other)
	{
		if ((number.sign && other.number.sign) || (!number.sign && !other.number.sign))
		{
			// Both addends are posive or negative
			return FixedPoint<T>(number.sign, number.value.fixedPoint + other.number.value.fixedPoint);
		}
		if (number.sign)
		{
			// 1st addend in negative, 2nd is positive
			if (number.value.fixedPoint > other.number.value.fixedPoint)
			{
				// 1st addend is greater than 2nd one
				return FixedPoint<T>(true, number.value.fixedPoint - other.number.value.fixedPoint);
			}
			// 1st addend is less or equal than 2nd one
			return FixedPoint<T>(false, other.number.value.fixedPoint - number.value.fixedPoint);
		}
		// 2nd addend is negative, 1st is positive
		if (other.number.value.fixedPoint > number.value.fixedPoint)
		{
			// 2nd addend is greater than 1st one
			return FixedPoint<T>(true, other.number.value.fixedPoint - number.value.fixedPoint);
		}
		// 2nd addend is less or equal than 1st one
		return FixedPoint<T>(false, number.value.fixedPoint - other.number.value.fixedPoint);
	}

	template <typename T>
	FixedPoint<T>&
	FixedPoint<T>::operator+=(FixedPoint<T> const& other)
	{
		*this = *this + other;
		return *this;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::operator-(FixedPoint<T> const& other)
	{
		if ((number.sign && !other.number.sign) || (!number.sign && other.number.sign))
		{
			// 1st subtracting is negative and 2nd is positive, or 1st is positive and 2nd negative
			return FixedPoint<T>(number.sign, number.value.fixedPoint + other.number.value.fixedPoint);
		}
		if (!number.sign && !other.number.sign)
		{
			// both subtractings are positive
			if (number.value.fixedPoint > other.number.value.fixedPoint)
			{
				// 1st subtracting is greater than 2nd one
				return FixedPoint<T>(false, number.value.fixedPoint - other.number.value.fixedPoint);
			}
			// 1st subtracting is less or equal than 2nd one
			return FixedPoint<T>(true, other.number.value.fixedPoint - number.value.fixedPoint);
		}
		// both subtractings are negative
		if (other.number.value.fixedPoint > number.value.fixedPoint)
		{
			// 2nd addend is greater than 1st one
			return FixedPoint<T>(false, other.number.value.fixedPoint - number.value.fixedPoint);
		}
		// 2nd addend is less or equal than 1st one
		return FixedPoint<T>(true, number.value.fixedPoint - other.number.value.fixedPoint);
	}

	template <typename T>
	FixedPoint<T>&
	FixedPoint<T>::operator-=(FixedPoint<T> const& other)
	{
		*this = *this - other;
		return *this;
	}

	// Multiplication operator
	// Given 2 numbers
	// a = 10.5 and b = 4.25 we shall obtain c = a * b = 10.5 * 4.25 = 44.625
	// We can rewrite the multiplication in the following form:
	// a * b = 10.5 * 4.25 = (10 + 0.5) * (4 + 0.25) =
	// (10 * 4) + (10 * 0.25) + (0.5 * 4) + (0.5 * 0.25) =
	// 40 + 2.5 + 2 + 0.125 = 44.625

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::operator*(FixedPoint<T> const& other)
	{
		bool isNegative = (IsNegative(*this) && !IsNegative(other)) || (!IsNegative(*this) && IsNegative(other));

		Whole part1 = number.value.integer;
		part1 *= other.number.value.integer;
		part1 <<= (sizeof(Fractional) * 8);
		Whole part2 = number.value.integer;
		part2 *= other.number.value.fractional;
		Whole part3 = number.value.fractional;
		part3 *= other.number.value.integer;
		Whole part4 = number.value.fractional;
		part4 *= other.number.value.fractional;
		part4 >>= (sizeof(Fractional) * 8);

		FixedPoint<T> value(isNegative, part1 + part2 + part3 + part4);
		return value;
	}

	template <typename T>
	FixedPoint<T>&
	FixedPoint<T>::operator*=(FixedPoint<T> const& other)
	{
		*this = *this * other;
		return *this;
	}

	// Split
	// Given a number, let's say 4.5
	// The function will return two numbers as the following:
	// first = 4 (integer part)
	// second = 0.525 [(4.2 / 4) / 2] = 1.05 / 2 = 0.525

	template <typename T>
	void
	FixedPoint<T>::Split(Whole& first, Whole& second) const
	{
		Whole fract = number.value.fixedPoint;	// 4.5 is 0x00048000 (consider 16 bits of fractional part)

		if (number.value.integer)
		{
			if (number.value.fractional)
			{
				fract /= number.value.integer;		// 4.5 / 4 = 1.125 (0x00012000)
				fract >>= 1;						// 1.125 / 2 = 0.5625 (0x00009000)
			}
			else
			{
				fract = 0;
			}
		}

		first = number.value.integer;
		second = fract;
	}

	// Division operator
	// Given 2 numbers
	// a = 10.5 and b = 2.1 we shall obtain c = a / b = 10.5 / 2.1 = 5
	// We can rewrite the division in the following form:
	// a / b = 10.5 / 2.1 = (10 + 0.5) / 2.1 = [(10 / 1.05) + (0.5 / 1.05)] / 2 = 5

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::operator/(FixedPoint<T> const& other)
	{
		if (other.number.value.fixedPoint == 0)
		{
			throw FPException("Division by zero");
		}

		bool isNegative = (IsNegative(*this) && !IsNegative(other)) || (!IsNegative(*this) && IsNegative(other));

		Whole a;
		Whole b;
		other.Split(a, b);

		FixedPointNumber n = number;
		if (a && b)
		{
			n.value.fixedPoint >>= 1;
		}

		Whole part1 = n.value.integer;
		part1 <<= (sizeof(Integer) * 8);
		Whole part2 = n.value.fractional;
		part2 <<= (sizeof(Fractional) * 8);

		if (b)
		{
			Whole r = part1 % b;
			r <<= (sizeof(Fractional) * 8);
			r /= b;

			part1 /= b;
			part1 <<= (sizeof(Fractional) * 8);
			part1 += r;

			part2 /= b;
		}
		if (a)
		{
			part1 /= a;
			part2 /= a;
			if (!b)
			{
				part2 >>= (sizeof(Fractional) * 8);
			}
		}

		FixedPoint<T> res(isNegative, static_cast<T>(part1) + part2);
		return res;
	}

	template <typename T>
	FixedPoint<T>&
	FixedPoint<T>::operator/=(FixedPoint<T> const& other)
	{
		*this = *this / other;
		return *this;
	}

	template <typename T>
	bool
	FixedPoint<T>::operator==(FixedPoint<T> const& other) const
	{
		return ((number.value.fixedPoint == other.number.value.fixedPoint) && (number.sign == other.number.sign));
	}

	template <typename T>
	bool
	FixedPoint<T>::operator!=(FixedPoint<T> const& other) const
	{
		return !operator==(other);
	}

	template <typename T>
	bool
	FixedPoint<T>::operator<(FixedPoint<T> const& other) const
	{
		if (!number.sign && !other.number.sign)
		{
			return number.value.fixedPoint < other.number.value.fixedPoint;
		}
		else if (number.sign && other.number.sign)
		{
			return number.value.fixedPoint > other.number.value.fixedPoint;
		}
		else if (number.sign)
		{
			return true;
		}

		return false;
	}

	template <typename T>
	bool
	FixedPoint<T>::operator<=(FixedPoint<T> const& other) const
	{
		if (*this < other)
		{
			return true;
		}

		return *this == other;
	}

	template <typename T>
	bool
	FixedPoint<T>::operator>(FixedPoint<T> const& other) const
	{
		if (!number.sign && !other.number.sign)
		{
			return number.value.fixedPoint > other.number.value.fixedPoint;
		}
		else if (number.sign && other.number.sign)
		{
			return number.value.fixedPoint < other.number.value.fixedPoint;
		}
		else if (number.sign)
		{
			return false;
		}

		return true;
	}

	template <typename T>
	bool
	FixedPoint<T>::operator>=(FixedPoint<T> const& other) const
	{
		if (*this > other)
		{
			return true;
		}

		return *this == other;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::FromString(std::string const& s)
	{
		std::string s2;
		std::string value;
		std::string::size_type t = s.find(".");

		if (t != std::string::npos)
		{
			value = s.substr(0, t);
			if ((t + 1) < s.size())
			{
				s2 = s.substr(t + 1);
			}
		}
		else
		{
			value = s;
		}

		if (s2.size() > 9)
		{
			// Maximum 9 digits after decimal point.
			s2 = s2.substr(0, 9);
		}
		s2.append(9 - s2.size(), '0');

		uint32_t fractionalPart = 0;
		uint32_t rightPart = atoi(s2.c_str());
		for(int i = 0; i < 32; ++i)
		{
			if ((rightPart >= table[i]) && (table[i] != 0))
			{
				fractionalPart |= (1 << (31 - i));
				rightPart -= table[i];
			}
		}

		fractionalPart >>= (32 - (sizeof(Fractional) * 8));
		FixedPoint<T> res((value[0] == '-' ? true : false), static_cast<Integer>(abs(atoi(value.c_str()))), static_cast<Fractional>(fractionalPart));

		return res;
	}

	template <typename T>
	std::string
	FixedPoint<T>::ToString() const
	{
		std::stringstream ss;
		Fractional fractional = 0;

		if (number.sign)
		{
			ss << "-";
		}
		ss << number.value.integer;
		ss << ".";

		fractional = number.value.fractional;

		uint32_t offset = 0;
		switch(sizeof(Integer))
		{
			case 1:
				// 8 bit
				offset = 8;
				break;
			case 2:
				// 16 bit
				offset = 16;
				break;
			case 4:
				// 32 bit
				offset = 32;
				break;
			default:
				throw FPException("Invalid size");
		}
		// Compute the decimal part
		uint32_t value = 0;
		for(int i = (sizeof(Fractional) * 8) - 1; i >= 0; --i)
		{
			uint32_t bit = 1 << i;
			if (fractional & bit)
			{
				value += table[offset - i - 1];
			}
		}
		// Compute pads
		uint8_t pads = 0;

		if (value < 10)
		{
			pads = 8;
		}
		else if (value < 100)
		{
			pads = 7;
		}
		else if (value < 1000)
		{
			pads = 6;
		}
		else if (value < 10000)
		{
			pads = 5;
		}
		else if (value < 100000L)
		{
			pads = 4;
		}
		else if (value < 1000000L)
		{
			pads = 3;
		}
		else if (value < 10000000L)
		{
			pads = 2;
		}
		else if (value < 100000000L)
		{
			pads = 1;
		}

		// Left pad with zeroes
		if (pads)
		{
			std::string s;
			for(uint8_t i = 0; i < pads; ++i)
			{
				s += "0";
			}
			ss << s;
		}
		// Append the decimal part
		ss << value;

		return ss.str();
	}

	// https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Babylonian_method
	// https://blogs.sas.com/content/iml/2016/05/16/babylonian-square-roots.html

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Sqrt(FixedPoint<T> const& value)
	{
		if (IsNegative(value))
		{
			// Negative number
			throw FPException("Invalid argument");
		}

		typename FixedPoint<T>::Number number = static_cast<typename FixedPoint<T>::Number>(value);

		FixedPoint<T> guess;
		if (number.value.integer <= 10)
		{
			guess = FixedPoint<T>(false, 2, 0);
		}
		else if (number.value.integer <= 100)
		{
			guess = FixedPoint<T>(false, 7, 0);
		}
		else if (number.value.integer <= 1000)
		{
			guess = FixedPoint<T>(false, 20, 0);
		}
		else if (number.value.integer <= 10000)
		{
			guess = FixedPoint<T>(false, 70, 0);
		}
		else if (number.value.integer <= 100000ul)
		{
			guess = FixedPoint<T>(false, 200, 0);
		}
		else if (number.value.integer <= 1000000ul)
		{
			guess = FixedPoint<T>(false, 700, 0);
		}
		else if (number.value.integer <= 10000000ul)
		{
			guess = FixedPoint<T>(false, 2000, 0);
		}
		else if (number.value.integer <= 100000000ul)
		{
			guess = FixedPoint<T>(false, 7000, 0);
		}
		else if (number.value.integer <= 1000000000ul)
		{
			guess = FixedPoint<T>(false, 20000, 0);
		}
		else
		{
			guess = FixedPoint<T>(false, static_cast<FixedPoint<T>::Integer>(56000), 0);
		}

		FixedPoint<T> x(guess);
		for(uint8_t i = 0; i < (sizeof(Whole) * 8); ++i)
		{
			FixedPoint<T> s(value);
			x = (x + s / x);
			x /= FixedPoint<T>(false, 2, 0);
			if (static_cast<T>(x) == 0)
			{
				break;
			}
		}

		return x;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Pow(FixedPoint<T> const& base, int exp)
	{
		if (exp == 0)
		{
			return FixedPoint<T>(false, 1, 0);
		}

		FixedPoint<T> res(base);
		for(int i = 1; i < ::abs(exp); ++i)
		{
			res *= base;
		}

		if (exp < 0)
		{
			res = FixedPoint<T>(false, 1, 0) / res;
		}

		return res;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Pow(FixedPoint<T> const& base, FixedPoint<T> const& exp)
	{
		FixedPoint<T> val(base);
		int integerPart = static_cast<int>(exp.number.value.integer);
		val = FixedPoint<T>::Pow(val, integerPart);

		if (exp.number.value.fractional)
		{
			FixedPoint<T> neper = FixedPoint<T>::FromString("2.718281828");

			FixedPoint<T> val2(base);
			val2 = FixedPoint<T>::Log(val2, neper);
			val2 *= FixedPoint<T>(false, 0, exp.number.value.fractional);
			val2 = Exp(val2);
			val *= val2;
		}

		if (exp.number.sign)
		{
			val = FixedPoint(false, 1, 0) / val;
		}

		return val;
	}

	typedef struct
	{
		bool sign;
		uint8_t pow;
		uint32_t divisor;
	} TaylorTerm;

	template <typename T>
	static FixedPoint<T>
	TaylorFormula(FixedPoint<T> const& exp, FixedPoint<T> const& initialValue, TaylorTerm const* terms, int elements)
	{
		if (!terms || !elements)
		{
			throw FPException("Invalid Taylor terms");
		}

		typename FixedPoint<T>::Number fpNumber = static_cast<FixedPoint<T>::Number>(initialValue);
		uint32_t fractional = fpNumber.value.fractional;
		fractional <<= (32 - sizeof(FixedPoint<T>::FractionalPart) * 8);
		FixedPoint<uint64_t> res(fpNumber.sign, fpNumber.value.integer, fractional);

		fpNumber = static_cast<FixedPoint<T>::Number>(exp);
		fractional = fpNumber.value.fractional;
		fractional <<= (32 - sizeof(FixedPoint<T>::FractionalPart) * 8);

		for(uint8_t i = 0; i < elements; i++)
		{
			FixedPoint<uint64_t> v(fpNumber.sign, fpNumber.value.integer, fractional);
			v = FixedPoint<uint64_t>::Pow(v, terms[i].pow);
			v /= FixedPoint<uint64_t>(terms[i].sign, terms[i].divisor, 0);

			res += v;
		}

		typename FixedPoint<uint64_t>::Number number = static_cast<FixedPoint<uint64_t>::Number>(res);
		fractional = number.value.fractional;
		fractional >>= (32 - (sizeof(FixedPoint<T>::FractionalPart) * 8));

		return FixedPoint<T>(number.sign, number.value.integer, fractional);
	}

	static TaylorTerm expTerms[]{ { false, 1, 1 }, { false, 2, 2 }, { false, 3, 6 }, { false, 4, 24 }, { false, 5, 120 }, { false, 6, 720 }, { false, 7, 5040 }, { false, 8, 40320 }, { false, 9, 362880L }, { false, 10, 3628800L }, { false, 11, 39916800L }, { false, 12, 479001600L } };

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Exp(FixedPoint<T> const& exp)
	{
		if (exp.number.value.fixedPoint == 0)
		{
			return FixedPoint<T>(false, 1, 0);
		}

		FixedPoint<T> neper = FixedPoint<T>::FromString("2.718281828");
		neper = FixedPoint<T>::Pow(neper, exp.number.value.integer);
		FixedPoint<T> initialValue(false, 1, 0);
		FixedPoint<T> remainder(TaylorFormula(Fract(exp), initialValue, expTerms, sizeof(expTerms) / sizeof(TaylorTerm)));
		FixedPoint<T> res = neper * remainder;

		if (IsNegative(exp))
		{
			res = FixedPoint<T>(false, 1, 0) / res;
		}

		return res;
	}

	// Log10
	// Given a number (let's say 368), we can write Log(368) as follows:
	// Log(368) = Log(3 * 100 * 1.22) = Log(3) + Log(100) + Log(1.22)
	// The Algorithm will split the number into these 3 components.
	// The last part Log(1.22) can be written as Log(1 + x) where x = 0.22
	// Since |x| < 1, we can compute the logarithm (natural base) by means of Taylor Series formula.
	// Then we multiply this computed value by Log<10>(e) in order to change the base from <e> to <10>.

	template <typename T>
	static FixedPoint<T> logTable[9];

	template <typename T>
	static FixedPoint<T> logN;

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Log10() const
	{
		static bool initialized = false;

		if (!initialized)
		{
			initialized = true;
			logTable<T>[0] = FixedPoint<T>::FromString("0.0");				// Log<10>(1)
			logTable<T>[1] = FixedPoint<T>::FromString("0.3010299956");		// Log<10>(2)
			logTable<T>[2] = FixedPoint<T>::FromString("0.4771212547");		// Log<10>(3)
			logTable<T>[3] = FixedPoint<T>::FromString("0.6020599913");		// Log<10>(4)
			logTable<T>[4] = FixedPoint<T>::FromString("0.6989700043");		// Log<10>(5)
			logTable<T>[5] = FixedPoint<T>::FromString("0.7781512503");		// Log<10>(6)
			logTable<T>[6] = FixedPoint<T>::FromString("0.8450980400");		// Log<10>(7)
			logTable<T>[7] = FixedPoint<T>::FromString("0.9030899869");		// Log<10>(8)
			logTable<T>[8] = FixedPoint<T>::FromString("0.9542425094");		// Log<10>(9)

			logN<T> = FixedPoint<T>::FromString("0.4342944819");			// Log<10>(e)
		}

		if (number.value.fixedPoint == 0)
		{
			throw FPException("Invalid argument");
		}
		if (IsNegative(*this))
		{
			// Negative number
			throw FPException("Invalid argument");
		}

		int count = 0;
		FixedPoint<T> val(*this);
		if (!number.value.integer && number.value.fractional)
		{
			// Compute the inverse of the argument
			val = FixedPoint<T>(false, 1, 0) / val;
		}

		FixedPoint<T> base(false, 10, 0);
		FixedPoint<T> val2(val);
		val2 /= base;
		while(val2.number.value.integer != 0)
		{
			++count;
			val2 /= FixedPoint<T>(base);
		}
		FixedPoint<T> integerPart = FixedPoint<T>::Pow(base, count);
		FixedPoint<T> mostSignificantDigit = val / integerPart;
		mostSignificantDigit.number.value.fractional = 0;

		// This section computes the log (natural base) of the last part
		// in the form 1.xxxxx by means of Taylor Series formula.
		FixedPoint<T> lastPart(false, 0);
		integerPart *= mostSignificantDigit;
		FixedPoint<T> fractional(false, 0);
		if (integerPart.number.value.fixedPoint)
		{
			fractional = val / integerPart;
		}

		if (fractional.number.value.fractional)
		{
			fractional.number.value.integer = 0;
			for(uint8_t i = 1; i <= 20; i++)
			{
				FixedPoint<T> r(fractional);
				r = FixedPoint<T>::Pow(r, i);
				r /= FixedPoint<T>((((i % 2) == 0) ? true : false), static_cast<Integer>(i), static_cast<Fractional>(0));

				lastPart += r;
			}
			// Multiply by Log10(Neper) to change base from natural to 10.
			lastPart *= logN<T>;
		}

		FixedPoint<T> res;
		if (mostSignificantDigit.number.value.fixedPoint)
		{
			res = logTable<T>[mostSignificantDigit.number.value.integer - 1];
		}
		res += FixedPoint<T>(false, count, 0);
		res += FixedPoint<T>(lastPart);

		if (!number.value.integer && number.value.fractional)
		{
			// The argument is less than 1 so the result will be negative
			// Invert the result
			res.number.sign = true;
		}

		return res;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Log(FixedPoint<T> const& arg, FixedPoint<T> const& base)
	{
		if (base <= 0)
		{
			throw FPException("Invalid argument");
		}

		return arg.Log10() / base.Log10();
	}

	static TaylorTerm sinTerms[]{ { true, 3, 6 }, { false, 5, 120 }, { true, 7, 5040 }, { false, 9, 362880L }, { true, 11, 39916800L } };

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Sin(FixedPoint<T> const& angle)
	{
		static FixedPoint<T> pi_2(FixedPoint<T>::FromString("1.570796326"));
		static FixedPoint<T> pi(FixedPoint<T>::FromString("3.141592653"));
		static FixedPoint<T> pi_3_2(FixedPoint<T>::FromString("4.712388980"));
		static FixedPoint<T> pi2(FixedPoint<T>::FromString("6.283185307"));

		bool sign = false;
		FixedPoint<T> newAngle(angle);
		if (newAngle > pi_2 && newAngle <= pi)
		{
			newAngle -= pi;
			newAngle.number.sign = true;
		}
		else if (newAngle > pi && newAngle <= pi_3_2)
		{
			sign = true;
			newAngle -= pi;
		}
		else if (newAngle > pi_3_2 && newAngle <= pi2)
		{
			sign = true;
			newAngle -= pi2;
			newAngle.number.sign = true;
		}

		FixedPoint<T> res(TaylorFormula(newAngle, newAngle, sinTerms, sizeof(sinTerms) / sizeof(TaylorTerm)));
		return FixedPoint<T>(sign, res.number.value.fixedPoint);
	}

	static TaylorTerm cosTerms[]{ { true, 2, 2 }, { false, 4, 24 }, { true, 6, 720 }, { false, 8, 40320L }, { true, 10, 3628800L }, { false, 12, 479001600L } };

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Cos(FixedPoint<T> const& angle)
	{
		static FixedPoint<T> pi_2(FixedPoint<T>::FromString("1.570796326"));
		static FixedPoint<T> pi(FixedPoint<T>::FromString("3.141592653"));
		static FixedPoint<T> pi_3_2(FixedPoint<T>::FromString("4.712388980"));
		static FixedPoint<T> pi2(FixedPoint<T>::FromString("6.283185307"));

		bool sign = false;
		FixedPoint<T> newAngle(angle);
		if ((newAngle > pi_2) && (newAngle <= pi))
		{
			sign = true;
			newAngle -= pi;
			newAngle.number.sign = true;
		}
		else if ((newAngle > pi) && (newAngle <= pi_3_2))
		{
			sign = true;
			newAngle -= pi;
		}
		else if (newAngle > pi_3_2)
		{
			newAngle -= pi2;
			newAngle.number.sign = true;
		}

		FixedPoint<T> res(TaylorFormula(newAngle, FixedPoint<T>(false, 1, 0), cosTerms, sizeof(cosTerms) / sizeof(TaylorTerm)));
		return FixedPoint<T>(sign, res.number.value.fixedPoint);
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Tan(FixedPoint<T> const& angle)
	{
		return Sin(angle) / Cos(angle);
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Floor(FixedPoint<T> const& value)
	{
		T v = static_cast<T>(value);
		v >>= (sizeof(Integer) * 8);

		if (IsNegative(value))
		{
			v += 1;
		}

		return FixedPoint<T>(value.number.sign, static_cast<Integer>(v), 0);
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Ceil(FixedPoint<T> const& value)
	{
		T v = static_cast<T>(value);
		v >>= (sizeof(Integer) * 8);

		if (!IsNegative(value))
		{
			v += 1;
		}

		return FixedPoint<T>(value.number.sign, static_cast<Integer>(v), 0);
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Round(FixedPoint<T> const& value)
	{
		FixedPoint<T> v(value);
		if (!IsNegative(v))
		{
			v += FixedPoint<T>::FromString("0.5");
		}
		else
		{
			v -= FixedPoint<T>::FromString("0.5");
		}

		v.number.value.fractional = 0;
		return v;
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Int(FixedPoint<T> const& value)
	{
		return FixedPoint<T>(value.number.sign, value.number.value.integer, 0);
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::Fract(FixedPoint<T> const& value)
	{
		return FixedPoint<T>(false, 0, value.number.value.fractional);
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::FPMin()
	{
		if (sizeof(T) == 1)
		{
			return T(true, 255u, 255u);
		}
		else if (sizeof(T) == 2)
		{
			return T(true, 65535u, 65535u);
		}
		else if (sizeof(T) == 4)
		{
			return T(true, 4294967295ul, 4294967295ul);
		}

		throw FPException("Invalid Size");
	}

	template <typename T>
	FixedPoint<T>
	FixedPoint<T>::FPMax()
	{
		if (sizeof(T) == 1)
		{
			return T(false, 255u, 255u);
		}
		else if (sizeof(T) == 2)
		{
			return T(false, 65535u, 65535u);
		}
		else if (sizeof(T) == 4)
		{
			return T(false, 4294967295ul, 4294967295ul);
		}

		throw FPException("Invalid Size");
	}

	// Helpers
	typedef FixedPoint<uint16_t> FixedPointSmall;
	typedef FixedPoint<uint32_t> FixedPointMedium;
	typedef FixedPoint<uint64_t> FixedPointLarge;

	// std::conditional
	template <typename T>
	class MyTest
	{
		using Other = typename std::conditional<sizeof(T) == 4, uint16_t, uint32_t>::type;
		using T1 = typename std::conditional<sizeof(T) == 2, uint8_t, Other>::type;
		using T2 = typename std::enable_if<std::is_integral<T>::value, T1>::type;

	public:
		T2 member;

		MyTest() : member(0) {}
	};

	// Vectors
	template <class T, int N>
	class Vec
	{
		T components[N];

	public:
		Vec();
		Vec(Vec const&);
		Vec(std::initializer_list<T>);
		Vec(std::vector<T> const&);

		Vec operator+(Vec const&);
		Vec operator+=(Vec const&);
		Vec operator-(Vec const&);
		Vec operator-=(Vec const&);
		T operator*(Vec const&);
		Vec operator*(T const&);
		Vec operator*=(T const&);
		static Vec Cross(Vec const&, Vec const&);
		static T Angle(Vec const&, Vec const&);
		T operator[](uint32_t) const;
		bool operator==(Vec const&) const;
		bool operator!=(Vec const&) const;
		T Mod() const;
		Vec Normalize() const;
		std::string ToString() const;
	};

	template <class T, int N>
	Vec<T, N>::Vec()
	{
		::memset(components, 0, N * sizeof(T));
	}

	template <class T, int N>
	Vec<T, N>::Vec(Vec const& other)
	{
		for(int i = 0; i < N; i++)
		{
			components[i] = other.components[i];
		}
	}

	template <class T, int N>
	Vec<T, N>::Vec(std::initializer_list<T> list)
	: Vec()
	{
		if (list.size() > N)
		{
			throw(FPException("Invalid size"));
		}

		int k = 0;
		for(auto const& elem : list)
		{
			components[k++] = elem;
		}
	}

	template <class T, int N>
	Vec<T, N>::Vec(std::vector<T> const& values)
	: Vec()
	{
		if (values.size() > N)
		{
			throw(FPException("Invalid size"));
		}

		int k = 0;
		for(auto const& elem : values)
		{
			components[k++] = elem;
		}
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::operator+(Vec const& other)
	{
		std::vector<T> v;
		for(int i = 0; i < N; ++i)
		{
			v.push_back(components[i] + other.components[i]);
		}

		return Vec(v);
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::operator+=(Vec const& other)
	{
		*this = *this + other;
		return *this;
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::operator-(Vec const& other)
	{
		std::vector<T> v;
		for(int i = 0; i < N; ++i)
		{
			v.push_back(components[i] - other.components[i]);
		}

		return Vec(v);
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::operator-=(Vec const& other)
	{
		*this = *this - other;
		return *this;
	}

	template <class T, int N>
	T
	Vec<T, N>::operator*(Vec const& other)
	{
		T sum(false, 0);
		for(int i = 0; i < N; ++i)
		{
			sum += (components[i] * other.components[i]);
		}

		return sum;
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::operator*(T const& number)
	{
		Vec<T, N> v(*this);
		for(int i = 0; i < N; ++i)
		{
			v.components[i] *= number;
		}

		return v;
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::operator*=(T const& number)
	{
		*this = *this * number;
		return *this;
	}

	// https://www.andreaminini.org/matematica/spazio-vettoriale/prodotto-vettoriale-spazio
	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::Cross(Vec const& first, Vec const& second)
	{
		if (N != 3)
		{
			throw FPException("Invalid size");
		}

		// 1st component
		//		| y1 y2 |
		//  det	| z1 z2 |
		// 2nd component
		//		| x1 x2 |
		// -det	| z1 z2 |
		// 3rd component
		//		| x1 x2 |
		//	det | y1 y2 |
		T components[N] = {};
		memcpy(components, first.components, N * sizeof(T));

		T det1 = components[1] * second.components[2];
		det1 -= components[2] * second.components[1];
		T det2 = components[0] * second.components[2];
		det2 -= components[2] * second.components[0];
		det2 *= T(true, 1, 0);
		T det3 = components[0] * second.components[1];
		det3 -= components[1] * second.components[0];

		return Vec<T, N>{ det1, det2, det3 };
	}

	template <class T, int N>
	T
	Vec<T, N>::Angle(Vec const& first, Vec const& second)
	{
		Vec<T, N> a(first);
		Vec<T, N> b(second);

		// Compute the cosine of the angle between the 2 vectors
		T cosine = a * b;					// Dot Product
		cosine /= (a.Mod() * b.Mod());		// Divide by the product of the 2 vectors magnitude
		if (cosine == T(false, 0))
		{
			// Angle between the 2 vectors is PI/2
			return T::FromString("1.570796326");
		}

		// Compute the sine of the angle between the 2 vectors
		T sine = T::Sqrt(T(false, 1, 0) - T::Pow(cosine, 2));

		// Compute the tangent of the angle between the 2 vectors
		T tangent = sine / cosine;

		// Compute the arctan in order to get the angle between the 2 vectors.
		// Use Taylor formula.
		T res(false, 0);
		for(uint8_t i = 0; i < 10; i++)
		{
			T val = T::Pow(tangent, (2 * i) + 1);
			val /= T(((i % 2) == 0) ? false : true, (2 * i) + 1, 0);

			res += val;
		}

		return res;
	}

	template <class T, int N>
	T
	Vec<T, N>::operator[](uint32_t index) const
	{
		if (index >= N)
		{
			throw FPException("Subscript out of range");
		}

		return components[index];
	}

	template <class T, int N>
	bool
	Vec<T, N>::operator==(Vec const& other) const
	{
		for(int i = 0; i < N; ++i)
		{
			if (components[i] != other.components[i])
			{
				return false;
			}
		}

		return true;
	}

	template <class T, int N>
	bool
	Vec<T, N>::operator!=(Vec const& other) const
	{
		return !operator==(other);
	}

	template <class T, int N>
	T
	Vec<T, N>::Mod() const
	{
		T sum(false, 0);
		for(int i = 0; i < N; ++i)
		{
			sum += T::Pow(components[i], 2);
		}

		return T::Sqrt(sum);
	}

	template <class T, int N>
	Vec<T, N>
	Vec<T, N>::Normalize() const
	{
		T mod = Mod();
		Vec<T, N> res(*this);

		for(auto& component : res.components)
		{
			component /= mod;
		}

		return res;
	}

	template <class T, int N>
	std::string
	Vec<T, N>::ToString() const
	{
		std::stringstream ss;
		ss << "(";
		for(int i = 0; i < N; i++)
		{
			ss << components[i].ToString();
			if (i != (N - 1))
			{
				ss << ", ";
			}
		}
		ss << ")";

		return ss.str();
	}

	// Helpers
	typedef Vec<FixedPointSmall, 2> Vec2Small;
	typedef Vec<FixedPointMedium, 2> Vec2Medium;
	typedef Vec<FixedPointLarge, 2> Vec2Large;
	typedef Vec<FixedPointSmall, 3> Vec3Small;
	typedef Vec<FixedPointMedium, 3> Vec3Medium;
	typedef Vec<FixedPointLarge, 3> Vec3Large;
	typedef Vec<FixedPointSmall, 4> Vec4Small;
	typedef Vec<FixedPointMedium, 4> Vec4Medium;
	typedef Vec<FixedPointLarge, 4> Vec4Large;

	// Matrices
	template <class T, int N>
	class Mat
	{
		T components[N][N];

	public:
		Mat(bool = false);
		Mat(Mat const&);
		Mat(std::initializer_list<T>);
		Mat(std::vector<T> const&);

		Mat operator+(Mat const&);
		Mat operator+=(Mat const&);
		Mat operator-(Mat const&);
		Mat operator-=(Mat const&);
		Mat operator*(T const&);
		Mat operator*=(T const&);
		Mat operator*(Mat const&);
		Mat operator*=(Mat const&);
		Vec<T, N> operator[](uint32_t) const;
		bool operator==(Mat const&) const;
		bool operator!=(Mat const&) const;

		Mat<T, N - 1> ExtractMatrix(uint8_t, uint8_t);
		Mat CoFactorMatrix();
		T Det() const;
		void Inv();
		void Transpose();
		static Mat Inv(Mat const&);
		static Mat Transpose(Mat const&);
		std::string ToString() const;
	};

	template <class T, int N>
	Mat<T, N>::Mat(bool toIdentity)
	{
		::memset(components, 0, N * N * sizeof(T));
		if (toIdentity)
		{
			for(int row = 0; row < N; row++)
			{
				for(int column = 0; column < N; column++)
				{
					if (row == column)
					{
						components[row][column] = T(false, 1, 0);
					}
				}
			}
		}
	}

	template <class T, int N>
	Mat<T, N>::Mat(Mat const& other)
	{
		::memcpy(components, other.components, sizeof(T) * N * N);
	}

	template <class T, int N>
	Mat<T, N>::Mat(std::initializer_list<T> list)
	: Mat()
	{
		if (list.size() > (N * N))
		{
			throw(FPException("Invalid size"));
		}

		int row = 0;
		int column = 0;
		for(auto const& elem : list)
		{
			components[row][column++] = elem;
			if (column >= N)
			{
				++row;
				column = 0;
			}
		}
	}

	template <class T, int N>
	Mat<T, N>::Mat(std::vector<T> const& values)
	: Mat()
	{
		if (values.size() > (N * N))
		{
			throw(FPException("Invalid size"));
		}

		int row = 0;
		int column = 0;
		for(auto const& elem : values)
		{
			components[row][column++] = elem;
			if (column >= N)
			{
				++row;
				column = 0;
			}
		}
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator+(Mat const& other)
	{
		Mat<T, N> result;
		for(int row = 0; row < N; ++row)
		{
			for(int column = 0; column < N; ++column)
			{
				result.components[row][column] = components[row][column] + other.components[row][column];
			}
		}

		return result;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator+=(Mat const& other)
	{
		*this = *this + other;
		return *this;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator-(Mat const& other)
	{
		Mat<T, N> result;
		for(int row = 0; row < N; ++row)
		{
			for(int column = 0; column < N; ++column)
			{
				result.components[row][column] = components[row][column] - other.components[row][column];
			}
		}

		return result;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator-=(Mat const& other)
	{
		*this = *this - other;
		return *this;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator*(T const& value)
	{
		Mat<T, N> result;
		for(int row = 0; row < N; ++row)
		{
			for(int column = 0; column < N; ++column)
			{
				result.components[row][column] = components[row][column] * value;
			}
		}

		return result;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator*=(T const& value)
	{
		*this = *this * value;
		return *this;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator*(Mat const& other)
	{
		Mat<T, N> result;

		for(int row = 0; row < N; row++)
		{
			for(int column = 0; column < N; column++)
			{
				for(int index = 0; index < N; index++)
				{
					result.components[row][column] += components[row][index] * other.components[index][column];
				}
			}
		}

		return result;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::operator*=(Mat const& other)
	{
		*this = *this * other;
		return *this;
	}

	template <class T, int N>
	Vec<T, N>
	Mat<T, N>::operator[](uint32_t row) const
	{
		if (row >= N)
		{
			throw FPException("Subscript out of range");
		}

		std::vector<T> values;
		for(int column = 0; column < N; column++)
		{
			values.push_back(components[row][column]);
		}

		return Vec<T, N>(values);
	}
	
	template <class T, int N>
	bool
	Mat<T, N>::operator==(Mat const& other) const
	{
		for(int row = 0; row < N; ++row)
		{
			for(int column = 0; column < N; ++column)
			{
				if (components[row][column] != other.components[row][column])
				{
					return false;
				}
			}
		}

		return true;
	}

	template <class T, int N>
	bool
	Mat<T, N>::operator!=(Mat const& other) const
	{
		return !operator==(other);
	}

	template <class T, int N>
	T
	Mat<T, N>::Det() const
	{
		return ::Det<T, N>(*this);
	}

	template <class T, int N>
	Mat<T, N - 1>
	Mat<T, N>::ExtractMatrix(uint8_t row, uint8_t column)
	{
		if (N <= 2)
		{
			throw FPException("Invalid size");
		}

		std::vector<T> values;
		for(uint8_t i = 0; i < N; i++)
		{
			for(uint8_t j = 0; j < N; j++)
			{
				if ((i != row) && (j != column))
				{
					values.push_back(components[i][j]);
				}
			}
		}

		return Mat<T, N - 1>(values);
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::CoFactorMatrix()
	{
		std::vector<T> values;
		for(uint8_t row = 0; row < N; row++)
		{
			for(uint8_t column = 0; column < N; column++)
			{
				T coeff((((row + column) % 2) == 0) ? false : true, 1, 0);
				Mat<T, N - 1> c(ExtractMatrix(row, column));
				
				values.push_back(coeff * c.Det());
			}
		}

		return Mat<T, N>(values);
	}

	template <class T, int N>
	void
	Mat<T, N>::Inv()
	{
		T det = Det();
		if (det == T(false, 0, 0))
		{
			throw FPException("Determinant equal to 0");
		}

		Mat<T, N> cofactorMatrix(CoFactorMatrix());
		cofactorMatrix.Transpose();
		T invDet = T(false, 1, 0) / det;
		cofactorMatrix *= invDet;

		*this = cofactorMatrix;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::Inv(Mat const& m)
	{
		Mat<T, N> result(m);
		result.Inv();

		return result;
	}

	template <class T, int N>
	void
	Mat<T, N>::Transpose()
	{
		Mat<T, N> result;

		for(int row = 0; row < N; row++)
		{
			for(int column = 0; column < N; column++)
			{
				result.components[column][row] = components[row][column];
			}
		}

		*this = result;
	}

	template <class T, int N>
	Mat<T, N>
	Mat<T, N>::Transpose(Mat const& m)
	{
		Mat<T, N> result(m);
		result.Transpose();

		return result;
	}

	template <class T, int N>
	std::string
	Mat<T, N>::ToString() const
	{
		std::stringstream ss;
		ss << "[";

		for(int row = 0; row < N; row++)
		{
			ss << "(";
			for(int column = 0; column < N; column++)
			{
				ss << components[row][column].ToString();
				if (column != (N - 1))
				{
					ss << ", ";
				}
			}
			ss << ")";
			if (row != (N - 1))
			{
				ss << ", ";
			}
		}

		ss << "]";
		return ss.str();
	}

	// SFINAE
	template <class T, int N>
	static T
	Det(typename std::enable_if<N == 2, Mat<T, N>>::type m)
	{
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}

	// Laplace rows develop
	template <class T, int N>
	static T
	Det(typename std::enable_if<N != 2, Mat<T, N>>::type m)
	{
		T result(false, 0, 0);

		// Iterate all the columns of the 1st row (develop is made over the 1st row)
		for(int i = 0; i < N; i++)
		{
			T coeff(((i % 2) == 0) ? false : true, 1, 0);

			// Compute Cofactor matrix
			std::vector<T> values;
			for(int j = 1; j < N; j++)
			{
				for(int k = 0; k < N; k++)
				{
					if (k != i)
					{
						values.push_back(m[j][k]);
					}
				}
			}
			Mat<T, N - 1> mm(values);
			// Compute partial result
			result += coeff * m[0][i] * Det<T, N - 1>(mm);
		}

		return result;
	}

	// Helpers
	typedef Mat<FixedPointSmall, 2> Mat2x2Small;
	typedef Mat<FixedPointMedium, 2> Mat2x2Medium;
	typedef Mat<FixedPointLarge, 2> Mat2x2Large;
	typedef Mat<FixedPointSmall, 3> Mat3x3Small;
	typedef Mat<FixedPointMedium, 3> Mat3x3Medium;
	typedef Mat<FixedPointLarge, 3> Mat3x3Large;
	typedef Mat<FixedPointSmall, 4> Mat4x4Small;
	typedef Mat<FixedPointMedium, 4> Mat4x4Medium;
	typedef Mat<FixedPointLarge, 4> Mat4x4Large;
}
#endif
