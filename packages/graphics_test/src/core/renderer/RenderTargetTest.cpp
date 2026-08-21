#include <boost/test/unit_test.hpp>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/textures/Texture.h>

/**
 * Tests using the default descriptor to construct a render target
 */
BOOST_AUTO_TEST_CASE(RenderTarget_basics)
{
    RenderTargetConfig descriptor;
    RenderTarget renderTarget(descriptor);

    BOOST_REQUIRE(renderTarget.frameBuffer() != nullptr);
    BOOST_TEST(renderTarget.frameBuffer()->isComplete());
    BOOST_REQUIRE(renderTarget.colorTextures().size() == 1);
    BOOST_TEST(renderTarget.colorTexture(0)->width() == 1);
    BOOST_TEST(renderTarget.colorTexture(0)->height() == 1);
    BOOST_TEST(renderTarget.colorTexture(0)->format() == PixelFormat::RGBA8);
    BOOST_TEST(renderTarget.depthStencilTexture() == nullptr);
    BOOST_REQUIRE(renderTarget.depthStencilRenderBuffer() != nullptr);
    BOOST_TEST(renderTarget.depthStencilRenderBuffer()->descriptor().format == PixelFormat::DEPTH24_STENCIL8);
}

/**
 * Tests creation of an ordered set of color attachments with custom formats
 */
BOOST_AUTO_TEST_CASE(RenderTarget_colorAttachments)
{
    RenderTargetConfig descriptor;
    descriptor.width = 8;
    descriptor.height = 4;
    descriptor.colorFormats = { PixelFormat::R8, PixelFormat::RGBA16F, PixelFormat::R32UI };
    descriptor.depthStencilStorage = DepthStencilStorage::NONE;

    RenderTarget renderTarget(descriptor);
    BOOST_REQUIRE(renderTarget.colorTextures().size() == descriptor.colorFormats.size());

    for (unsigned int i = 0; i < descriptor.colorFormats.size(); ++i)
    {
        TexturePtr texture = renderTarget.colorTexture(i);
        BOOST_REQUIRE(texture != nullptr);
        BOOST_TEST(texture->width() == descriptor.width);
        BOOST_TEST(texture->height() == descriptor.height);
        BOOST_TEST(texture->format() == descriptor.colorFormats[i]);
    }
    BOOST_TEST(renderTarget.depthStencilTexture() == nullptr);
    BOOST_TEST(renderTarget.depthStencilRenderBuffer() == nullptr);
    BOOST_TEST(renderTarget.frameBuffer()->isComplete());
}

/**
 * Tests texture-backed depth and depth-stencil attachments.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_depthTexture)
{
    RenderTargetConfig config;
    config.width = 5;
    config.height = 3;
    config.colorFormats.clear();
    config.depthStencilStorage = DepthStencilStorage::TEXTURE;

    // Create a render target with each type of depth-stencil format
    PixelFormat formats[] = { PixelFormat::DEPTH32F, PixelFormat::DEPTH24_STENCIL8 };
    for (PixelFormat format : formats)
    {
        config.depthStencilFormat = format;

        RenderTarget renderTarget(config);
        BOOST_TEST(renderTarget.colorTextures().empty());
        BOOST_REQUIRE(renderTarget.depthStencilTexture() != nullptr);
        BOOST_TEST(renderTarget.depthStencilTexture()->width() == config.width);
        BOOST_TEST(renderTarget.depthStencilTexture()->height() == config.height);
        BOOST_TEST(renderTarget.depthStencilTexture()->format() == format);
        BOOST_TEST(renderTarget.depthStencilRenderBuffer() == nullptr);
        BOOST_TEST(renderTarget.frameBuffer()->isComplete());
    }
}

/**
 * Tests resizing all attachments without replacing their objects.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_resize)
{
    RenderTargetConfig descriptor;
    descriptor.width = 2;
    descriptor.height = 3;
    descriptor.colorFormats = { PixelFormat::RGB8, PixelFormat::RGBA8 };
    descriptor.depthStencilStorage = DepthStencilStorage::TEXTURE;
    descriptor.depthStencilFormat = PixelFormat::DEPTH32F;

    RenderTarget renderTarget(descriptor);
    FrameBufferPtr frameBuffer = renderTarget.frameBuffer();
    TexturePtr color0 = renderTarget.colorTexture(0);
    TexturePtr color1 = renderTarget.colorTexture(1);
    TexturePtr depth = renderTarget.depthStencilTexture();

    // Resize from original dimensions
    renderTarget.resize(7, 6);
    BOOST_TEST(renderTarget.descriptor().width == 7);
    BOOST_TEST(renderTarget.descriptor().height == 6);
    BOOST_TEST(renderTarget.frameBuffer() == frameBuffer);
    BOOST_TEST(renderTarget.colorTexture(0) == color0);
    BOOST_TEST(renderTarget.colorTexture(1) == color1);
    BOOST_TEST(renderTarget.depthStencilTexture() == depth);
    BOOST_TEST(color0->width() == 7);
    BOOST_TEST(color0->height() == 6);
    BOOST_TEST(color1->width() == 7);
    BOOST_TEST(color1->height() == 6);
    BOOST_TEST(depth->width() == 7);
    BOOST_TEST(depth->height() == 6);
    BOOST_TEST(frameBuffer->isComplete());

    // Resizing with the same values results in no change
    renderTarget.resize(7, 6);
    BOOST_TEST(renderTarget.frameBuffer() == frameBuffer);
    BOOST_TEST(renderTarget.colorTexture(0) == color0);
    BOOST_TEST(renderTarget.depthStencilTexture() == depth);
}

/**
 * Tests invalid dimensions, formats, attachment counts, and color indices.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_failureCases)
{
    RenderTargetConfig descriptor;
    descriptor.width = 0;
    BOOST_CHECK_THROW(RenderTarget rt(descriptor), IllegalArgumentException);

    descriptor.width = 1;
    descriptor.height = 0;
    BOOST_CHECK_THROW(RenderTarget rt(descriptor), IllegalArgumentException);

    descriptor.height = 1;
    descriptor.colorFormats = { PixelFormat::RGBA8, PixelFormat::RGBA8, PixelFormat::RGBA8, PixelFormat::RGBA8, PixelFormat::RGBA8 };
    BOOST_CHECK_THROW(RenderTarget rt(descriptor), IllegalArgumentException);

    descriptor.colorFormats = { PixelFormat::DEPTH32F };
    BOOST_CHECK_THROW(RenderTarget rt(descriptor), IllegalArgumentException);

    descriptor.colorFormats = { PixelFormat::RGBA8 };
    descriptor.depthStencilFormat = PixelFormat::RGBA8;
    BOOST_CHECK_THROW(RenderTarget rt(descriptor), IllegalArgumentException);

    descriptor.depthStencilStorage = DepthStencilStorage::NONE;
    RenderTarget renderTarget(descriptor);
    BOOST_CHECK_THROW(renderTarget.colorTexture(1), IllegalArgumentException);
    BOOST_CHECK_THROW(renderTarget.resize(0, 1), IllegalArgumentException);
    BOOST_CHECK_THROW(renderTarget.resize(1, 0), IllegalArgumentException);
    BOOST_TEST(renderTarget.descriptor().width == 1);
    BOOST_TEST(renderTarget.descriptor().height == 1);
}
