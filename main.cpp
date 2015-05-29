#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE
#include <boost/test/unit_test.hpp>
#include "Polynomial.h"
#include <vector>
#include <stdexcept>
#include <limits>

BOOST_AUTO_TEST_CASE(SetCoefficient_GetCoefficient_GetHighestCoefficient)
{
	Polynomial p;
	
	int insert[] = {0, -10, 20};
	std::vector<int> list (insert, insert + sizeof(insert) / sizeof(int));

	//Insert
	for (unsigned int i = 0; i < list.size(); i++)
	{
		p.SetCoefficient(list[i], i);
	}

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1);

	//Check
	for (unsigned int i = 0; i < p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i]);
	}

	//Change 0
	int zeroValue = 5;
	p.SetCoefficient(zeroValue, 0);
	list[0] = zeroValue;
	BOOST_CHECK_EQUAL(p.GetCoefficient(0), zeroValue);
	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1);

	unsigned int coef = 4;
	unsigned int coefValue = 40;
	p.SetCoefficient(coefValue, coef);

	//Check again
	for (unsigned int i = 0; i < p.GetHighestCoefficient(); i++)
	{
		if (i < list.size())
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i]);
		}
		else if (i < coef)
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), 0);
		}
		else
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), coefValue);
		}
	}

	BOOST_CHECK_THROW(p.GetCoefficient(std::numeric_limits<unsigned int>::max()), std::out_of_range);

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	Polynomial p;

	BOOST_CHECK_EQUAL(p.GetHighestCoefficient(), 0);
	BOOST_CHECK_EQUAL(p.GetCoefficient(0), 0);
}

BOOST_AUTO_TEST_CASE(Constructor)
{
	unsigned int expo = 4;
	Polynomial p{expo};

	BOOST_REQUIRE(p.GetHighestCoefficient() == expo);

	//Check
	for (unsigned int i = 0; i < p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), 0);
	}
}