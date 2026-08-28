#include <boost/test/unit_test.hpp>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/textures/Texture2D.h>

/**
 * Tests using the default config to construct a render target
 */
BOOST_AUTO_TEST_CASE(RenderTarget_basics)
{
    RenderTargetPtr target = RenderTarget::create({
        TargetSizeMode::FIXED, 1, 1
    });

    BOOST_REQUIRE(target->frameBuffer() != nullptr);
    BOOST_TEST(target->frameBuffer()->isComplete());
    BOOST_REQUIRE(target->colorTextures().size() == 1);
    BOOST_TEST(target->colorTexture(0)->width() == 1);
    BOOST_TEST(target->colorTexture(0)->height() == 1);
    BOOST_TEST(target->colorTexture(0)->format() == PixelFormat::RGBA8);
    BOOST_TEST(target->depthStencilTexture() == nullptr);
    BOOST_REQUIRE(target->depthStencilRenderBuffer() != nullptr);
    BOOST_TEST(target->depthStencilRenderBuffer()->config().format == PixelFormat::DEPTH24_STENCIL8);
}

/**
 * Tests creation of an ordered set of color attachments with custom formats
 */
BOOST_AUTO_TEST_CASE(RenderTarget_colorAttachments)
{
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = 8;
    config.height = 4;
    config.colorFormats = { PixelFormat::R8, PixelFormat::RGBA16F, PixelFormat::R32UI };
    config.depthStencilStorage = DepthStencilStorage::NONE;

    RenderTargetPtr renderTarget = RenderTarget::create(config);
    BOOST_REQUIRE(renderTarget->colorTextures().size() == config.colorFormats.size());

    for (unsigned int i = 0; i < config.colorFormats.size(); ++i)
    {
        TexturePtr texture = renderTarget->colorTexture(i);
        BOOST_REQUIRE(texture != nullptr);
        BOOST_TEST(texture->width() == config.width);
        BOOST_TEST(texture->height() == config.height);
        BOOST_TEST(texture->format() == config.colorFormats[i]);
    }
    BOOST_TEST(renderTarget->depthStencilTexture() == nullptr);
    BOOST_TEST(renderTarget->depthStencilRenderBuffer() == nullptr);
    BOOST_TEST(renderTarget->frameBuffer()->isComplete());
}

/**
 * Tests texture-backed depth and depth-stencil attachments.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_depthTexture)
{
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = 5;
    config.height = 3;
    config.colorFormats.clear();
    config.depthStencilStorage = DepthStencilStorage::TEXTURE;

    // Create a render target with each type of depth-stencil format
    PixelFormat formats[] = { PixelFormat::DEPTH32F, PixelFormat::DEPTH24_STENCIL8 };
    for (PixelFormat format : formats)
    {
        config.depthStencilFormat = format;

        RenderTargetPtr renderTarget = RenderTarget::create(config);
        BOOST_TEST(renderTarget->colorTextures().empty());
        BOOST_REQUIRE(renderTarget->depthStencilTexture() != nullptr);
        BOOST_TEST(renderTarget->depthStencilTexture()->width() == config.width);
        BOOST_TEST(renderTarget->depthStencilTexture()->height() == config.height);
        BOOST_TEST(renderTarget->depthStencilTexture()->format() == format);
        BOOST_TEST(renderTarget->depthStencilRenderBuffer() == nullptr);
        BOOST_TEST(renderTarget->frameBuffer()->isComplete());
    }
}

/**
 * Tests resizing all attachments without replacing their objects.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_resize)
{
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = 2;
    config.height = 3;
    config.colorFormats = { PixelFormat::RGB8, PixelFormat::RGBA8 };
    config.depthStencilStorage = DepthStencilStorage::TEXTURE;
    config.depthStencilFormat = PixelFormat::DEPTH32F;

    RenderTargetPtr renderTarget = RenderTarget::create(config);
    FrameBufferPtr frameBuffer = renderTarget->frameBuffer();
    TexturePtr color0 = renderTarget->colorTexture(0);
    TexturePtr color1 = renderTarget->colorTexture(1);
    TexturePtr depth = renderTarget->depthStencilTexture();

    // Resize from original dimensions
    renderTarget->resize(7, 6);
    BOOST_TEST(renderTarget->config().width == 7);
    BOOST_TEST(renderTarget->config().height == 6);
    BOOST_TEST(renderTarget->frameBuffer() == frameBuffer);
    BOOST_TEST(renderTarget->colorTexture(0) == color0);
    BOOST_TEST(renderTarget->colorTexture(1) == color1);
    BOOST_TEST(renderTarget->depthStencilTexture() == depth);
    BOOST_TEST(color0->width() == 7);
    BOOST_TEST(color0->height() == 6);
    BOOST_TEST(color1->width() == 7);
    BOOST_TEST(color1->height() == 6);
    BOOST_TEST(depth->width() == 7);
    BOOST_TEST(depth->height() == 6);
    BOOST_TEST(frameBuffer->isComplete());

    // Resizing with the same values results in no change
    renderTarget->resize(7, 6);
    BOOST_TEST(renderTarget->frameBuffer() == frameBuffer);
    BOOST_TEST(renderTarget->colorTexture(0) == color0);
    BOOST_TEST(renderTarget->depthStencilTexture() == depth);
}

/**
 * Tests invalid dimensions, formats, attachment counts, and color indices.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_failureCases)
{
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = 0;
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);

    config.width = 1;
    config.height = 0;
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);

    config.height = 1;
    config.colorFormats = { PixelFormat::RGBA8, PixelFormat::RGBA8, PixelFormat::RGBA8, PixelFormat::RGBA8, PixelFormat::RGBA8 };
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);

    config.colorFormats = { PixelFormat::DEPTH32F };
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);

    config.colorFormats = { PixelFormat::RGBA8 };
    config.depthStencilFormat = PixelFormat::RGBA8;
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);

    config.depthStencilStorage = DepthStencilStorage::NONE;
    RenderTargetPtr renderTarget = RenderTarget::create(config);
    BOOST_CHECK_THROW(renderTarget->colorTexture(1), IllegalArgumentException);
    BOOST_CHECK_THROW(renderTarget->resize(0, 1), IllegalArgumentException);
    BOOST_CHECK_THROW(renderTarget->resize(1, 0), IllegalArgumentException);
    BOOST_TEST(renderTarget->config().width == 1);
    BOOST_TEST(renderTarget->config().height == 1);
}
