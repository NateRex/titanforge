#pragma once
#include <graphics/core/Renderer.h>

/**
 * Test fixture that instantiates a new static renderer instance for use across tests.
 * This, in turn, results in GLFW and GLAD being initialized.
 * @author Nathaniel Rex
 */
class GlobalTestFixture
{
public:

    /**
     * Global renderer for use across tests
     */
    static Renderer* RENDERER;

    /**
     * Code to run before all tests
     */
    GlobalTestFixture();

    /**
     * Code to run after all tests
     */
    ~GlobalTestFixture();
};