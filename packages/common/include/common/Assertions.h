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