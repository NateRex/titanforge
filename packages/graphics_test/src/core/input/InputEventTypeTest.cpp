#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputEventType.h>
#include <common/PrintHelpers.h>
#include <GLFW/glfw3.h>

/**
 * Tests the mapping from GLFW action type to input event type
 */
BOOST_AUTO_TEST_CASE(InputEventType_mapping)
{
	BOOST_TEST(mapGLFWActionType(GLFW_PRESS) == InputEventType::PRESSED);
	BOOST_TEST(mapGLFWActionType(GLFW_RELEASE) == InputEventType::RELEASED);
	BOOST_TEST(mapGLFWActionType(GLFW_REPEAT) == InputEventType::HELD);
	BOOST_TEST(mapGLFWActionType(-1) == InputEventType::PRESSED);	// default case
}