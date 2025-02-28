#include <common/Assertions.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <sstream>

void assertGreaterThan(double value, double lowerBound) {
	if (value <= lowerBound)
	{
		std::ostringstream oss;
		oss << value << " is not greater than " << lowerBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertGreaterThanOrEqual(double value, double lowerBound) {
	if (value < lowerBound)
	{
		std::ostringstream oss;
		oss << value << " is not greater than or equal to " << lowerBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertLessThan(double value, double upperBound) {
	if (value >= upperBound)
	{
		std::ostringstream oss;
		oss << value << " is not less than " << upperBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertLessThanOrEqual(double value, double upperBound) {
	if (value > upperBound)
	{
		std::ostringstream oss;
		oss << value << " is not less than or equal to " << upperBound;
		throw IllegalArgumentException(oss.str());
	}
}

void assertInRange(double value, double lowerBound, double upperBound, bool inclusive)
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