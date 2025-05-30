#include <boost/test/unit_test.hpp>
#include <graphics/core/input/TriggerType.h>
#include <common/PrintHelpers.h>
#include <GLFW/glfw3.h>

/**
 * Tests the mapping from GLFW action type to input trigger type
 */
BOOST_AUTO_TEST_CASE(TriggerType_mapping)
{
	BOOST_TEST(mapGLFWActionType(GLFW_PRESS) == TriggerType::PRESSED);
	BOOST_TEST(mapGLFWActionType(GLFW_RELEASE) == TriggerType::RELEASED);
	BOOST_TEST(mapGLFWActionType(GLFW_REPEAT) == TriggerType::HELD);
	BOOST_TEST(mapGLFWActionType(-1) == TriggerType::PRESSED);	// default case
}