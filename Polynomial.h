#ifndef _POLYNOMIAL
#define _POLYNOMIAL

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <future>
#include <memory>

class Polynomial
{
private:
	//Using Pimpl idiom
	struct PolynomialData;
	mutable std::unique_ptr<PolynomialData> pImpl;

	mutable std::mutex integralGuard;

public:
	Polynomial(); //Default constructor
	Polynomial(const Polynomial& p); //Copy constructor
	Polynomial(Polynomial&& p); //Move constructor
	Polynomial(std::initializer_list<int> list); //Constructor for braced initialization
	Polynomial(const int value, const unsigned int exponent);
	~Polynomial(); //Destructor

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
	int GetCoefficient(const unsigned int exponent) const;
	int GetHighestCoefficient() const;

	void Scale(const int scalar);

	void AddRoot(const int root);
	template<typename T> void AddRootRange(typename T::const_iterator first, typename T::const_iterator last)
	{
		while (first != last)
		{
			this->AddRoot(*first);
			first++;
		}
	}

	double ValueAt(const double x) const;
	Polynomial CalculateDerivative() const;
	double CalculateIntegral(const int a, const int b) const;

	Polynomial& operator+=(const Polynomial& rhs);
	Polynomial& operator*=(const Polynomial& rhs);
	Polynomial& operator=(const Polynomial& p);
};

Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);
Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);
std::ostream& operator<<(std::ostream&, const Polynomial&);

#endif