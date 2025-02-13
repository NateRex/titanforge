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
 * Performs a null-check on the given pointer. If non-null, the pointer is deleted. Otherwise, this function
 * does nothing.
 * @param ptr Possibly null pointer
 */
void safeDelete(void* ptr);

/**
 * Converts a value from radians to degrees
 * @param rad Angle in radians
 * @return Angle in degrees
 */
double rad2Deg(double rad);

/**
 * Converts a value from degrees to radians
 * @param deg Angle in degrees
 * @return Angle in radians
 */
double deg2Rad(double deg);

/**
 * Attempts to dereference the given pointer. If that pointer is null, this method returns the default value
 * provided instead.
 * @param <T> The value type
 * @param ptr Pointer to dereference
 * @param def Default value to return if the pointer is null
 * @return The dereferenced pointer, or the default value if the pointer was null
 */
template <typename T>
T& getOrDefault(T* ptr, T& def)
{
	return ptr != nullptr ? *ptr : def;
}