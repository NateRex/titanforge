#pragma once
#include <graphics/core/buffers/pointers/FrameBufferPtr.h>
#include <graphics/core/buffers/pointers/RenderBufferPtr.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <string>

/**
 * Identifies an attachment point on a frame buffer
 * @author Nathaniel Rex
 */
enum class FrameBufferAttachment
{
    /**
     * First color attachment
     */
    COLOR0,

    /**
     * Second color attachment
     */
    COLOR1,

    /**
     * Third color attachment
     */
    COLOR2,

    /**
     * Fourth color attachment
     */
    COLOR3,

    /**
     * Depth-only attachment
     */
    DEPTH,

    /**
     * Stencil-only attachment
     */
    STENCIL,

    /**
     * Combined depth-stencil attachment
     */
    DEPTH_STENCIL
};

/**
 * Converts an engine frame buffer attachment to its OpenGL representation
 * @param attachment Attachment to convert
 * @return The corresponding OpenGL attachment
 */
unsigned int toGLAttachment(FrameBufferAttachment attachment);

/**
 * A collection of buffers that can be used as the destination for rendering in a render target
 * @author Nathaniel Rex
 */
class FrameBuffer
{
public:

    /**
     * Destructor
     */
    ~FrameBuffer();

    /**
     * @return A newly-created frame buffer object
     */
    static FrameBufferPtr create();

    /**
     * Binds the default frame buffer
     */
    static void bindDefault();

    /**
     * @return The OpenGL frame buffer object identifier
     */
    unsigned int id() const { return _id; }

    /**
     * Binds this frame buffer as the current framebuffer
     */
    void bind() const;

    /**
     * Attaches a texture to this frame buffer
     * @param attachment Attachment point
     * @param texture Texture to attach
     */
    void attach(FrameBufferAttachment attachment, const TexturePtr texture);

    /**
     * Attaches a render buffer to this frame buffer
     * @param attachment Attachment point
     * @param renderBuffer Render buffer
     */
    void attach(FrameBufferAttachment attachment, const RenderBufferPtr renderBuffer);

    /**
     * @return True if this frame buffer has a complete attachment configuration
     */
    bool isComplete() const;

    /**
     * Asserts that this frame buffer is complete
     * @throws IllegalStateException If the frame buffer is incomplete
     */
    void assertComplete() const;

private:

    /**
     * OpenGL frame buffer object identifier
     */
    unsigned int _id = 0;

    /**
     * Constructor
     */
    FrameBuffer();

    /**
     * @return A description of the current status of this frame buffer
     */
    std::string status() const;
};