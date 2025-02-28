#include <common/Assertions.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <sstream>

void assertGreaterThan(float value, float lowerBound) {
	if (value <= lowerBound)
	{
		std::ostringstream oss;
		oss << value << " is not greater than " << lowerBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertGreaterThanOrEqual(float value, float lowerBound) {
	if (value < lowerBound)
	{
		std::ostringstream oss;
		oss << value << " is not greater than or equal to " << lowerBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertLessThan(float value, float upperBound) {
	if (value >= upperBound)
	{
		std::ostringstream oss;
		oss << value << " is not less than " << upperBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertLessThanOrEqual(float value, float upperBound) {
	if (value > upperBound)
	{
		std::ostringstream oss;
		oss << value << " is not less than or equal to " << upperBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertInRange(float value, float lowerBound, float upperBound, bool inclusive)
{
	if (inclusive)
	{
		assertGreaterThanOrEqual(value, lowerBound);
		assertLessThanOrEqual(value, upperBound);
	}
	else
	{
		assertGreaterThan(value, lowerBound);
		assertLessThan(value, upperBound);
	}
}