#pragma once

/**
 * Representation of a window target for rendering
 * @author Nathaniel Rex
 */
class Window
{
public:

    Window();

private:

    /**
     * A pointer to the GLFW window object
     */
    GLFWwindow* _glfwWindow;

};