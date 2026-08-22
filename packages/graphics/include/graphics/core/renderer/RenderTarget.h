#pragma once
#include <graphics/core/buffers/pointers/FrameBufferPtr.h>
#include <graphics/core/buffers/pointers/RenderBufferPtr.h>
#include <graphics/core/PixelFormats.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <vector>

/**
 * Describes how a render targte stores its depth-stencil attachment
 * @author Nathaniel Rex
 */
enum class DepthStencilStorage
{
    /**
     * No depth-stencil attachment
     */
    NONE,

    /**
     * Depth-stencil data is stored in a sampleable texture
     */
    TEXTURE,

    /**
     * Depth-stencil data is stored in a render buffer
     */
    RENDERBUFFER
};

/**
 * Describes the dimensions and attachments of a render target
 * @author Nathaniel Rex
 */
struct RenderTargetConfig
{
    /**
     * Render target width in pixels
     */
    unsigned int width = 1;

    /**
     * Render target height in pixels
     */
    unsigned int height = 1;

    /**
     * Formats for the ordered set of color attachments. The size of this vector determines the number of color attachments.
     * Defaults to one color attachment, with format PixelFormat::RGBA8.
     */
    std::vector<PixelFormat> colorFormats = { PixelFormat::RGBA8 };

    /**
     * Storage type for the depth-stencil attachment. Defaults to DepthStencilStorage::RENDERBUFFER.
     */
    DepthStencilStorage depthStencilStorage = DepthStencilStorage::RENDERBUFFER;

    /**
     * Format of the optional depth-stencil attachment. Defaults to PixelFormat::DEPTH24_STENCIL8.
     */
    PixelFormat depthStencilFormat = PixelFormat::DEPTH24_STENCIL8;

};

/**
 * A render target describes an off-screen destination for rendering by grouping one or more color outputs with an optional
 * depth-stencil output. It can be bound as the destination of a render pass, allowing rendered results to be sampled by later
 * passes or used for post-processing.
 * @author Nathaniel Rex
 */
class RenderTarget
{
public:

    /**
     * Constructor
     * @param config Render target config
     */
    RenderTarget(const RenderTargetConfig& config);

    /**
     * @return The config currently used by this render target
     */
    const RenderTargetConfig& config() const { return _config; }

    /**
     * @return The frame buffer backing this render target
     */
    const FrameBufferPtr frameBuffer() const { return _frameBuffer; }

    /**
     * @return The ordered collection of color attachment textures
     */
    const std::vector<TexturePtr>& colorTextures() const;

    /**
     * Retrieves the color attachment texture for a given index
     * @param index Zero-based color attachment index
     * @return The color attachment texture for the given index
     * @throws IllegalArgumentException If the given index is out of bounds
     */
    TexturePtr colorTexture(unsigned int index) const;

    /**
     * @return The depth-stencil texture. May be null in cases where a depth-stencil attachment isn't used, or a render buffer is
     * used instead.
     */
    const TexturePtr depthStencilTexture() const { return _depthStencilTexture; }

    /**
     * @return The depth-stencil render buffer. May be null in cases where a depth-stencil attachment isn't used, or a texture is
     * used instead.
     */
    const RenderBufferPtr depthStencilRenderBuffer() const { return _depthStencilRenderBuffer; }

    /**
     * Resizes this render target and rebuilds all of its attachments
     * @param width New width in pixels
     * @param height New height in pixels
     */
    void resize(unsigned int width, unsigned int height);

private:

    /**
     * Current attachment and dimension configuration
     */
    RenderTargetConfig _config;

    /**
     * Frame buffer containing the render target attachments
     */
    FrameBufferPtr _frameBuffer;

    /**
     * Ordered color attachment textures
     */
    std::vector<TexturePtr> _colorTextures;

    /**
     * Texture supporting the depth-stencil attachment. May be null in cases where a depth-stencil attachment isn't used,
     * or a render buffer is used instead.
     */
    TexturePtr _depthStencilTexture;

    /**
     * Render buffer supporting the depth-stencil attachment. May be null in cases where a depth-stencil attachment isn't used,
     * or a texture is used instead.
     */
    RenderBufferPtr _depthStencilRenderBuffer;

    /**
     * Builds the frame buffer and its attachments, as described by the current config
     */
    void build();

};