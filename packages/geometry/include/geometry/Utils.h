#pragma once

/**
 * Tests that two values are equal, within tolerance.
 * @param a First value
 * @param b Second value
 * @param tol (Optional) Tolerance. Defaults to 0.
 * @return True if the two values are equal within tolerance. Returns false otherwise.
 */
bool equals(double a, double b, double tol);

/**
 * Attempts to dereference the given pointer. If that pointer is null, this method returns the default value
 * provided instead.
 * @param <T> The value type
 * @param ptr Pointer to dereference
 * @param def Default value to return if the pointer is null
 * @return The dereferenced pointer, or the default value if the pointer was null
 */
#pragma warning(disable : 4172)
template <typename T>
T getOrDefault(const T* ptr, const T& def)
{
	return ptr != nullptr ? *ptr : def;
}
#pragma warning(default : 4172)