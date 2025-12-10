#pragma once
#include <graphics/core/windows/pointers/WindowPtr.h>
#include <mutex>

struct GLFWwindow;
class InputController;

/**
 * A window target for rendering
 * @author Nathaniel Rex
 */
class Window
{
public:

    friend class Renderer;

    /**
     * Destructor
     */
    ~Window();

    /**
     * Sets the global flag that determines whether or not windows should be initialized in "headless mode",
     * which prevents the engine from trying to actually display them on screen. This is primarily useful in
     * test environments.
     * @param headlessMode True if windows should be initialized in "headless mode". False otherwise.
     */
    static void setHeadlessMode(bool headlessMode);

    /**
     * Constructs a new window
     * @param title Window title
     * @param width Starting width (in pixels).
     * @param height Starting height (in pixels).
     * @return The new window instance
     */
    static WindowPtr create(const char* title, unsigned int width, unsigned int height);

    /**
     * @return The input controller
     */
    InputController* getInputController();

    /**
     * @return True if this window is open. Returns false if the window has been closed, and
     * therefore destroyed.
     */
    bool isOpen() const;

    /**
     * Destroys this window, releasing all of its resources
     */
    void destroy();

private:

    /**
     * Boolean flag that, when true, indicates that GLFW should be initialized in "headless mode",
     * which prevents the display of windows. This is primarily useful in test environments.
     */
    static bool _HEADLESS;

    /**
     * The number of windows currently in existence. This counter is used to determine when we need
     * to init and deinit GLFW.
     */
    static int _WINDOW_COUNT;

    /**
     * Mutex for modifying the global window count
     */
    static std::mutex _MUTEX;

    /**
     * A pointer to the GLFW window object
     */
    GLFWwindow* _glfwWindow;

    /**
     * Input controller
     */
    InputController* _inputController;

    /**
     * Constructor.
     * @param title Window title
     * @param width Starting width (in pixels).
     * @param height Starting height (in pixels).
     */
    Window(const char* title, unsigned int width, unsigned int height);

    /**
     * Increments the global window count. If this is the first window being created, this will result
     * in the initialization of GLFW.
     */
    static void incrementWindowCount();

    /**
     * Decrements the global window count. If this is the last window to be destroyed, this will result
     * in the termination of GLFW.
     */
    static void decrementWindowCount();

    /**
     * Initializes GLFW. This should only be done on creation of the first window.
     * @return True if initialization was successful. Returns false otherwise.
     */
    static bool initGLFW();

    /**
     * Terminates GLFW. This should only be done once the last window has been destroyed.
     */
    static void terminateGLFW();

    /**
     * Callback method that is triggered whenever a window is resized
     * @param window The GLFW window pointer
     * @param width New window width (in pixels)
     * @param height New window height (in pixels)
     */
    static void onResize(GLFWwindow* window, int width, int height);
};