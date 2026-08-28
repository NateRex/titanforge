#pragma once
#include <graphics/core/Color.h>
#include <graphics/core/renderer/pointers/RenderTargetPtr.h>
#include <graphics/core/renderer/RenderModes.h>

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
inline ClearFlags operator|(ClearFlags left, ClearFlags right)
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
    return (static_cast<unsigned int>(flags) & static_cast<unsigned int>(flag)) != 0;
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
 * Describes visualization, target, viewport, and clearing behavior while performing a render pass
 * @author Nathaniel Rex
 */
struct RenderPass
{
    /**
     * Destination render target, or null for the default frame buffer.
     */
    RenderTargetPtr target = nullptr;

    /**
     * Pixel region to receive rendered output. When unspecified, defaults to the dimensions of the target frame buffer.
     */
    Viewport viewport;

    /**
     * Buffers cleared when the pass begins. Defaults to ClearFlags::COLOR | ClearFlags::DEPTH.
     */
    ClearFlags clearFlags = ClearFlags::COLOR | ClearFlags::DEPTH;

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
     * Boolean flag that, when true, enables depth testing for this rendering pass. Defaults to true.
     */
    bool depthTest = true;
};
