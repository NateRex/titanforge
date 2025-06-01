#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputTrigger.h>
#include <common/PrintHelpers.h>
#include <GLFW/glfw3.h>

/**
 * Tests the mapping from GLFW action type to input trigger type
 */
BOOST_AUTO_TEST_CASE(InputTrigger_mapping)
{
	BOOST_TEST(mapGLFWActionType(GLFW_PRESS) == InputTrigger::PRESSED);
	BOOST_TEST(mapGLFWActionType(GLFW_RELEASE) == InputTrigger::RELEASED);
	BOOST_TEST(mapGLFWActionType(GLFW_REPEAT) == InputTrigger::HELD);
	BOOST_TEST(mapGLFWActionType(-1) == InputTrigger::PRESSED);	// default case
}