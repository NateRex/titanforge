#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputTrigger.h>
#include <common/PrintHelpers.h>
#include <GLFW/glfw3.h>

/**
 * Tests the mapping from GLFW action type to input trigger type
 */
BOOST_AUTO_TEST_CASE(InputTrigger_mapping)
{
	BOOST_TEST(mapGLFWAction(GLFW_PRESS) == InputTrigger::PRESSED);
	BOOST_TEST(mapGLFWAction(GLFW_RELEASE) == InputTrigger::RELEASED);
	BOOST_TEST(mapGLFWAction(GLFW_REPEAT) == InputTrigger::HELD);
	BOOST_TEST(mapGLFWAction(-1) == InputTrigger::PRESSED);		// default case
}