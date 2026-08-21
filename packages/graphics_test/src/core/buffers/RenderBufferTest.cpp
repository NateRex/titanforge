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
 * Tests the default descriptor values and creation of a render buffer
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_basics)
{
    RenderBufferDescriptor descriptor;
    BOOST_TEST(descriptor.width == 1);
    BOOST_TEST(descriptor.height == 1);
    BOOST_TEST(descriptor.format == PixelFormat::DEPTH24_STENCIL8);
    BOOST_TEST(descriptor.samples == 1);

    RenderBufferPtr renderBuffer = RenderBuffer::create(descriptor);
    BOOST_REQUIRE(renderBuffer != nullptr);
    BOOST_TEST(renderBuffer->id() != 0);
    BOOST_TEST(renderBuffer->descriptor().width == descriptor.width);
    BOOST_TEST(renderBuffer->descriptor().height == descriptor.height);
    BOOST_TEST(renderBuffer->descriptor().format == descriptor.format);
    BOOST_TEST(renderBuffer->descriptor().samples == descriptor.samples);
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
    RenderBufferDescriptor previousDescriptor;
    previousDescriptor.format = PixelFormat::RGBA8;

    RenderBufferPtr previous = RenderBuffer::create(previousDescriptor);
    glBindRenderbuffer(GL_RENDERBUFFER, previous->id());

    RenderBufferDescriptor descriptor;
    descriptor.width = 8;
    descriptor.height = 4;
    descriptor.format = PixelFormat::RGBA8;
    descriptor.samples = 2;

    RenderBufferPtr renderBuffer = RenderBuffer::create(descriptor);
    BOOST_TEST(boundRenderBuffer() == previous->id());

    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_INTERNAL_FORMAT) == GL_RGBA8);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_SAMPLES) == 2);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * Tests resizing of buffer while preserving the format, sample count, and current binding
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_resize)
{
    RenderBufferDescriptor descriptor;
    descriptor.width = 2;
    descriptor.height = 3;
    descriptor.format = PixelFormat::RGBA8;

    RenderBufferPtr renderBuffer = RenderBuffer::create(descriptor);
    RenderBufferPtr previous = RenderBuffer::create(descriptor);
    glBindRenderbuffer(GL_RENDERBUFFER, previous->id());

    renderBuffer->resize(7, 5);
    BOOST_TEST(boundRenderBuffer() == previous->id());

    BOOST_TEST(renderBuffer->descriptor().width == 7);
    BOOST_TEST(renderBuffer->descriptor().height == 5);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_WIDTH) == 7);
    BOOST_TEST(renderBufferParameter(*renderBuffer, GL_RENDERBUFFER_HEIGHT) == 5);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * Tests validation of dimensions and sample counts during creation and resize.
 */
BOOST_AUTO_TEST_CASE(RenderBuffer_failureCases)
{
    RenderBufferDescriptor descriptor;
    descriptor.width = 0;
    BOOST_CHECK_THROW(RenderBuffer::create(descriptor), IllegalArgumentException);

    descriptor.width = 1;
    descriptor.height = 0;
    BOOST_CHECK_THROW(RenderBuffer::create(descriptor), IllegalArgumentException);

    descriptor.height = 1;
    descriptor.samples = 0;
    BOOST_CHECK_THROW(RenderBuffer::create(descriptor), IllegalArgumentException);

    descriptor.samples = 1;
    RenderBufferPtr renderBuffer = RenderBuffer::create(descriptor);
    BOOST_CHECK_THROW(renderBuffer->resize(0, 1), IllegalArgumentException);
    BOOST_CHECK_THROW(renderBuffer->resize(1, 0), IllegalArgumentException);
    BOOST_TEST(renderBuffer->descriptor().width == 1);
    BOOST_TEST(renderBuffer->descriptor().height == 1);
}
