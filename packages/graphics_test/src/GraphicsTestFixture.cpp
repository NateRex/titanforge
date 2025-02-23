#include <graphics_test/GraphicsTestFixture.h>
#include <graphics/Engine.h>

GraphicsTestFixture::GraphicsTestFixture()
{
	setup();
}

GraphicsTestFixture::~GraphicsTestFixture()
{
	teardown();
}

void GraphicsTestFixture::setup()
{
	Engine::start();
}

void GraphicsTestFixture::teardown()
{
	Engine::stop();
}