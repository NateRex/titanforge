#pragma once
#include <common/exceptions/NullPointerException.h>
#include <functional>

/**
 * Asserts that the given pointer is not null.
 * @param <T> The object type
 * @param obj The object to test
 * @param errMsg Error message to display in the case where the object is null
 * @param callback (Optional) Callback function that should be invoked prior to throwing an exception in
 * the case where the object is null
 * @throws NullPointerException if the pointer is null
 */
template <typename T>
void assertNotNull(const T* obj, const char* errMsg, const std::function<void()> func = []() {})
{
	if (obj == nullptr)
	{
		func();
		throw NullPointerException(errMsg);
	}
}

/**
 * Asserts that a value is greater than some constant
 * @param value Value to test
 * @param lowerBound Lower bound
 * @throws IllegalArgumentException if value is not greater than the constant
 */
void assertGreaterThan(float value, float lowerBound);

/**
 * Asserts that a value is greater than or equal to some constant
 * @param value Value to test
 * @param lowerBound Lower bound
 * @throws IllegalArgumentException if value is not greater than or equal to the constant
 */
void assertGreaterThanOrEqual(float value, float lowerBound);

/**
 * Asserts that a value is less than some constant
 * @param value Value to test
 * @param upperBound Upper bound
 * @throws IllegalArgumentException if value is not less than the constant
 */
void assertLessThan(float value, float upperBound);

/**
 * Asserts that a value is less than or equal to some constant
 * @param value Value to test
 * @param upperBound Upper bound
 * @throws IllegalArgumentException if value is not less than or equal to the constant
 */
void assertLessThanOrEqual(float value, float upperBound);

/**
 * Asserts that a value is within a given range
 * @param value Value to test
 * @param lowerBound Lower bound
 * @param upperBound Upper bound
 * @param inclusive True if the lower and upper bounds should be considered inclusive. False otherwise.
 * Defaults to true.
 * @throws IllegalArgumentException if value is not within the expected range
 */
void assertInRange(float value, float lowerBound, float upperBound, bool inclusive = true);