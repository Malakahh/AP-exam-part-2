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

template <typename C> class Polynomial
{
private:
	//Using Pimpl idiom
	template <typename DC> struct PolynomialData
	{
		std::vector<DC> coefficients;
		std::unordered_map<int, DC> integralData;
	};
	mutable std::unique_ptr<PolynomialData<C>> pImpl;

	mutable std::mutex integralGuard;

	//for tag dispatch
	C CalculateIntegralDispatch(const C a, const C b, std::true_type) const;
	C CalculateIntegralDispatch(const C a, const C b, std::false_type) const;

public:
	Polynomial(); //Default constructor
	Polynomial(const Polynomial<C>& p); //Copy constructor
	Polynomial(Polynomial<C>&& p); //Move constructor
	Polynomial(std::initializer_list<C> list); //Constructor for braced initialization
	Polynomial(const C value, const unsigned int exponent);
	~Polynomial(); //Destructor

	void SetCoefficient(const C value, const unsigned int exponent);
	void AddRoot(const C root);
	void Scale(const C scalar);
	C ValueAt(const C x) const;

	C GetCoefficient(const unsigned int exponent) const;
	C GetHighestCoefficient() const;

	Polynomial<C> CalculateDerivative() const;
	C CalculateIntegral(const C a, const C b) const;

	template<typename T> void SetCoefficientRange(typename T::const_iterator first, typename T::const_iterator last, const unsigned int offset = 0)
	{
		auto exponent = offset;
		while (first + exponent - offset != last)
		{
			this->SetCoefficient(*(first + exponent - offset), exponent);
			exponent++;
		}
	}

	template<typename T> void AddRootRange(typename T::const_iterator first, typename T::const_iterator last)
	{
		while (first != last)
		{
			this->AddRoot(*first);
			first++;
		}
	}

	Polynomial<C>& operator+=(const Polynomial<C>& rhs);
	Polynomial<C>& operator*=(const Polynomial<C>& rhs);
	Polynomial<C>& operator=(const Polynomial<C>& p);

	Polynomial<C> operator+(const Polynomial<C>& rhs);
	Polynomial<C> operator*(const Polynomial<C>& rhs);
};

template <typename CO> std::ostream& operator<<(std::ostream&, const Polynomial<CO>&);

#endif