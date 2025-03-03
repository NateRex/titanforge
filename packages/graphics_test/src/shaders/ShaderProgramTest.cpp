#include <boost/test/unit_test.hpp>
#include <graphics/shaders/ShaderProgram.h>

/**
 * Test the basic accessors of the class
 */
BOOST_AUTO_TEST_CASE(ShaderProgram_basics)
{
	ShaderProgram prgm("ShaderProgramTest_Prgm1", { "shader" });
	BOOST_TEST(prgm.getName() == "ShaderProgramTest_Prgm1");
}