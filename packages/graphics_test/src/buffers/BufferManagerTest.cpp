#include <boost/test/unit_test.hpp>
#include <graphics/buffers/BufferManager.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests the ability to create and fetch a buffer
 */
BOOST_AUTO_TEST_CASE(BufferManager_createAndFetch)
{
	BufferManager::startBuffer("test-buffer").finish();
	BOOST_TEST(BufferManager::get("test-buffer") != nullptr);
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
}

/**
 * Tests that an exception is thrown if we try to fetch a buffer that does not exist
 */
BOOST_AUTO_TEST_CASE(BufferManager_fetchFailure)
{
	BOOST_REQUIRE_THROW(BufferManager::get("does-not-exist"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to destroy a buffer that does not exist
 */
BOOST_AUTO_TEST_CASE(BufferManager_destroyFailure)
{
	BOOST_REQUIRE_THROW(BufferManager::destroy("does-not-exist"), IllegalArgumentException);

	BufferManager::startBuffer("test-buffer").finish();
	BOOST_REQUIRE_NO_THROW(BufferManager::destroy("test-buffer"));
	BOOST_REQUIRE_THROW(BufferManager::destroy("test-buffer"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to create more than one buffer with the same name
 */
BOOST_AUTO_TEST_CASE(BufferManager_duplicateNames)
{
	BufferManager::startBuffer("test-buffer").finish();
	BOOST_REQUIRE_THROW(BufferManager::startBuffer("test-buffer").finish(), IllegalArgumentException);

	BufferManager::destroy("test-buffer");
}