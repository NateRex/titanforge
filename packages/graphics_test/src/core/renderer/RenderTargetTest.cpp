#include <boost/test/unit_test.hpp>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/IllegalStateException.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/renderer/ScopedRenderState.h>
#include <graphics/textures/Texture2D.h>
#include <glad/glad.h>

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
    config.depthStencilStorage = DepthStencilStorage::TEXTURE_2D;

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
    config.depthStencilStorage = DepthStencilStorage::TEXTURE_2D;
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
 * Tests that multisampling is applied consistently to every framebuffer attachment.
 */
BOOST_AUTO_TEST_CASE(RenderTarget_multisampledAttachments)
{
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = 4;
    config.height = 3;
    config.samples = 2;

    RenderTargetPtr renderTarget = RenderTarget::create(config);
    BOOST_REQUIRE(renderTarget->colorTexture(0) != nullptr);
    BOOST_TEST(renderTarget->colorTexture(0)->type() == GL_TEXTURE_2D_MULTISAMPLE);
    BOOST_REQUIRE(renderTarget->depthStencilRenderBuffer() != nullptr);
    BOOST_TEST(renderTarget->depthStencilRenderBuffer()->config().samples == config.samples);
    BOOST_TEST(renderTarget->frameBuffer()->isComplete());
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

    config.samples = 0;
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);
}


/**
 * Tests the ability to use cube textures as storage for the color attachments
 */
BOOST_AUTO_TEST_CASE(RenderTarget_cubeColorStorage)
{
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = config.height = 8;
    config.colorFormats = {PixelFormat::RGBA8, PixelFormat::RGBA16F};
    config.colorStorage = ColorStorage::TEXTURE_CUBE;
    for (auto depth : {DepthStencilStorage::NONE, DepthStencilStorage::TEXTURE_CUBE})
    {
        config.depthStencilStorage = depth;
        auto target = RenderTarget::create(config);
        BOOST_TEST(target->frameBuffer()->isComplete());
        for (unsigned int i = 0; i < 2; ++i) {
            auto texture = target->colorTexture(i);
            BOOST_TEST(texture->type() == GL_TEXTURE_CUBE_MAP);
            BOOST_TEST(texture->format() == config.colorFormats[i]);
            BOOST_TEST(texture->width() == 8u);
        }
        target->frameBuffer()->bind();
        GLint layered;
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_LAYERED, &layered);
        BOOST_TEST(layered == GL_TRUE);
        auto texture = target->colorTexture(0);
        target->resize(16, 16);
        BOOST_TEST(target->colorTexture(0) == texture);
        BOOST_TEST(texture->width() == 16u);
        BOOST_TEST(target->frameBuffer()->isComplete());
    }
    FrameBuffer::bindDefault();
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Tests the ability to use cube textures as storage for the depth attachment
 */
BOOST_AUTO_TEST_CASE(RenderTarget_cubeDepthStorage)
{
    ScopedRenderState restore;
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = config.height = 8;
    config.colorFormats.clear();
    config.depthStencilStorage = DepthStencilStorage::TEXTURE_CUBE;
    for (auto format : {PixelFormat::DEPTH32F, PixelFormat::DEPTH24_STENCIL8})
    {
        config.depthStencilFormat = format;
        auto target = RenderTarget::create(config);
        auto texture = target->depthStencilTexture();
        BOOST_REQUIRE(texture != nullptr);
        BOOST_TEST(texture->type() == GL_TEXTURE_CUBE_MAP);
        BOOST_TEST(texture->format() == format);
        BOOST_TEST(target->depthStencilRenderBuffer() == nullptr);
        BOOST_TEST(target->frameBuffer()->isComplete());
        target->frameBuffer()->bind();

        GLint layered, drawBuffer, readBuffer;
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_FRAMEBUFFER_ATTACHMENT_LAYERED, &layered);
        glGetIntegerv(GL_DRAW_BUFFER, &drawBuffer);
        glGetIntegerv(GL_READ_BUFFER, &readBuffer);
        BOOST_TEST(layered == GL_TRUE);
        BOOST_TEST(drawBuffer == GL_NONE);
        BOOST_TEST(readBuffer == GL_NONE);

        target->resize(16, 16);
        BOOST_TEST(target->depthStencilTexture() == texture);
        BOOST_TEST(texture->width() == 16u);
        BOOST_TEST(texture->height() == 16u);
        BOOST_TEST(target->frameBuffer()->isComplete());
        BOOST_CHECK_THROW(target->resize(8, 4), IllegalArgumentException);
        BOOST_TEST(target->config().width == 16);
        BOOST_TEST(target->config().height == 16);
        BOOST_TEST(texture->width() == 16u);
        BOOST_TEST(target->frameBuffer()->isComplete());
    }
    
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}

/**
 * Tests exception cases when specifying an invalid cube storage configuration
 */
BOOST_AUTO_TEST_CASE(RenderTarget_invalidCubeStorage)
{
    for (bool cubeDepth : {false, true})
    {
        RenderTargetConfig config;
        config.sizeMode = TargetSizeMode::FIXED;
        config.width = 8;
        config.height = 4;
        config.depthStencilStorage = cubeDepth ? DepthStencilStorage::TEXTURE_CUBE : DepthStencilStorage::NONE;
        config.colorFormats = cubeDepth ? std::vector<PixelFormat>{} : std::vector<PixelFormat>{PixelFormat::RGBA8};
        config.colorStorage = cubeDepth ? ColorStorage::TEXTURE_2D : ColorStorage::TEXTURE_CUBE;
        BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);
        config.height = 8;
        config.samples = 2;
        BOOST_CHECK_THROW(RenderTarget::create(config), IllegalArgumentException);
    }
    RenderTargetConfig config;
    config.sizeMode = TargetSizeMode::FIXED;
    config.width = config.height = 8;
    config.depthStencilStorage = DepthStencilStorage::TEXTURE_CUBE;
    // A layered depth attachment cannot share a framebuffer with ordinary 2D color storage.
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalStateException);
    config.colorStorage = ColorStorage::TEXTURE_CUBE;
    config.depthStencilStorage = DepthStencilStorage::RENDERBUFFER;
    BOOST_CHECK_THROW(RenderTarget::create(config), IllegalStateException);
    BOOST_TEST(glGetError() == GL_NO_ERROR);
}
