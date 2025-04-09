#pragma once
#include <common/exceptions/NullPointerException.h>
#include <common/exceptions/IllegalArgumentException.h>
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
 * Asserts that the given value is true
 * @param value Value to test
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not true
 */
void assertTrue(bool value, const char* errMsg = "Value was not true");

/**
 * Asserts that the given value is false
 * @param value Value to test
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not false
 */
void assertFalse(bool value, const char* errMsg = "Value was not false");

/**
 * Asserts that a value is equal to an expected value
 * @param value Value to test
 * @param expected Expected value
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if values are not equal
 */
void assertEquals(float value, float expected,
		const char* errMsg = "Value was not equal to expected constant");

/**
 * Asserts that a value is greater than some constant
 * @param value Value to test
 * @param lowerBound Lower bound
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not greater than the constant
 */
void assertGreaterThan(float value, float lowerBound,
		const char* errMsg = "Value was not greater than expected constant");

/**
 * Asserts that a value is greater than or equal to some constant
 * @param value Value to test
 * @param lowerBound Lower bound
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not greater than or equal to the constant
 */
void assertGreaterThanOrEqual(float value, float lowerBound,
		const char* errMsg = "Value was not greater than or equal to expected constant");

/**
 * Asserts that a value is less than some constant
 * @param value Value to test
 * @param upperBound Upper bound
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not less than the constant
 */
void assertLessThan(float value, float upperBound,
		const char* errMsg = "Value was not less than expected constant");

/**
 * Asserts that a value is less than or equal to some constant
 * @param value Value to test
 * @param upperBound Upper bound
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not less than or equal to the constant
 */
void assertLessThanOrEqual(float value, float upperBound,
		const char* errMsg = "Value was not less than or equal to than expected constant");

/**
 * Asserts that a value is within a given range
 * @param value Value to test
 * @param lowerBound Lower bound
 * @param upperBound Upper bound
 * @param inclusive True if the lower and upper bounds should be considered inclusive. False otherwise.
 * Defaults to true.
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if value is not within the expected range
 */
void assertInRange(float value, float lowerBound, float upperBound, bool inclusive = true,
		const char* errMsg = "Value was not in expected range");

/**
 * Asserts that a map contains a key
 * @param map The map to evaluate
 * @param key The key to search for
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if key is not present within the map
 */
template <typename MapType, typename KeyType>
void assertKeyInMap(const MapType& map, const KeyType& key, const char* errMsg = "Key was not found in map")
{
	if (map.find(key) == map.end())
	{
		throw IllegalArgumentException(errMsg);
	}
}

/**
 * Asserts that a map does not contain a key
 * @param map The map to evaluate
 * @param key The key to search for
 * @param errMsg (Optional) Custom error message
 * @throws IllegalArgumentException if key is present within the map
 */
template <typename MapType, typename KeyType>
void assertKeyNotInMap(const MapType& map, const KeyType& key, const char* errMsg = "Key already exists in map")
{
	if (map.find(key) != map.end())
	{
		throw IllegalArgumentException(errMsg);
	}
}