#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputKey.h>
#include <common/PrintHelpers.h>
#include <GLFW/glfw3.h>

/**
 * Tests the ability to map input keys to their corresponding GLFW values and vise-versa
 */
BOOST_AUTO_TEST_CASE(InputKey_glfwMapping)
{
	// Test a few important starting positions within the enumeration

	BOOST_TEST(glfwToInputKey(GLFW_KEY_SPACE) == InputKey::KEY_SPACE);
	BOOST_TEST(glfwToInputKey(GLFW_KEY_0) == InputKey::KEY_0);
	BOOST_TEST(glfwToInputKey(GLFW_KEY_A) == InputKey::KEY_A);
	BOOST_TEST(glfwToInputKey(GLFW_KEY_RIGHT) == InputKey::KEY_RIGHT);
	BOOST_TEST(glfwToInputKey(GLFW_KEY_F1) == InputKey::KEY_F1);
	BOOST_TEST(glfwToInputKey(GLFW_KEY_KP_0) == InputKey::KEY_NUMPAD_0);
	BOOST_TEST(glfwToInputKey(GLFW_KEY_LEFT_SHIFT) == InputKey::KEY_LEFT_SHIFT);

	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_SPACE) == GLFW_KEY_SPACE);
	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_0) == GLFW_KEY_0);
	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_A) == GLFW_KEY_A);
	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_RIGHT) == GLFW_KEY_RIGHT);
	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_F1) == GLFW_KEY_F1);
	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_NUMPAD_0) == GLFW_KEY_KP_0);
	BOOST_TEST(inputKeyToGLFW(InputKey::KEY_LEFT_SHIFT) == GLFW_KEY_LEFT_SHIFT);
}