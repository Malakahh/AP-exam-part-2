#ifndef _POLYNOMIAL
#define _POLYNOMIAL

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>

class Polynomial
{
private:
	std::vector<int> coefficients;

public:
	Polynomial();
	Polynomial(const int value, const unsigned int exponent);
	void SetCoefficient(const int value, const unsigned int exponent);
	template<typename T> void SetCoefficientRange(typename T::const_iterator first, typename T::const_iterator last, const unsigned int offset = 0)
	{
		auto exponent = offset;
		while (first + exponent - offset != last)
		{
			this->SetCoefficient(*(first + exponent - offset), exponent);
			exponent++;
		}
	}
	const int GetCoefficient(const unsigned int exponent) const;
	const int GetHighestCoefficient() const;

	void Scale(const int scalar);

	friend std::ostream& operator<<(std::ostream&, const Polynomial&);
};

std::ostream& operator<<(std::ostream&, const Polynomial&);

#endif