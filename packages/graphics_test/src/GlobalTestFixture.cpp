#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>

RendererPtr GlobalTestFixture::RENDERER = nullptr;

GlobalTestFixture::GlobalTestFixture()
{
	// Run in headless mode
	Window::setHeadlessMode(true);

	// Set up renderer
	if (!RENDERER)
	{
		RENDERER = Renderer::create();
	}
}

GlobalTestFixture::~GlobalTestFixture()
{
	RENDERER = nullptr;
}