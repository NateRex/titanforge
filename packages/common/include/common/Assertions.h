#pragma once
#include <common/exceptions/NullPointerException.h>
#include <functional>

/**
 * Asserts that the given pointer is not null.
 * @param <T> The object type
 * @param obj The object to test
 * @param callback (Optional) Callback function that should be invoked prior to throwing an exception in
 * the case that the object is null
 * @throws NullPointerException if the pointer is null
 */
template <typename T>
void assertNotNull(const T* obj, const std::function<void()> func = []() {})
{
	if (obj == nullptr)
	{
		func();
		throw NullPointerException("Unexpected null pointer");
	}
}