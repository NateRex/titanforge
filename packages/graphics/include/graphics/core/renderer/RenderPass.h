#pragma once
#include <graphics/core/Color.h>

class RenderTarget;

/**
 * Bit flags representing buffers cleared by a render pass
 * @author Nathaniel Rex
 */
enum class ClearFlags : unsigned int
{
    /**
     * Preserves all frame buffer contents
     */
    NONE = 0,

    /**
     * Clears the color buffers
     */
    COLOR = 1,

    /**
     * Clears the depth buffer
     */
    DEPTH = 2,

    /**
     * Clears the stencil buffer
     */
    STENCIL = 4
};

/**
 * Combines two clear-flag values
 * @param left First flag set
 * @param right Second flag set
 * @return The union of both flag sets
 */
inline ClearFlags::operator|(ClearFlags left, ClearFlags right)
{
    return static_cast<ClearFlags>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right));
}

/**
 * Tests whether a clear-flag set contains a given flag
 * @param flags Flag set to inspect
 * @param flag Flag to search for
 * @return True if the flag is present in the set. Returns false otherwise.
 */
inline bool hasFlag(ClearFlags flags, ClearFlags flag)
{
    return (static_cast<unsigned int>(flags) & static_cast<unsigned_int>(flag)) != 0;
}

/**
 * Defines a rectangular rendering region, in pixels
 * @author Nathaniel Rex
 */
struct Viewport
{
    /**
     * Horizontal coordinate of the viewport's lower-left corner
     */
    int x = 0;

    /**
     * Vertical coordinate of the viewport's lower-left corner
     */
    int y = 0;

    /**
     * Viewport width, in pixels
     */
    unsigned int width = 0;

    /**
     * Viewport height, in pixels
     */
    unsigned int height = 0;
};

/**
 * Describes render target selection and fixed-function render-pass state
 * @author Nathaniel Rex
 */
struct RenderPass
{
    /**
     * Destination render target, or null for the default frame buffer.
     */
    const RenderTarget* target = nullptr;

    /**
     * Pixel region receiving rendered output
     */
    Viewport viewport;

    /**
     * Buffers cleared when the pass begins. Defaults to ClearFlags::COLOR & ClearFlags::DEPTH.
     */
    ClearFlags clearFlags;

    /**
     * Color value used when clearing color buffers. Defaults to black.
     */
    Color clearColor = Color::BLACK;

    /**
     * Depth value used when clearing the depth buffer. Defaults to 1.
     */
    float clearDepth = 1.f;

    /**
     * Stencil value used when clearing the stencil buffer.
     */
    int clearStencil = 0;

    /**
     * Boolean flag indicating whether depth testing is enabled. Defaults to true.
     */
    bool depthTest = true;

    /**
     * Boolean flag indicating whether fragment depth values are written. Defaults to true.
     */
    bool depthWrite = true;

    /**
     * Boolean flag indicating whether color blending is enabled. Defaults to true.
     */
    bool blending = true;

    /**
     * Boolean flag indicating whether polygon face culling is enabled. Defaults to true.
     */
    bool faceCulling = true;
};