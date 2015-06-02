/*
	Name: 			Michael Lausdahl Fuglsang
	email:			mfugls11
	Study No.:		20112699

	Code also available at:
	https://github.com/Malakahh/Ap-exam-part-2
*/

#ifndef _POLYNOMIAL
#define _POLYNOMIAL

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <future>
#include <cassert>

template <typename C> class Polynomial
{
private:
	/*
		Pimpl idiom used for move semantics.
		Solves requirement 7.
	*/
	struct PolynomialData;
	mutable std::unique_ptr<PolynomialData> pImpl;


	/*
		Mutex used for making the integral cache thread safe.
		Used in solving requirement 6.
	*/
	mutable std::mutex integralGuard;



	/*
		Integral tag dispatch, used for requirement 8.
	*/
	C CalculateIntegralDispatch(const C a, const C b, std::true_type) const;
	C CalculateIntegralDispatch(const C a, const C b, std::false_type) const;

public:
	/*
		Default constructor
		Creates a trivial Polynomial. Solves requirement 1a.
	*/
	Polynomial();

	//Copy constructor
	Polynomial(const Polynomial<C>& p); 

	//Move constructor
	Polynomial(Polynomial<C>&& p);

	/*
		Braced initialization
		You use this for creating a polynomial with specific degree coefficients. Solves requirement 1b,
			as well as the braced initializer support from requirement 5.
	*/
	Polynomial(std::initializer_list<C> list); 

	//Insert data, form: value * x^exponent
	Polynomial(const C value, const unsigned int exponent);

	~Polynomial();





	//Sets a coefficient of the form: value * x^exponent
	void SetCoefficient(const C value, const unsigned int exponent);

	/*
		Scales the polynomial by the given value.
		Solves requirement 1c.
	*/
	void Scale(const C scalar);

	/*
		Adds a root to the polynomial (by multiplying with x - root).
		Solves requirement 1d.
	*/
	void AddRoot(const C root);

	/*
		Valuates the polynomial at a given point.
		Solves requirement 1f.
	*/
	C ValueAt(const C x) const;


	
	//Gets a coefficient for a specific exponent.
	C GetCoefficient(const unsigned int exponent) const;

	//Gets the coefficient for the highest exponent.
	C GetHighestCoefficient() const;



	/*
		Computes a polynomial which is a derivative of this polynomial.
		Solves requirement 1g.
	*/
	Polynomial<C> CalculateDerivative() const;

	/*
		Computes an integral for the given interval bounds.
		Solves requirement 1h.
	*/
	C CalculateIntegral(const C a, const C b) const;

	/*
		Sets a range of coefficients, see SetCoefficient. Supports any type of container through const_iterator.
		Solves requirement 5.
	*/
	template<typename T> void SetCoefficientRange(typename T::const_iterator first, typename T::const_iterator last, const unsigned int offset = 0)
	{
		auto exponent = offset;
		while (first + exponent - offset != last)
		{
			this->SetCoefficient(*(first + exponent - offset), exponent);
			exponent++;
		}
	}

	/*
		Adds several roots at once.
		Solves requirement 1e.

		Furthermore, it supports any type of container through const_iterator.
		This solves requirement 5.
	*/
	template<typename T> void AddRootRange(typename T::const_iterator first, typename T::const_iterator last)
	{
		while (first != last)
		{
			this->AddRoot(*first);
			first++;
		}
	}



	//Calculates the sum of this and a given polynomial
	Polynomial<C>& operator+=(const Polynomial<C>& rhs);

	//Calculates the product of this and a given polynomial
	Polynomial<C>& operator*=(const Polynomial<C>& rhs);

	//Copy assignment
	Polynomial<C>& operator=(const Polynomial<C>& p);



	/*
		Returns a polynomial equal to the sum of this and given polynomial.
		Solves requirement 1i.
	*/
	Polynomial<C> operator+(const Polynomial<C>& rhs);

	/*
		Returns a polynomial equal to the product of this and a given polynomial.
		Solves requirement 1j.
	*/
	Polynomial<C> operator*(const Polynomial<C>& rhs);
};

//Pretty print
template <typename CO> std::ostream& operator<<(std::ostream&, const Polynomial<CO>&);

#endif