#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE
#include <boost/test/unit_test.hpp>
#include "Polynomial.h"
#include <vector>
#include <stdexcept>
#include <limits>

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	Polynomial p;

	BOOST_CHECK_EQUAL(p.GetHighestCoefficient(), 0);
	BOOST_CHECK_EQUAL(p.GetCoefficient(0), 0);
}

BOOST_AUTO_TEST_CASE(Constructor)
{
	const auto value = 40;
	const unsigned int expo = 4;
	Polynomial p{value, expo};

	BOOST_REQUIRE(p.GetHighestCoefficient() == expo);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		if (i == expo)
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), value);
		}
		else
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), 0);
		}
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Ordered)
{
	Polynomial p;

	auto list = std::vector<int> {3, -10, 20};

	//Insert
	for (unsigned int i = 0; i < list.size(); i++)
	{
		p.SetCoefficient(list[i], i);
	}

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Skipped)
{
	Polynomial p;

	unsigned int coef = 4;
	unsigned int coefValue = 40;
	p.SetCoefficient(coefValue, coef);

	BOOST_REQUIRE(p.GetHighestCoefficient() == coef);

	//Check again
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		if (i < p.GetHighestCoefficient())
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), 0);
		}
		else
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), coefValue);
		}
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Full_Range)
{
	Polynomial p;

	auto list = std::vector<int> {4, -11, 21};

	p.SetCoefficientRange<std::vector<int>>(list.cbegin(), list.cend());

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Partial_Range)
{
	Polynomial p;

	//Insert initial
	auto coef = 9;
	auto coefValue = 90;
	p.SetCoefficient(coefValue, coef);

	//Insert new
	auto list2 = std::vector<int>{11,12,13,14,15};
	auto offset = 1;
	p.SetCoefficientRange<std::vector<int>>(list2.cbegin(), list2.cend(), offset);

	BOOST_REQUIRE(p.GetHighestCoefficient() == coef);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		if (i < offset || i >= list2.size() + offset)
		{
			if (i == coef)
			{
				BOOST_CHECK_EQUAL(p.GetCoefficient(i), coefValue);
			}
			else
			{
				BOOST_CHECK_EQUAL(p.GetCoefficient(i), 0);
			}
		}
		else
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), list2[i-offset]);
		}
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Extended_Range)
{
	Polynomial p;

	//Insert initial
	auto coef = 3;
	auto coefValue = 0;
	p.SetCoefficient(coefValue, coef);

	//Extend
	auto offset = 3;
	auto list = std::vector<int> {4, -11, 21};
	p.SetCoefficientRange<std::vector<int>>(list.cbegin(), list.cend(), offset);

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1 + offset);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		if (i < offset)
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), 0);
		}
		else
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i - offset]);
		}
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Change)
{
	Polynomial p;

	unsigned int coef = 4;
	unsigned int coefValue = 40;
	p.SetCoefficient(coefValue, coef);

	BOOST_REQUIRE(p.GetHighestCoefficient() == coef);

	//Change 1
	int oneValue = 5;
	p.SetCoefficient(oneValue, 1);
	BOOST_CHECK_EQUAL(p.GetCoefficient(1), oneValue);
	BOOST_CHECK_EQUAL(p.GetHighestCoefficient(), coef);

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(GetCoefficient_Out_Of_Bounds)
{
	Polynomial p;

	BOOST_CHECK_THROW(p.GetCoefficient(std::numeric_limits<unsigned int>::max()), std::out_of_range);

	std::cout << p << std::endl;
}

void ScaleTest(const int scalar)
{
	Polynomial p;

	auto list = std::vector<int> {3, -10, 0, 20};

	//Insert
	for (unsigned int i = 0; i < list.size(); i++)
	{
		p.SetCoefficient(list[i], i);
	}

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1);

	//Scale
	p.Scale(scalar);

	//Check
	for (unsigned int i = 0; i < list.size(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i] * scalar);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Scale_Positive)
{
	ScaleTest(2);
}

BOOST_AUTO_TEST_CASE(Scale_Negative)
{
	ScaleTest(-2);
}

