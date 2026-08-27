#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>

/**
 * Tests that shaders can be obtained for each type of material type
 */
BOOST_AUTO_TEST_CASE(ShaderManager_getShader)
{
	BOOST_TEST(ShaderManager::getShader(ShaderId::POINT) != nullptr);
	BOOST_TEST(ShaderManager::getShader(ShaderId::LINE) != nullptr);
	BOOST_TEST(ShaderManager::getShader(ShaderId::MESH) != nullptr);
	BOOST_TEST(ShaderManager::getShader(ShaderId::WIREFRAME) != nullptr);
	BOOST_TEST(ShaderManager::getShader(ShaderId::SKYBOX) != nullptr);
	BOOST_TEST(ShaderManager::getShader(ShaderId::POST_PROCESS) != nullptr);
	BOOST_TEST(ShaderManager::getShader(ShaderId::NORMALS) != nullptr);
}
