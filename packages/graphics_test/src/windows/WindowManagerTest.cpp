#include <boost/test/unit_test.hpp>
#include <graphics/windows/WindowManager.h>
#include <graphics/windows/Window.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 *  Tests that a window can be constructed, fetched, and destroyed via the manager
 */
BOOST_AUTO_TEST_CASE(WindowManager_basics)
{
	BOOST_REQUIRE_NO_THROW(WindowManager::create("test-window"));
	BOOST_TEST(WindowManager::get("test-window") != nullptr);
	BOOST_REQUIRE_NO_THROW(WindowManager::destroy("test-window"));
}

/**
 * Tests the ability to set the current window
 */
BOOST_AUTO_TEST_CASE(WindowManager_setCurrent)
{
	Window* oldWindow = WindowManager::getCurrent();

	Window* newWindow = WindowManager::create("test-window");
	WindowManager::setCurrent("test-window");
	BOOST_TEST(WindowManager::getCurrent() == newWindow);

	BOOST_REQUIRE_NO_THROW(WindowManager::destroy("test-window"));
	WindowManager::setCurrent(oldWindow->name);
}

/**
 * Tests that an exception is thrown if we try to create a window that already exists
 */
BOOST_AUTO_TEST_CASE(WindowManager_cannotCreateDuplicates)
{
	WindowManager::create("test-window");
	BOOST_REQUIRE_THROW(WindowManager::create("test-window"), IllegalArgumentException);

	WindowManager::destroy("test-window");
}

/**
 * Tests that an exception is thrown if we try to fetch a window that does not exist
 */
BOOST_AUTO_TEST_CASE(WindowManager_cannotFetchMissing)
{
	BOOST_REQUIRE_THROW(WindowManager::get("does-not-exist"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to destroy a window that does not exist
 */
BOOST_AUTO_TEST_CASE(WindowManager_cannotDestroyMissing)
{
	BOOST_REQUIRE_THROW(WindowManager::destroy("does-not-exist"), IllegalArgumentException);

	WindowManager::create("test-window");
	BOOST_REQUIRE_NO_THROW(WindowManager::destroy("test-window"));
	BOOST_REQUIRE_THROW(WindowManager::destroy("test-window"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown trying to set the current context to a window that does not exist
 */
BOOST_AUTO_TEST_CASE(WindowManager_cannotSetNullCurrent)
{
	BOOST_REQUIRE_THROW(WindowManager::setCurrent("does-not-exist"), IllegalArgumentException);
}