#pragma once
#include <common/exceptions/IllegalStateException.h>
#include <string>
#include <memory>

/**
 * Tests that two values are equal, within tolerance.
 * @param a First value
 * @param b Second value
 * @param tol (Optional) Tolerance. Defaults to 0.
 * @return True if the two values are equal within tolerance. Returns false otherwise.
 */
bool equals(float a, float b, float tol);

/**
 * Converts a value from radians to degrees
 * @param rad Angle in radians
 * @return Angle in degrees
 */
float rad2Deg(float rad);

/**
 * Converts a value from degrees to radians
 * @param deg Angle in degrees
 * @return Angle in radians
 */
float deg2Rad(float deg);

/**
 * Clamps a value to the given range
 * @param value Value to clamp
 * @param min Lower bound (inclusive)
 * @param max Upper bound (inclusive)
 * @return The clamped value, guaranteed to be within the given range
 */
float clamp(float value, float min, float max);

/**
 * Resolves a relative path to the directory containing this executable
 * @param relPath The relative path
 * @return The absolute path to the same file or directory
 * @throws IllegalStateException On failure to obtain the path to this executable
 */
std::string resolvePath(const std::string& relPath);

/**
 * Determines if a string ends with the given substring
 * @param str String value
 * @param suffix Suffix value to check for
 * @return True if the string value ends with the given suffix. Returns false otherwise.
 */
bool checkSuffix(const std::string& str, const std::string& suffix);

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
    if (ptr != nullptr)
    {
        return *ptr;
    }
    return ptr != nullptr ? *ptr : def;
}

/**
 * Attempts to cast a dynamic pointer to the given type, throwing an error on failure.
 * @param <S> The type to cast to
 * @param <T> The current object type
 * @param ptr The pointer to cast
 * @return The casted pointer
 * @throws IllegalStateException If the cast fails
 */
template <class S, class T>
std::shared_ptr<S> cast(const std::shared_ptr<T>& ptr)
{
    std::shared_ptr<S> casted = std::dynamic_pointer_cast<S>(ptr);
    if (!casted)
    {
        throw IllegalStateException("Failed to cast object to child subtype");
    }

    return casted;
}