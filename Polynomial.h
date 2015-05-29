#ifndef _POLYNOMIAL
#define _POLYNOMIAL

#include <iostream>
#include <vector>
#include <stdexcept>

class Polynomial
{
private:
	std::vector<int> coefficients;

public:
	Polynomial();
	Polynomial(const unsigned int exponent);
	void SetCoefficient(const int value, const unsigned int exponent);
	const int GetCoefficient(const unsigned int exponent) const;
	const int GetHighestCoefficient() const;

	friend std::ostream& operator<<(std::ostream&, const Polynomial&);
};

std::ostream& operator<<(std::ostream&, const Polynomial&);

#endif