#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>

Renderer* GlobalTestFixture::RENDERER = nullptr;

GlobalTestFixture::GlobalTestFixture()
{
	// Run in headless mode
	Window::setHeadlessMode(true);

	// Set up renderer
	if (!RENDERER)
	{
		RENDERER = new Renderer();
	}
}

GlobalTestFixture::~GlobalTestFixture()
{
	if (RENDERER)
	{
		delete RENDERER;
		RENDERER = nullptr;
	}
}