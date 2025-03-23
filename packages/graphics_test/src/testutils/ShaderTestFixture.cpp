#include <graphics_test/testutils/ShaderTestFixture.h>
#include <graphics/shaders/ShaderManager.h>

ShaderTestFixture::ShaderTestFixture()
{
	ShaderManager::create("test", Shaders::TEST_VERTEX, Shaders::TEST_FRAGMENT);
}

ShaderTestFixture::~ShaderTestFixture()
{
	ShaderManager::destroy("test");
}