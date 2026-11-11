#pragma once

/**
 * Saves the global OpenGL state to be restored when leaving scope, including during exception unwinding.
 * @author Nathaniel Rex
 */
struct ScopedRenderState
{
    /**
     * Draw framebuffer binding to restore
     */
    int drawFrameBuffer;

    /**
     * Read framebuffer binding to restore
     */
    int readFrameBuffer;

    /**
     * Saved viewport dimensions: x, y, width, and height
     */
    int viewport[4];

    /**
     * Shader program that was previously active
     */
    int program;

    /**
     * Vertex array object that was previously bound
     */
    int vao;

    /**
     * Previously active texture unit, expressed as GL_TEXTURE0 plus its index
     */
    int activeTexture;

    /**
     * Two-dimensional texture previously bound to texture unit zero
     */
    int texture;

    /**
     * Depth comparison function that was previously configured
     */
    int depthFunction;

    /**
     * Face selection previously used for culling. Stored independently of whether or not face culling was enabled.
     */
    int cullFace;

    /**
     * Whether writes to the depth buffer were previously enabled
     */
    unsigned char depthWrite;

    /**
     * Depth value previously used when clearing the depth buffer
     */
    double clearDepth;

    /**
     * Whether depth testing was previously enabled
     */
    unsigned char depth;

    /**
     * Whether blending was previously enabled
     */
    unsigned char blend;

    /**
     * Whether face culling was previously enabled
     */
    unsigned char cull;

    /**
     * Whether framebuffer sRGB conversion was previously enabled
     */
    unsigned char srgb;

    /**
     * Constructor. Captures the current global OpenGL state to be restored when this object
     * goes out of scope. Leaves texture unit zero active.
     */
    ScopedRenderState();

    /**
     * Restores all captured global OpenGL settings, including bindings, settings, and flags. Restores
     * texture unit zero's two-dimensional texture binding before reactivating the previously active texture unit.
     */
    ~ScopedRenderState();
};