#pragma once
#include <iostream>
#include <type_traits>
#include <utility>

/**
 * Helper struct used to detect if a given type T has an overloaded << operator
 * @param <T> The object type
 */
template <typename T, typename = void>
struct has_ostream_operator : std::false_type {};

/**
 * Helper struct used to detect if a given type T has an overloaded << operator
 * @param <T> The object type
 */
template <typename T>
struct has_ostream_operator<
    T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>
> : std::true_type {
};

/**
 * Overloaded << operator, enabled for classes only that do not already have an << operator
 * defined
 * @param <T> The object type
 * @param os The output stream
 * @param obj The object to print
 */
template <typename T>
std::enable_if_t<!has_ostream_operator<T>::value, std::ostream&>
operator<<(std::ostream& os, const T& obj) {
    return os << "[object]";
}