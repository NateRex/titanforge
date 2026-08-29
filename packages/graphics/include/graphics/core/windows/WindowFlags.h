#pragma once

/**
 * Bit flags representing global window settings
 * @author Nathaniel Rex
 */
enum class WindowFlags : unsigned int
{
    /**
     * No customized window settings
     */
    NONE = 0,

    /**
     * Enables anti-aliasing, resulting in smoother object edges at the cost of some small
     * runtime performance
     */
    ANTI_ALIASING = 1,

    /**
     * Allows the window to be resizable
     */
    RESIZABLE = 2,

    /**
     * Window will be fullscreen by default
     */
    FULLSCREEN = 4,

    /**
     * Enables vsync to ensure the engine framerate matches the monitor's refresh rate,
     * reducing the chance of screen tear problems
     */
    VSYNC = 8
};

/**
 * Combines two window flag values
 * @param left First flag set
 * @param right Second flag set
 * @return The union of both flag sets
 */
inline WindowFlags operator|(WindowFlags left, WindowFlags right)
{
    return static_cast<WindowFlags>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right));
}

/**
 * Tests whether a window flag set contains a given flag
 * @param flags Flag set to inspect
 * @param flag Flag to search for
 * @return True if the flag is present in the set. Returns false otherwise.
 */
inline bool hasFlag(WindowFlags flags, WindowFlags flag)
{
    return (static_cast<unsigned int>(flags) & static_cast<unsigned int>(flag)) != 0;
}