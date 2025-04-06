#pragma once
#include <graphics/core/windows/pointers/WindowPtr.h>

struct GLFWwindow;
class InputController;

/**
 * A window target for rendering
 * @author Nathaniel Rex
 */
class Window
{
public:

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

private:

    /**
     * Boolean flag that, when true, indicates that GLFW has been initialized 
     */
    static bool GLFW_INIT;

    /**
     * Boolean flag that, when true, indicates that GLFW should be initialized in "headless mode",
     * which prevents the display of windows. This is primarily useful in test environments.
     */
    static bool HEADLESS;

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
     * Initializes GLFW. This should only be done once, on creation of the first window.
     */
    static void initGLFW();

    /**
     * Callback method that is triggered whenever a window is resized
     * @param window The GLFW window pointer
     * @param width New window width (in pixels)
     * @param height New window height (in pixels)
     */
    static void onResize(GLFWwindow* window, int width, int height);
};