#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/materials/MaterialType.h>

/**
 * Tests that shaders can be obtained for each type of material type
 */
BOOST_AUTO_TEST_CASE(ShaderManager_getShader)
{
	ShaderManager& mgr = ShaderManager::getInstance();
	BOOST_TEST(mgr.getShader(MaterialType::BASIC) != nullptr);
}
