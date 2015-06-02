#include "Polynomial.h"

/*******
******** 	Private Members
********/
	
	/*
template <typename DC> struct Polynomial<DC>::PolynomialData
{
	std::vector<DC> coefficients;
	std::unordered_map<int, DC> integralData;
};*/

/*******
******** 	Constructors/Destructor
********/

//Default
template <typename C> Polynomial<C>::Polynomial(): Polynomial(0,0) {}

//Copy
template <typename C> Polynomial<C>::Polynomial(const Polynomial<C>& p) : pImpl(std::make_unique<Polynomial::PolynomialData<C>>(*p.pImpl)) {}

//Move
template <typename C> Polynomial<C>::Polynomial(Polynomial<C>&& p) : pImpl(std::make_unique<Polynomial::PolynomialData<C>>())
{
	std::swap(this->pImpl, p.pImpl);
}

//Insert data, form: value * x^exponent
template <typename C> Polynomial<C>::Polynomial(const C value, const unsigned int exponent) : pImpl(std::make_unique<Polynomial::PolynomialData<C>>())
{
	this->SetCoefficient(value, exponent);
}


template <typename C> Polynomial<C>::Polynomial(std::initializer_list<C> list) : pImpl(std::make_unique<Polynomial::PolynomialData<C>>())
{
	this->SetCoefficientRange<std::initializer_list<C>>(list.begin(), list.end());
}

template <typename C> Polynomial<C>::~Polynomial() = default;

/*******
******** 	Public Members
********/

template <typename C> void Polynomial<C>::SetCoefficient(const C value, const unsigned int exponent)
{
	std::lock_guard<std::mutex> lock(this->integralGuard);
	this->pImpl->integralData.clear();

	if (exponent < this->pImpl->coefficients.size())
	{
		this->pImpl->coefficients[exponent] = value;
	}
	else
	{
		for (unsigned int i = this->pImpl->coefficients.size(); i < exponent + 1; i++)
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

template <typename C> C Polynomial<C>::GetCoefficient(const unsigned int exponent) const
{
	if (exponent >= this->pImpl->coefficients.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	return this->pImpl->coefficients[exponent];
}

template <typename C> C Polynomial<C>::GetHighestCoefficient() const
{
	return this->pImpl->coefficients.size() - 1;
}

template <typename C> void Polynomial<C>::Scale(const C scalar)
{
	for (auto i = 0; i <= this->GetHighestCoefficient(); i++)
	{
		this->SetCoefficient(this->GetCoefficient(i) * scalar, i);
	}
}

template <typename C> void Polynomial<C>::AddRoot(const C root)
{
	const auto start = this->GetHighestCoefficient();
	for (auto i = start; i >= 0; i--)
	{
		//Move value to higher exponent
		if (i == this->GetHighestCoefficient())
		{
			//Higher exponent does not yet exist
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

template <typename C> C Polynomial<C>::ValueAt(const C x) const
{
	C res = 0;

	for (auto i = 0; i <= this->GetHighestCoefficient(); i++)
	{
		res += this->GetCoefficient(i) * std::pow(x, i);
	}

	return res;
}

template <typename C> Polynomial<C> Polynomial<C>::CalculateDerivative() const
{
	Polynomial p(*this);

	for (auto i = 0; i < p.GetHighestCoefficient(); i++)
	{
		p.SetCoefficient(p.GetCoefficient(i + 1) * (i + 1), i);
	}

	//Erase highest exponent
	p.pImpl->coefficients.erase(p.pImpl->coefficients.end() - 1);

	return p;
}

template <typename C> C Polynomial<C>::CalculateIntegralDispatch(const C a, const C b, std::true_type) const
{
	static_assert(false, "Integrals for integer types are not supported");
}

template <typename C> C Polynomial<C>::CalculateIntegralDispatch(const C a, const C b, std::false_type) const
{
	auto p = this;
	auto IntegralPart = [p](const auto n){
		auto res = 0.;

		if (p->pImpl->integralData.count(n) > 0) //key exists
		{
			std::cout << "Retrieving: " << n << " from integral cache..." << std::endl;
			res = p->pImpl->integralData.find(n)->second;	
		}
		else
		{	
			std::lock_guard<std::mutex> lock(p->integralGuard);

			for (auto i = 0; i <= p->GetHighestCoefficient(); i++)
			{
				res += p->GetCoefficient(i) * std::pow(n, i + 1) / (i + 1);
			}

			p->pImpl->integralData.insert({ n, res });
		}

		return res;
	};

	auto partB = std::async(IntegralPart, b);
	auto partA = std::async(IntegralPart, a);

	return partB.get() - partA.get();
}

template <typename C> C Polynomial<C>::CalculateIntegral(const C a, const C b) const
{
	typename std::is_integral<C>::type isIntergral;
	return this->CalculateIntegralDispatch(a, b, isIntergral);
}

/*******
******** 	Operator overloads
********/

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

template <typename C> Polynomial<C>& Polynomial<C>::operator *=(const Polynomial<C>& rhs)
{
	std::lock_guard<std::mutex> lock(this->integralGuard);
	this->pImpl->integralData.clear();
	std::vector<C> res(this->GetHighestCoefficient() + rhs.GetHighestCoefficient() + 1, 0);

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

template <typename C> Polynomial<C>& Polynomial<C>::operator=(const Polynomial& p)
{
	*this->pImpl = *p.pImpl;
	return *this;
}

template <typename C> Polynomial<C> Polynomial<C>::operator+(const Polynomial<C>& rhs)
{
	Polynomial<C> p(*this);

	p += rhs;

	return p;
}

template <typename C> Polynomial<C> Polynomial<C>::operator*(const Polynomial<C>& rhs)
{
	Polynomial<C> p(*this);

	p *= rhs;

	return p;
}

template <typename CO> std::ostream& operator<<(std::ostream& s, const Polynomial<CO>& p)
{
	s << "P(x) = ";

	for (int i = p.GetHighestCoefficient(); i >= 0; i--)
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

//Integer types
template std::ostream& operator<< <int>(std::ostream&, const Polynomial<int>&);
template class Polynomial<int>;


//Floating point types

/*
template std::ostream& operator<< <float>(std::ostream&, const Polynomial<float>&);
template class Polynomial<float>;
*/

template std::ostream& operator<< <double>(std::ostream&, const Polynomial<double>&);
template class Polynomial<double>;

/*
template std::ostream& operator<< <long double>(std::ostream&, const Polynomial<long double>&);
template class Polynomial<long double>;
*/