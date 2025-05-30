#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputCode.h>
#include <GLFW/glfw3.h>

/**
 * Tests the ability to map input codes to their corresponding GLFW values
 */
BOOST_AUTO_TEST_CASE(InputCode_mapping)
{
	// Test a few important starting positions within the enumeration
	BOOST_TEST(mapInputCode(InputCode::KEY_SPACE) == GLFW_KEY_SPACE);
	BOOST_TEST(mapInputCode(InputCode::KEY_0) == GLFW_KEY_0);
	BOOST_TEST(mapInputCode(InputCode::KEY_A) == GLFW_KEY_A);
	BOOST_TEST(mapInputCode(InputCode::KEY_RIGHT) == GLFW_KEY_RIGHT);
	BOOST_TEST(mapInputCode(InputCode::KEY_F1) == GLFW_KEY_F1);
	BOOST_TEST(mapInputCode(InputCode::KEY_NUMPAD_0) == GLFW_KEY_KP_0);
	BOOST_TEST(mapInputCode(InputCode::KEY_LEFT_SHIFT) == GLFW_KEY_LEFT_SHIFT);
}