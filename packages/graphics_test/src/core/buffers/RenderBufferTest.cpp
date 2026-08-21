#include <boost/test/unit_test.hpp>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <glad/glad.h>

/**
 * Helper method that determines the value for an OpenGL paramater of a render buffer
 * @param buffer Render buffer
 * @param parameter OpenGL parameter
 * @return Integer value for that parameter on the buffer
 */
int renderBufferParameter(const RenderBuffer& renderBuffer, unsigned int parameter)
{
    int previous = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &previous);

    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer.id());
    int value = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, parameter, &value);

    glBindRenderbuffer(GL_RENDERBUFFER, previous);
    return value;
}

/**
 * Helper method that returns the ID of the currently-bound render buffer
 * @return The render buffer ID
 */
int boundRenderBuffer()
{
    int binding = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &binding);
    return binding;
}

/**
 * Tests the default config values and creation of a render buffer
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_basics)
{
    RenderBufferConfig config;
    BOOST_TEST(config.width == 1);
    BOOST_TEST(config.height == 1);
    BOOST_TEST(config.format == PixelFormat::DEPTH24_STENCIL8);
    BOOST_TEST(config.samples == 1);

    RenderBufferPtr renderBuffer = RenderBuffer::create(config);
    BOOST_REQUIRE(renderBuffer != nullptr);
    BOOST_TEST(renderBuffer->id() != 0);
    BOOST_TEST(renderBuffer->config().width == config.width);
    BOOST_TEST(renderBuffer->config().height == config.height);
    BOOST_TEST(renderBuffer->config().format == config.format);
    BOOST_TEST(renderBuffer->config().samples == config.samples);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_WIDTH) == 1);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_HEIGHT) == 1);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_INTERNAL_FORMAT) == GL_DEPTH24_STENCIL8);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_SAMPLES) == 0);
}

/**
 * Tests custom multisampled storage and preservation of the current binding during allocation
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_multiSampledStorage)
{
    RenderBufferConfig previousConfig;
    previousConfig.format = PixelFormat::RGBA8;

    RenderBufferPtr previous = RenderBuffer::create(previousConfig);
    glBindRenderbuffer(GL_RENDERBUFFER, previous->id());

    RenderBufferConfig config;
    config.width = 8;
    config.height = 4;
    config.format = PixelFormat::RGBA8;
    config.samples = 2;

    RenderBufferPtr renderBuffer = RenderBuffer::create(config);
    BOOST_TEST(boundRenderBuffer() == previous->id());

    // The requested sample count is a minimum. OpenGL implementations may allocate the next supported sample count instead,
    // making the final count platform-dependent
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_SAMPLES) >= config.samples);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_INTERNAL_FORMAT) == GL_RGBA8);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * Tests resizing of buffer while preserving the format, sample count, and current binding
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_resize)
{
    RenderBufferConfig config;
    config.width = 2;
    config.height = 3;
    config.format = PixelFormat::RGBA8;

    RenderBufferPtr renderBuffer = RenderBuffer::create(config);
    RenderBufferPtr previous = RenderBuffer::create(config);
    glBindRenderbuffer(GL_RENDERBUFFER, previous->id());

    renderBuffer->resize(7, 5);
    BOOST_TEST(boundRenderBuffer() == previous->id());

    BOOST_TEST(renderBuffer->config().width == 7);
    BOOST_TEST(renderBuffer->config().height == 5);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_WIDTH) == 7);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_HEIGHT) == 5);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * Tests validation of dimensions and sample counts during creation and resize.
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_failureCases)
{
    RenderBufferConfig config;
    config.width = 0;
    BOOST_CHECK_THROW(RenderBuffer::create(config), IllegalArgumentException);

    config.width = 1;
    config.height = 0;
    BOOST_CHECK_THROW(RenderBuffer::create(config), IllegalArgumentException);

    config.height = 1;
    config.samples = 0;
    BOOST_CHECK_THROW(RenderBuffer::create(config), IllegalArgumentException);

    config.samples = 1;
    RenderBufferPtr renderBuffer = RenderBuffer::create(config);
    BOOST_CHECK_THROW(renderBuffer->resize(0, 1), IllegalArgumentException);
    BOOST_CHECK_THROW(renderBuffer->resize(1, 0), IllegalArgumentException);
    BOOST_TEST(renderBuffer->config().width == 1);
    BOOST_TEST(renderBuffer->config().height == 1);
}
