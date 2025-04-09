#include <common/Assertions.h>

void assertTrue(bool value, const char* errMsg)
{
	if (!value)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertFalse(bool value, const char* errMsg)
{
	if (value)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertEquals(float value, float expected, const char* errMsg)
{
	if (value != expected)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertGreaterThan(float value, float lowerBound, const char* errMsg)
{
	if (value <= lowerBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertGreaterThanOrEqual(float value, float lowerBound, const char* errMsg)
{
	if (value < lowerBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertLessThan(float value, float upperBound, const char* errMsg)
{
	if (value >= upperBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertLessThanOrEqual(float value, float upperBound, const char* errMsg)
{
	if (value > upperBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertInRange(float value, float lowerBound, float upperBound, bool inclusive, const char* errMsg)
{
	if (inclusive)
	{
		assertGreaterThanOrEqual(value, lowerBound, errMsg);
		assertLessThanOrEqual(value, upperBound, errMsg);
	}
	else
	{
		assertGreaterThan(value, lowerBound, errMsg);
		assertLessThan(value, upperBound, errMsg);
	}
}