#include "Polynomial.h"

/*******
******** 	Constructors
********/

Polynomial::Polynomial(): Polynomial(0,0) {}

Polynomial::Polynomial(const Polynomial& p) : coefficients(std::vector<int>(p.coefficients)), integralData(std::unordered_map<int, double>(p.integralData)) {}

Polynomial::Polynomial(Polynomial&& p) : coefficients(std::move(p.coefficients)), integralData(std::move(p.integralData)) {}

Polynomial::Polynomial(const int value, const unsigned int exponent)
{
	this->SetCoefficient(value, exponent);
}

Polynomial::Polynomial(std::initializer_list<int> list)
{
	this->SetCoefficientRange<std::initializer_list<int>>(list.begin(), list.end());
}

/*******
******** 	Public methods
********/

void Polynomial::SetCoefficient(const int value, const unsigned int exponent)
{
	std::lock_guard<std::mutex> lock(this->integralGuard);
	this->integralData.clear();

	if (exponent < this->coefficients.size())
	{
		this->coefficients[exponent] = value;
	}
	else
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

int Polynomial::GetCoefficient(const unsigned int exponent) const
{
	if (exponent >= this->coefficients.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	return this->coefficients[exponent];
}

int Polynomial::GetHighestCoefficient() const
{
	return this->coefficients.size() - 1;
}

void Polynomial::Scale(const int scalar)
{
	for (auto i = 0; i <= this->GetHighestCoefficient(); i++)
	{
		this->SetCoefficient(this->GetCoefficient(i) * scalar, i);
	}
}

void Polynomial::AddRoot(const int root)
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

double Polynomial::ValueAt(const double x) const
{
	double res = 0;

	for (auto i = 0; i <= this->GetHighestCoefficient(); i++)
	{
		res += this->GetCoefficient(i) * std::pow(x, i);
	}

	return res;
}

Polynomial Polynomial::CalculateDerivative() const
{
	Polynomial p(*this);

	for (auto i = 0; i < p.GetHighestCoefficient(); i++)
	{
		p.SetCoefficient(p.GetCoefficient(i + 1) * (i + 1), i);
	}

	//Erase highest exponent
	p.coefficients.erase(p.coefficients.end() - 1);

	return p;
}

double Polynomial::CalculateIntegral(const int a, const int b) const
{
	auto p = this;
	auto IntegralPart = [p](const int n){
		if (p->integralData.count(n) > 0) //key exists
		{
			std::cout << "Retrieving: " << n << " from integral cache..." << std::endl;
			return p->integralData.find(n)->second;	
		}
		else
		{	
			std::lock_guard<std::mutex> lock(p->integralGuard);
			auto res = 0.;

			for (auto i = 0; i <= p->GetHighestCoefficient(); i++)
			{
				res += p->GetCoefficient(i) * std::pow(n, i + 1) / (i + 1);
			}

			p->integralData.insert({ n, res });
			return res;
		}
	};

	auto partB = std::async(IntegralPart, b);
	auto partA = std::async(IntegralPart, a);

	return partB.get() - partA.get();
	//return IntegralPart(b) - IntegralPart(a);
}

/*******
******** 	Operator overloads
********/

Polynomial& Polynomial::operator+=(const Polynomial& rhs)
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

Polynomial& Polynomial::operator *=(const Polynomial& rhs)
{
	std::lock_guard<std::mutex> lock(this->integralGuard);
	this->integralData.clear();
	std::vector<int> res(this->GetHighestCoefficient() + rhs.GetHighestCoefficient() + 1, 0);

	for (auto i = this->GetHighestCoefficient(); i >= 0; i--)
	{
		for (auto j = rhs.GetHighestCoefficient(); j >= 0; j--)
		{
			res[i + j] += this->GetCoefficient(i) * rhs.GetCoefficient(j);
		}
	}

	this->coefficients = res;

	return *this;
}

Polynomial& Polynomial::operator=(Polynomial&& p)
{
	this->coefficients = std::move(p.coefficients);
	this->integralData = std::move(p.integralData);

	return *this;
}

Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs)
{
	Polynomial p(lhs);

	p += rhs;

	return p;
}

Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs)
{
	Polynomial p(lhs);

	p *= rhs;

	return p;
}

std::ostream& operator<<(std::ostream& s, const Polynomial& p)
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