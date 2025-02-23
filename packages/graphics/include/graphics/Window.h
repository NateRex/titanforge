#pragma once

struct GLFWwindow;

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
     * @return True if this window has been instructed to close. Returns false otherwise.
     */
    bool closed() const;

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
     * Callback method that is triggered whenever the window is resized
     * @param window The GLFW window pointer
     * @param width New window width (in pixels)
     * @param height New window height (in pixels)
     */
    static void onResize(GLFWwindow* window, int width, int height);
};