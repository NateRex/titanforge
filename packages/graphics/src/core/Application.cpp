#include <graphics/core/Application.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/IllegalStateException.h>
#include <algorithm>

Application::Application(RendererPtr renderer): _renderer(renderer)
{
    if (!renderer)
    {
        throw IllegalArgumentException("Application requires a renderer");
    }
}

void Application::run(const RunCallback& callback)
{
    if (_running)
    {
        throw IllegalStateException("Application is already running");
    }
    if (!callback)
    {
        throw IllegalArgumentException("Application requires a callback function");
    }

    _running = true;

    const double startTime = _renderer->getTime();
    double previousFrameTime = startTime;
    unsigned long long frameNumber = 0;

    while (_running && _renderer->getWindow()->isOpen())
    {
        const double currentTime = _renderer->getTime();
        const double rawDeltaTime = std::max(currentTime - previousFrameTime, 0.);
        previousFrameTime = currentTime;

        Frame frame;
        frame.deltaTime = static_cast<float>(std::min(rawDeltaTime, static_cast<double>(_maxDeltaTime)));
        frame.elapsedTime = currentTime - startTime;
        frame.frameNumber = frameNumber++;

        _renderer->getWindow()->getInputController()->pollForKeyHolds(frame.deltaTime);
        callback(frame);
    }

    _running = false;
}

void Application::setMaxDeltaTime(float seconds)
{
    if (seconds <= 0.f)
    {
        throw IllegalArgumentException("Maximum delta time must be greater than zero");
    }
    _maxDeltaTime = seconds;
}