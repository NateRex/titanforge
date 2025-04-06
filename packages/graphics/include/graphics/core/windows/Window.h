#pragma once
#include <memory>
#include <string>
#include <graphics/core/Color.h>

struct GLFWwindow;
class InputController;

/**
 * Representation of a window target for rendering
 * @author Nathaniel Rex
 */
class Window
{
public:

    friend class WindowManager;
    friend class Engine;

    /**
     * The unique name of this window
     */
    const std::string name;

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
     * Sets the background (clear) color
     * @param color Color
     */
    void setBackgroundColor(const Color color);

private:

    /**
     * A pointer to the GLFW window object. Will be null until this window is constructed explicitly via
     * create()
     */
    GLFWwindow* _glfwWindow;

    /**
     * Input controller
     */
    std::shared_ptr<InputController> _inputController;

    /**
     * Background (clear) color. Defaults to black.
     */
    Color _clearColor;

    /**
     * Constructor.
     * @param name Unique name to associate with this window
     */
    Window(const std::string& name);

    /**
     * Creates the OpenGL resources for this window
     * @param width Starting width of the window (in pixels).
     * @param height Starting height of the window (in pixels).
     */
    void create(unsigned int width, unsigned int height);

    /**
     * Destroys this window, freeing its resources
     */
    void destroy();

    /**
     * Callback method that is triggered whenever a window is resized
     * @param window The GLFW window pointer
     * @param width New window width (in pixels)
     * @param height New window height (in pixels)
     */
    static void onResize(GLFWwindow* window, int width, int height);
};