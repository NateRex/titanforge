#include <boost/test/unit_test.hpp>
#include <graphics/buffers/BufferManager.h>
#include <graphics/buffers/Buffer.h>

/**
 * Tests construction and destruction of a buffer via the buffer manager
 */
BOOST_AUTO_TEST_CASE(Buffer_basics)
{
	BOOST_REQUIRE_NO_THROW(BufferManager::startBuffer("test-buffer").finish());
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
}