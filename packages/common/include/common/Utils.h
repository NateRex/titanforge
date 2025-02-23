#pragma once
#include <iostream>
#include <type_traits>

/**
 * Tests that two values are equal, within tolerance.
 * @param a First value
 * @param b Second value
 * @param tol (Optional) Tolerance. Defaults to 0.
 * @return True if the two values are equal within tolerance. Returns false otherwise.
 */
bool equals(double a, double b, double tol);

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
 * Performs a null-check on the given pointer. If non-null, the pointer is deleted. Otherwise, this function
 * does nothing.
 * @param ptr Possibly null pointer
 * @return The nullptr
 */
template <typename T>
T* safeDelete(T* ptr)
{
	if (ptr != nullptr)
    {
        delete ptr;
    }
    return nullptr;
}

/**
 * Attempts to dereference the given pointer. If that pointer is null, this method returns a copy of the
 * default value. If the pointer is non-null, this method returns the dereferenced pointer.
 * @param <T> The value type
 * @param ptr Pointer to dereference
 * @param def Default value to return if the pointer is null
 * @return The dereferenced pointer, or the default value if the pointer was null
 */
template <typename T>
T& getOrDefault(T* ptr, T def)
{
    if (ptr != nullptr) {
        return *ptr;
    }
	return ptr != nullptr ? *ptr : def;
}

/**
 * Helper struct used to detect if a given type T has an overloaded << operator
 */
template <typename T, typename = void>
struct has_ostream_operator : std::false_type {};

/**
 * Helper struct used to detect if a given type T has an overloaded << operator
 */
template <typename T>
struct has_ostream_operator<
    T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>
> : std::true_type {};

/**
 * Overloaded << operator, enabled for classes only that do not already have an << operator
 * defined
 */
template <typename T>
std::enable_if_t<!has_ostream_operator<T>::value, std::ostream&>
operator<<(std::ostream& os, const T& obj) {
    return os << "[object]";
}