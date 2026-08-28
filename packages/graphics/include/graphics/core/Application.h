#pragma once
#include <graphics/core/pointers/RendererPtr.h>
#include <functional>

class InputController;

/**
 * Timing and indexing information for a single application frame
 * @author Nathaniel Rex
 */
struct Frame
{
    /**
     * Time in seconds since the previous frame
     */
    float deltaTime = 0.f;

    /**
     * Seconds elapsed since the application was started
     */
    double elapsedTime = 0.;

    /**
     * Zero-based frame index
     */
    unsigned long long frameNumber = 0;
};

/**
 * An application encompasses the main logic loop and monitors timing between frames
 * @author Nathaniel Rexs
 */
class Application
{
public:

    using RunCallback = std::function<void(const Frame&)>;

    /**
     * Constructs an application around a renderer
     * @param renderer The renderer used by the application
     */
    Application(RendererPtr renderer);

    /**
     * Runs the application until the rendering window closes or Application::stop is explicitly called.
     * @param callback Callback function responsible for rendering the frame. Accepts a frame context as
     * input.
     */
    void run(const RunCallback& callback);
    
    /**
     * Requests that the active run loop stop after the current callback
     */
    void stop() { _running = false; }

    /**
     * @return True if the application is currently running. Returns false otherwise.
     */
    bool isRunning() const { return _running; }

    /**
     * Returns the application's renderer
     */
    RendererPtr getRenderer() const { return _renderer; }

    /**
     * @return The input controller, owned by the window context of the current renderer
     */
    InputController* getInputController();

    /**
     * Sets the maximum delta time allowed to be passed to callbacks in between frames. This prevents large
     * simulation jumps after events such as debugger pauses or the application window being dragged.
     * @param seconds Max delta time in seconds
     */
    void setMaxDeltaTime(float seconds);

private:

    /**
     * Renderer
     */
    RendererPtr _renderer;

    /**
     * Boolean flag that, when true, implies the application is currently running
     */
    bool _running = false;

    /**
     * Maximum delta time to report between frames, in seconds
     */
    float _maxDeltaTime = 0.1f;
};