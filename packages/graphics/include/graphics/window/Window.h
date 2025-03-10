#pragma once
#include <memory>
#include <graphics/Color.h>

struct GLFWwindow;
class InputController;

/**
 * Representation of a window target for rendering
 * @author Nathaniel Rex
 */
class Window
{
public:

    friend class Engine;

    /**
     * @return The input controller
     */
    InputController* getInputController();

    /**
     * @return True if this window is currently open. Returns false otherwise.
     */
    bool isOpen() const;

    /**
     * Closes the window
     */
    void close();

    /**
     * Sets the background (clear) color
     * @param color Color
     */
    void setBackgroundColor(const Color color);

private:

    /**
     * A pointer to the GLFW window object
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
     */
    Window();

    /**
     * Makes this window the target of the current rendering context
     */
    void makeCurrent();

    /**
     * Callback method that is triggered whenever the window is resized
     * @param window The GLFW window pointer
     * @param width New window width (in pixels)
     * @param height New window height (in pixels)
     */
    static void onResize(GLFWwindow* window, int width, int height);
};