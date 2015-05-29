#include "Polynomial.h"

Polynomial::Polynomial(): Polynomial{0}{}

Polynomial::Polynomial(const unsigned int exponent)
{
	this->SetCoefficient(0, exponent);
}

void Polynomial::SetCoefficient(const int value, const unsigned int exponent)
{
	if (exponent < this->coefficients.size())
	{
		this->coefficients[exponent] = value;
	}
	/*else if (exponent == this->coefficients.size())
	{
		this->coefficients.push_back(value);
	}*/
	else if (exponent >= this->coefficients.size())
	{
		for (unsigned int i = coefficients.size(); i < exponent + 1; i++)
		{
			if (i < exponent)
			{
				this->coefficients.push_back(0);
			}
			else
			{
				this->coefficients.push_back(value);
			}
		}
	}
}

const int Polynomial::GetCoefficient(const unsigned int exponent) const
{
	if (exponent >= this->coefficients.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	return this->coefficients[exponent];
}

const int Polynomial::GetHighestCoefficient() const
{
	return this->coefficients.size() - 1;
}

std::ostream& operator<<(std::ostream& s, const Polynomial& p)
{
	s << "P(x) = ";

	for (int i = p.coefficients.size() - 1; i >= 0; i--)
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