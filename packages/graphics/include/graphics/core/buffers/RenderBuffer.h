#pragma once
#include <graphics/core/buffers/pointers/RenderBufferPtr.h>
#include <graphics/core/PixelFormat.h>

/**
 * Describes the storage allocated to an OpenGL render buffer
 * @author Nathaniel Rex
 */
struct RenderBufferDescriptor
{
    /**
     * Width in pixels
     */
    unsigned int width = 1;

    /**
     * Height in pixels
     */
    unsigned int height = 1;

    /**
     * Internal storage format. Defaults to PixelFormat::DEPTH24_STENCIL8.
     */
    PixelFormat format = PixelFormat::DEPTH24_STENCIL8;

    /**
     * Number of multisampling samples per pixel. Defaults to 1.
     */
    unsigned int samples = 1;
};

/**
 * A memory container used to store a single image-based rendering target, such as color, depth and/or stencil.
 * @author Nathaniel Rex
 */
class RenderBuffer
{
public:

    /**
     * Destructor
     */
    ~RenderBuffer();

    /**
     * Creates and allocates a new renderbuffer
     * @param descriptor Storage configuration
     * @return The newly created renderbuffer
     */
    static RenderBufferPtr create(const RenderBufferDescriptor& descriptor);

    /**
     * @return The OpenGL renderbuffer object identifier
     */
    unsigned int id() const { return _id; }

    /**
     * @return The descriptor currently used by this renderbuffer
     */
    const RenderBufferDescriptor& descriptor() const { return _descriptor; }

private:

    /**
     * OpenGL renderbuffer object identifier
     */
    unsigned int _id = 0;

    /**
     * Current storage configuration
     */
    RenderBufferDescriptor _descriptor;

    /**
     * Constructor
     * @param descriptor Renderbuffer descriptor
     */
    RenderBuffer(const RenderBufferDescriptor& descriptor);

    /**
     * Allocates storage according to the current descriptor
     */
    void allocate();
};