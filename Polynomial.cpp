/*
	Name: 			Michael Lausdahl Fuglsang
	email:			mfugls11
	Study No.:		20112699

	Code also available at:
	https://github.com/Malakahh/Ap-exam-part-2
*/

#include "Polynomial.h"

/*******
******** 	Private Members
********/

/*
	Pimpl idiom used for move semantics.
	Solves requirement 7.
*/
template <typename C> struct Polynomial<C>::PolynomialData
{
	std::vector<C> coefficients;

	/*
		Integral cache used for requirement 6.
	*/
	std::unordered_map<int, C> integralData;
};

/*******
******** 	Constructors/Destructor
********/

/*
	Default constructor
	Creates a trivial Polynomial. Solves requirement 1a.
*/
template <typename C> Polynomial<C>::Polynomial(): Polynomial(0,0) {}

//Copy constructor
template <typename C> Polynomial<C>::Polynomial(const Polynomial<C>& p) : pImpl(std::make_unique<Polynomial<C>::PolynomialData>(*p.pImpl)) {}

//Move constructor
template <typename C> Polynomial<C>::Polynomial(Polynomial<C>&& p) : pImpl(std::make_unique<Polynomial<C>::PolynomialData>())
{
	std::swap(this->pImpl, p.pImpl);
}

/*
	Braced initialization
	You use this for creating a polynomial with specific degree coefficients. Solves requirement 1b,
		as well as the braced initializer support from requirement 5.
*/
template <typename C> Polynomial<C>::Polynomial(std::initializer_list<C> list) : pImpl(std::make_unique<Polynomial<C>::PolynomialData>())
{
	this->SetCoefficientRange<std::initializer_list<C>>(list.begin(), list.end());
}

//Insert data, form: value * x^exponent
template <typename C> Polynomial<C>::Polynomial(const C value, const unsigned int exponent) : pImpl(std::make_unique<Polynomial<C>::PolynomialData>())
{
	this->SetCoefficient(value, exponent);
}

template <typename C> Polynomial<C>::~Polynomial() = default;

/*******
******** 	Public Members
********/

//Sets a coefficient of the form: value * x^exponent
template <typename C> void Polynomial<C>::SetCoefficient(const C value, const unsigned int exponent)
{
	//Make sure to clear cache before we alter the polynomial
	std::lock_guard<std::mutex> lock(this->integralGuard);
	this->pImpl->integralData.clear();

	if (exponent < this->pImpl->coefficients.size()) //Alter value currently stored
	{
		this->pImpl->coefficients[exponent] = value;
	}
	else //exponent is higher than the currently highest
	{
		//Set new highest exponent, filling any in between with 0 values
		for (auto i = this->pImpl->coefficients.size(); i < exponent + 1; i++)
		{
			if (i < exponent)
			{
				this->pImpl->coefficients.push_back(0);
			}
			else
			{
				this->pImpl->coefficients.push_back(value);
			}
		}
	}
}

//Gets a coefficient for a specific exponent.
template <typename C> C Polynomial<C>::GetCoefficient(const unsigned int exponent) const
{
	//Throw error if requested exponent is higher than what currently exists in this polynomial.
	if (exponent >= this->pImpl->coefficients.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	return this->pImpl->coefficients[exponent];
}

//Gets the coefficient for the highest exponent.
template <typename C> C Polynomial<C>::GetHighestCoefficient() const
{
	return this->pImpl->coefficients.size() - 1;
}

/*
	Scales the polynomial by the given value.
	Solves requirement 1c.
*/
template <typename C> void Polynomial<C>::Scale(const C scalar)
{
	//Calculate scale for each term
	for (auto i = 0; i <= this->GetHighestCoefficient(); i++)
	{
		this->SetCoefficient(this->GetCoefficient(i) * scalar, i);
	}
}

/*
	Adds a root to the polynomial (by multiplying with x - root).
	Solves requirement 1d.
*/
template <typename C> void Polynomial<C>::AddRoot(const C root)
{
	for (auto i = this->GetHighestCoefficient(); i >= 0; i--)
	{
		//Move value to higher exponent
		if (i == this->GetHighestCoefficient())
		{
			//Higher exponent does not yet exist, so set its value
			this->SetCoefficient(this->GetCoefficient(i), i + 1);
		}
		else
		{
			//Higher expoent exists, so add to its value
			this->SetCoefficient(this->GetCoefficient(i) + this->GetCoefficient(i + 1), i + 1);
		}

		//Calculate current exponent value
		this->SetCoefficient(this->GetCoefficient(i) * root * -1, i);
	}
}

/*
	Valuates the polynomial at a given point.
	Solves requirement 1f.
*/
template <typename C> C Polynomial<C>::ValueAt(const C x) const
{
	C res = 0;

	//Calculate the value for the given x
	for (auto i = 0; i <= this->GetHighestCoefficient(); i++)
	{
		res += this->GetCoefficient(i) * std::pow(x, i);
	}

	return res;
}

/*
	Computes a polynomial which is a derivative of this polynomial.
	Solves requirement 1g.
*/
template <typename C> Polynomial<C> Polynomial<C>::CalculateDerivative() const
{
	//Prepare derivative polynomial by copying current one
	Polynomial p(*this);

	//Calculate derivative polynomial
	for (auto i = 0; i < p.GetHighestCoefficient(); i++)
	{
		p.SetCoefficient(p.GetCoefficient(i + 1) * (i + 1), i);
	}

	//Erase highest exponent, as it is now invalidated
	p.pImpl->coefficients.erase(p.pImpl->coefficients.end() - 1);

	return p;
}

/*
	Integral tag dispatch, used for requirement 8.
*/
template <typename C> C Polynomial<C>::CalculateIntegralDispatch(const C a, const C b, std::true_type) const
{
	/*
		Fail assertion for int types on integral data.
		Used for requirement 8.
	*/
	std::cout << "Error: Integrals for integer types are not supported" << std::endl;
	assert(false);
}

/*
	Integral tag dispatch, used for requirement 8.
*/
template <typename C> C Polynomial<C>::CalculateIntegralDispatch(const C a, const C b, std::false_type) const
{
	auto p = this;

	/*
		Lambda expression used to calculate one part of an integral.
		Solves requirement 9.

		In addition, I'm using auto here to deduce types.
		This solves requirement 3.

		Furthermore, it caches its results to minimize calculations. This is thread-safe.
		Solves requirement 6.
	*/
	auto IntegralPart = [p](const auto n){
		auto res = 0.;

		if (p->pImpl->integralData.count(n) > 0) //key exists, set only once
		{
			std::cout << "Retrieving: " << n << " from integral cache..." << std::endl;
			res = p->pImpl->integralData.find(n)->second;	
		}
		else //key doesn't exist, Calculate...
		{	
			//Lock cache, as we want to alter it
			std::lock_guard<std::mutex> lock(p->integralGuard);

			//Calculate integral
			for (auto i = 0; i <= p->GetHighestCoefficient(); i++)
			{
				res += p->GetCoefficient(i) * std::pow(n, i + 1) / (i + 1);
			}

			//Store in cache
			p->pImpl->integralData.insert({ n, res });
		}

		return res;
	};

	/*
		Run the integral part lambda concurrently.
		Solves requirement 10.
	*/
	auto partB = std::async(IntegralPart, b);
	auto partA = std::async(IntegralPart, a);

	//Get results from tasks
	return partB.get() - partA.get();
}

/*
	Computes an integral for the given interval bounds.
	Solves requirement 1h.
*/
template <typename C> C Polynomial<C>::CalculateIntegral(const C a, const C b) const
{
	/*
		Integral tag dispatch using traits.
		Solves requirement 8.
	*/
	typename std::is_integral<C>::type isIntergral;
	return this->CalculateIntegralDispatch(a, b, isIntergral);
}

/*******
******** 	Operator overloads
********/

//Calculates the sum of this and a given polynomial
template <typename C> Polynomial<C>& Polynomial<C>::operator+=(const Polynomial<C>& rhs)
{
	for (auto i = 0; i <= rhs.GetHighestCoefficient(); i++)
	{
		if (i <= this->GetHighestCoefficient())
		{
			this->SetCoefficient(rhs.GetCoefficient(i) + this->GetCoefficient(i), i);
		}
		else
		{
			this->SetCoefficient(rhs.GetCoefficient(i), i);
		}
	}

	return *this;
}

//Calculates the product of this and a given polynomial
template <typename C> Polynomial<C>& Polynomial<C>::operator *=(const Polynomial<C>& rhs)
{
	//Make sure to clear cache before we alter the polynomial
	std::lock_guard<std::mutex> lock(this->integralGuard);
	this->pImpl->integralData.clear();

	//Prepare new list
	auto res = std::vector<C>(this->GetHighestCoefficient() + rhs.GetHighestCoefficient() + 1, 0);

	//Calculate product
	for (auto i = this->GetHighestCoefficient(); i >= 0; i--)
	{
		for (auto j = rhs.GetHighestCoefficient(); j >= 0; j--)
		{
			res[i + j] += this->GetCoefficient(i) * rhs.GetCoefficient(j);
		}
	}

	this->pImpl->coefficients = res;

	return *this;
}

//Copy assignment
template <typename C> Polynomial<C>& Polynomial<C>::operator=(const Polynomial& p)
{
	*this->pImpl = *p.pImpl;
	return *this;
}

/*
	Returns a polynomial equal to the sum of this and given polynomial.
	Solves requirement 1i.
*/
template <typename C> Polynomial<C> Polynomial<C>::operator+(const Polynomial<C>& rhs)
{
	Polynomial<C> p(*this);

	p += rhs;

	return p;
}

/*
	Returns a polynomial equal to the product of this and a given polynomial.
	Solves requirement 1j.
*/
template <typename C> Polynomial<C> Polynomial<C>::operator*(const Polynomial<C>& rhs)
{
	Polynomial<C> p(*this);

	p *= rhs;

	return p;
}

//Pretty print
template <typename CO> std::ostream& operator<<(std::ostream& s, const Polynomial<CO>& p)
{
	s << "P(x) = ";

	for (auto i = p.GetHighestCoefficient(); i >= 0; i--)
	{
		s << p.GetCoefficient(i);
		if (i > 0)
		{
			if (i == 1)
			{
				s << "x + ";
			}
			else
			{
				s << "x^" << i << " + ";		
			}
		}
	}

	return s;
}

/*******
******** 	Generate specializations
********/

/*
	Used for requirement 2
*/

//Integer types
template std::ostream& operator<< <int>(std::ostream&, const Polynomial<int>&);
template class Polynomial<int>;


//Floating point types

template std::ostream& operator<< <float>(std::ostream&, const Polynomial<float>&);
template class Polynomial<float>;

template std::ostream& operator<< <double>(std::ostream&, const Polynomial<double>&);
template class Polynomial<double>;

template std::ostream& operator<< <long double>(std::ostream&, const Polynomial<long double>&);
template class Polynomial<long double>;
