#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Application.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/windows/Window.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/IllegalStateException.h>
#include <chrono>
#include <thread>

/**
 * Tests construction and access to the objects owned by an application.
 */
BOOST_AUTO_TEST_CASE(Application_basics)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	Application app(renderer);

	BOOST_TEST(!app.isRunning());
	BOOST_TEST(app.getRenderer() == renderer);
	BOOST_TEST(app.getInputController() == renderer->getWindow()->getInputController());
}

/**
 * Tests that an application cannot be constructed without a renderer.
 */
BOOST_AUTO_TEST_CASE(Application_requiresRenderer)
{
	BOOST_CHECK_THROW(Application(nullptr), IllegalArgumentException);
}

/**
 * Tests frame metadata and stopping an active run loop.
 */
BOOST_AUTO_TEST_CASE(Application_runAndStop)
{
	Application app(GlobalTestFixture::RENDERER);
	unsigned int callbackCount = 0;

	app.run([&](const Frame& frame)
	{
		BOOST_TEST(app.isRunning());
		BOOST_TEST(frame.frameNumber == 0u);
		BOOST_TEST(frame.deltaTime >= 0.f);
		BOOST_TEST(frame.elapsedTime >= 0.0);
		callbackCount++;
		app.stop();
	});

	BOOST_TEST(callbackCount == 1u);
	BOOST_TEST(!app.isRunning());
}

/**
 * Tests that frame numbers advance and elapsed time never moves backwards.
 */
BOOST_AUTO_TEST_CASE(Application_frameProgression)
{
	Application app(GlobalTestFixture::RENDERER);
	double previousElapsedTime = 0.0;
	unsigned long long expectedFrameNumber = 0;

	app.run([&](const Frame& frame)
	{
		BOOST_TEST(frame.frameNumber == expectedFrameNumber);
		BOOST_TEST(frame.elapsedTime >= previousElapsedTime);
		BOOST_TEST(frame.deltaTime >= 0.f);

		previousElapsedTime = frame.elapsedTime;
		expectedFrameNumber++;
		if (expectedFrameNumber == 3)
		{
			app.stop();
		}
	});

	BOOST_TEST(expectedFrameNumber == 3u);
}

/**
 * Tests validation of run callbacks and nested run loops.
 */
BOOST_AUTO_TEST_CASE(Application_runValidation)
{
	Application app(GlobalTestFixture::RENDERER);

	BOOST_CHECK_THROW(app.run(Application::RunCallback()), IllegalArgumentException);

	app.run([&](const Frame&)
	{
		BOOST_CHECK_THROW(app.run([](const Frame&) {}), IllegalStateException);
		app.stop();
	});
}

/**
 * Tests validation and enforcement of the maximum reported delta time.
 */
BOOST_AUTO_TEST_CASE(Application_maxDeltaTime)
{
	Application app(GlobalTestFixture::RENDERER);
	BOOST_CHECK_THROW(app.setMaxDeltaTime(0.f), IllegalArgumentException);
	BOOST_CHECK_THROW(app.setMaxDeltaTime(-1.f), IllegalArgumentException);

	constexpr float maxDeltaTime = 0.001f;
	app.setMaxDeltaTime(maxDeltaTime);

	app.run([&](const Frame& frame)
	{
		BOOST_TEST(frame.deltaTime <= maxDeltaTime);
		if (frame.frameNumber == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			// The sleep guarantees that the raw delta exceeds the configured cap.
			BOOST_TEST(frame.deltaTime == maxDeltaTime);
			app.stop();
		}
	});
}
