#include <common/Assertions.h>
#include <common/exceptions/IllegalArgumentException.h>

void assertGreaterThan(float value, float lowerBound, const char* errMsg) {
	if (value <= lowerBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertGreaterThanOrEqual(float value, float lowerBound, const char* errMsg) {
	if (value < lowerBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertLessThan(float value, float upperBound, const char* errMsg) {
	if (value >= upperBound)
	{
		throw IllegalArgumentException(errMsg);
	}
}

void assertLessThanOrEqual(float value, float upperBound, const char* errMsg) {
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