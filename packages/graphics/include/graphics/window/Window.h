#pragma once
#include <functional>

struct GLFWwindow;
class InputController;

/**
 * Representation of a window target for rendering
 * @author Nathaniel Rex
 */
class Window
{
public:

    /**
     * Constructor
     */
    Window();

    /**
     * Destructor
     */
    ~Window();

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
     * Renders the next frame
     */
    void renderFrame() const;

private:

    /**
     * A pointer to the GLFW window object
     */
    GLFWwindow* _glfwWindow;

    /**
     * Input controller
     */
    InputController* _inputController;

    /**
     * Callback method that is triggered whenever the window is resized
     * @param window The GLFW window pointer
     * @param width New window width (in pixels)
     * @param height New window height (in pixels)
     */
    static void onResize(GLFWwindow* window, int width, int height);
};