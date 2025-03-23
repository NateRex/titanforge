#include <graphics_test/testutils/GlobalTestFixture.h>
#include <graphics/Engine.h>

GlobalTestFixture::GlobalTestFixture()
{
	Engine::start(true);
}

GlobalTestFixture::~GlobalTestFixture()
{
	Engine::stop();
}