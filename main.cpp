/*
	Name: 			Michael Lausdahl Fuglsang
	email:			mfugls11
	Study No.:		20112699

	Code also available at:
	https://github.com/Malakahh/Ap-exam-part-2
*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE
#include <boost/test/unit_test.hpp>
#include "Polynomial.h"
#include <vector>
#include <stdexcept>
#include <limits>

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	Polynomial<double> p;

	BOOST_CHECK_EQUAL(p.GetHighestCoefficient(), 0);
	BOOST_CHECK_EQUAL(p.GetCoefficient(0), 0);
}

BOOST_AUTO_TEST_CASE(Constructor)
{
	BOOST_TEST_MESSAGE("Constructor\n");

	const auto value = 40;
	const unsigned int expo = 4;
	Polynomial<double> p(value, expo);

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

BOOST_AUTO_TEST_CASE(ConstructorBraced)
{
	Polynomial<double> p{10, 20, -30, -40, 20};
	auto expectedResult = std::vector<double>{10, 20, -30, -40, 20};

	//Check
	for (auto i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
	const auto value = 40;
	const unsigned int expo = 4;
	Polynomial<double> p(value, expo);

	Polynomial<double> p2(p);
	p2.SetCoefficient(77, 0);

	for (auto i = 0; i <= p2.GetHighestCoefficient(); i++)
	{
		if (i == 0)
		{
			BOOST_CHECK_NE(p.GetCoefficient(i), p2.GetCoefficient(i));
		}
		else
		{
			BOOST_CHECK_EQUAL(p.GetCoefficient(i), p2.GetCoefficient(i));
		}
	}

	std::cout << p2 << std::endl;
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
	Polynomial<double> p;
	Polynomial<double> q(std::move(p));

	BOOST_REQUIRE_THROW(p.GetCoefficient(0), std::out_of_range);
	BOOST_CHECK_EQUAL(q.GetCoefficient(0), 0);
}

BOOST_AUTO_TEST_CASE(Insert_Ordered)
{
	Polynomial<double> p;

	auto list = std::vector<double> {3, -10, 20};

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
	Polynomial<double> p;

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
	Polynomial<double> p;

	auto list = std::vector<double> {4, -11, 21};

	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), list[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Full_Range_Array)
{
	Polynomial<double> p;

	auto arr = std::array<double, 3> {4, -11, 21};

	p.SetCoefficientRange<std::array<double, 5>>(arr.cbegin(), arr.cend());

	BOOST_REQUIRE(p.GetHighestCoefficient() == arr.size() - 1);

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), arr[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Insert_Partial_Range)
{
	Polynomial<double> p;

	//Insert initial
	auto coef = 9;
	auto coefValue = 90;
	p.SetCoefficient(coefValue, coef);

	//Insert new
	auto list2 = std::vector<double>{11,12,13,14,15};
	auto offset = 1;
	p.SetCoefficientRange<std::vector<double>>(list2.cbegin(), list2.cend(), offset);

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

BOOST_AUTO_TEST_CASE(Insert_Partial_Range_int)
{
	Polynomial<int> p;

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

BOOST_AUTO_TEST_CASE(Insert_Partial_Range_Long_Double)
{
	Polynomial<long double> p;

	//Insert initial
	auto coef = 9;
	auto coefValue = 90;
	p.SetCoefficient(coefValue, coef);

	//Insert new
	auto list2 = std::vector<long double>{11,12,13,14,15};
	auto offset = 1;
	p.SetCoefficientRange<std::vector<long double>>(list2.cbegin(), list2.cend(), offset);

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
	Polynomial<double> p;

	//Insert initial
	auto coef = 3;
	auto coefValue = 0;
	p.SetCoefficient(coefValue, coef);

	//Extend
	auto offset = 3;
	auto list = std::vector<double> {4, -11, 21};
	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend(), offset);

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
	Polynomial<double> p;

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
	Polynomial<double> p;

	BOOST_CHECK_THROW(p.GetCoefficient(std::numeric_limits<unsigned int>::max()), std::out_of_range);

	std::cout << p << std::endl;
}

void ScaleTest(const int scalar)
{
	Polynomial<double> p;

	auto list = std::vector<double> {3, -10, 0, 20};

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

BOOST_AUTO_TEST_CASE(Add_Root)
{
	Polynomial<double> p;
	auto root = -2;
	auto list = std::vector<double> {5, -1, 4, 2};
	auto expectedResult = std::vector<double>{10, 3, 7, 8, 2};

	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());
	p.AddRoot(root);

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size()); //list.size() is equal to oldP.GetHighestCoefficient() + 1

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Add_Root_Range)
{
	Polynomial<double> p;
	auto roots = std::vector<double>{-2, -1, 0, 1, 4};
	auto list = std::vector<double> {5, -1, 4, 2};
	auto expectedResult = std::vector<double>{0, 40, 2, -15, 23, -25, -27, 0, 2};

	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	p.AddRootRange<std::vector<double>>(roots.cbegin(), roots.cend());

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1 + roots.size());

	//Check
	for (unsigned int i = 0; i <= p.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(p.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Valuate)
{
	Polynomial<double> p;
	auto roots = std::vector<double>{-2, -1, 0, 1, 4};
	auto list = std::vector<double> {5, -1, 4, 2};

	auto x = std::vector<float> {-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};
	auto values = std::vector<float> {-53.32, 45.11, -15.82, 18.86, -315.82, -5204.88}; //Values calculated using Graph, some precision has been lost, https://www.padowan.dk/

	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	p.AddRootRange<std::vector<double>>(roots.cbegin(), roots.cend());

	BOOST_REQUIRE(p.GetHighestCoefficient() == list.size() - 1 + roots.size());

	for (auto i = 0; i < x.size(); i++)
	{
		std::cout << x[i] << " - CalcVal: " << p.ValueAt(x[i]) << " Actual: " << values[i] << std::endl;

		//Using two-decimal precision
		BOOST_CHECK(trunc(10. * p.ValueAt(x[i])) == trunc(10. * values[i]));
	}
}

BOOST_AUTO_TEST_CASE(Derivative)
{
	Polynomial<double> p;
	auto list = std::vector<double> {5, -1, 4, 2};
	auto expectedResult = std::vector<double> {-1, 8, 6};

	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	auto pMark = p.CalculateDerivative();

	for (auto i = 0; i <= pMark.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(pMark.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << pMark << std::endl;
}

BOOST_AUTO_TEST_CASE(Integral)
{
	Polynomial<double> p{5, -1, 4, 2};

	//Calculating integral twice, to verify cache is in use
	for (auto i = 0; i < 2; i++)
	{
		auto a = 3.;
		auto b = 5.;
		auto expectedResult = 1214. / 3.;

		auto area = p.CalculateIntegral(a, b);

		//Using two-decimal precision
		BOOST_CHECK(trunc(10. * area) == trunc(10. * expectedResult));

		std::cout << "Area: " << area << " Expected: " << expectedResult << std::endl;
	}
}

/*
//This breaks the compilation through static assert, as it should
BOOST_AUTO_TEST_CASE(Integral_Int)
{
	Polynomial<int> p{5, -1, 4, 2};

	//Calculating integral twice, to verify cache is in use
	for (auto i = 0; i < 2; i++)
	{
		auto a = 3.;
		auto b = 5.;
		auto expectedResult = 1214. / 3.;

		auto area = p.CalculateIntegral(a, b);

		//Using two-decimal precision
		BOOST_CHECK(trunc(10. * area) == trunc(10. * expectedResult));

		std::cout << "Area: " << area << " Expected: " << expectedResult << std::endl;
	}
}
*/

BOOST_AUTO_TEST_CASE(Plus_Equal_Operator)
{
	Polynomial<double> p;
	auto list = std::vector<double> {5, -1, 4, 2};
	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	Polynomial<double> p2;
	auto list2 = std::vector<double> {-3, 6, -2, 5, 3};
	p2.SetCoefficientRange<std::vector<double>>(list2.cbegin(), list2.cend());

	auto expectedResult = std::vector<double> {2, 5, 2, 7, 3};

	p += p2;

	for (auto i = 0; i <= p2.GetHighestCoefficient(); i++)
	{
		BOOST_REQUIRE_EQUAL(p.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Plus_Operator)
{
	Polynomial<double> p;
	auto list = std::vector<double> {5, -1, 4, 2};
	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	Polynomial<double> p2;
	auto list2 = std::vector<double> {-3, 6, -2, 5, 3};
	p2.SetCoefficientRange<std::vector<double>>(list2.cbegin(), list2.cend());

	auto expectedResult = std::vector<double> {2, 5, 2, 7, 3};

	auto res = p + p2;

	for (auto i = 0; i <= p2.GetHighestCoefficient(); i++)
	{
		BOOST_CHECK_EQUAL(res.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << res << std::endl;
}

BOOST_AUTO_TEST_CASE(Star_Operator)
{
	Polynomial<double> p;
	auto list = std::vector<double> {8, 7, 6, 3};
	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	Polynomial<double> p2;
	auto list2 = std::vector<double> {-2, 0, 10};
	p2.SetCoefficientRange<std::vector<double>>(list2.cbegin(), list2.cend());

	auto expectedResult = std::vector<double> { -16, -14, 68, 64, 60, 30};

	p *= p2;

	for (auto i = 0; i < expectedResult.size(); i++)
	{
		BOOST_REQUIRE_EQUAL(p.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << p << std::endl;
}

BOOST_AUTO_TEST_CASE(Star__Equal_Operator)
{
	Polynomial<double> p;
	auto list = std::vector<double> {8, 7, 6, 3};
	p.SetCoefficientRange<std::vector<double>>(list.cbegin(), list.cend());

	Polynomial<double> p2;
	auto list2 = std::vector<double> {-2, 0, 10};
	p2.SetCoefficientRange<std::vector<double>>(list2.cbegin(), list2.cend());

	auto expectedResult = std::vector<double> { -16, -14, 68, 64, 60, 30};

	auto res = p * p2;

	for (auto i = 0; i < expectedResult.size(); i++)
	{
		BOOST_REQUIRE_EQUAL(res.GetCoefficient(i), expectedResult[i]);
	}

	std::cout << res << std::endl;
}