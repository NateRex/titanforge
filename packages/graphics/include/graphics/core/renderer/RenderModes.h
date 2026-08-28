#pragma once

/**
 * Bit flags representing additional scene visualization modes
 * @author Nathaniel Rex
 */
enum class RenderModes : unsigned int
{
    /**
     * Default rendering using each item's material.
     */
    MATERIAL = 1,

    /**
     * Renders geometry vertex normals for three-dimensional objects
     */
    VERTEX_NORMALS = 2
};

/**
 * Array consisting of all rendering modes
 */
constexpr RenderModes ALL_RENDER_MODES[] = {
    RenderModes::MATERIAL,
    RenderModes::VERTEX_NORMALS
};

/**
 * Combines two render mode flag values
 * @param left First mode set
 * @param right Second mode set
 * @return The union of both mode sets
 */
inline RenderModes operator|(RenderModes left, RenderModes right)
{
    return static_cast<RenderModes>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right));
}

/**
 * Tests whether a render mode set contains a given mode
 * @param modes Mode set to inspect
 * @param mode Mode to search for
 * @return True if the mode is present in the set. Returns false otherwise.
 */
inline bool hasFlag(RenderModes modes, RenderModes mode)
{
    return (static_cast<unsigned int>(modes) & static_cast<unsigned int>(mode)) != 0;
}