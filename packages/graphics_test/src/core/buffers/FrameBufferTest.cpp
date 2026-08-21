#include <boost/test/unit_test.hpp>
#include <common/exceptions/IllegalStateException.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/textures/Texture.h>
#include <glad/glad.h>

/**
 * Helper method that returns the ID of the currently-bound frame buffer
 * @return The frame buffer ID
 */
int boundFrameBuffer()
{
    int binding = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &binding);
    return binding;
}

/**
 * Tests conversion from engine attachment points to OpenGL attachment constants.
 */
BOOST_AUTO_TEST_CASE(FrameBuffer_attachmentConversion)
{
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::COLOR0) == GL_COLOR_ATTACHMENT0);
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::COLOR1) == GL_COLOR_ATTACHMENT1);
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::COLOR2) == GL_COLOR_ATTACHMENT2);
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::COLOR3) == GL_COLOR_ATTACHMENT3);
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::DEPTH) == GL_DEPTH_ATTACHMENT);
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::STENCIL) == GL_STENCIL_ATTACHMENT);
    BOOST_TEST(toGLAttachment(FrameBufferAttachment::DEPTH_STENCIL) == GL_DEPTH_STENCIL_ATTACHMENT);
}

/**
 * Tests frame buffer creation and explicit/default binding
 */
BOOST_AUTO_TEST_CASE(FrameBuffer_creationAndBinding)
{
    FrameBufferPtr frameBuffer = FrameBuffer::create();

    BOOST_REQUIRE(frameBuffer != nullptr);
    BOOST_TEST(frameBuffer->id() != 0);

    frameBuffer->bind();
    BOOST_TEST(boundFrameBuffer() == frameBuffer->id());

    FrameBuffer::bindDefault();
    BOOST_TEST(boundFrameBuffer() == 0);
}

/**
 * Tests the ability to make color attachments, while preserving the previously bound frame buffer
 */
BOOST_AUTO_TEST_CASE(FrameBuffer_textureAttachment)
{
    FrameBufferPtr previous = FrameBuffer::create();
    FrameBufferPtr frameBuffer = FrameBuffer::create();
    TextureConfig descriptor;
    descriptor.width = 4;
    descriptor.height = 4;
    TexturePtr texture = Texture::create(descriptor);

    previous->bind();
    frameBuffer->attach(FrameBufferAttachment::COLOR0, texture);
    BOOST_TEST(boundFrameBuffer() == previous->id());

    BOOST_TEST(frameBuffer->isComplete());
    BOOST_TEST(boundFrameBuffer() == previous->id());

    BOOST_REQUIRE_NO_THROW(frameBuffer->assertComplete());
    BOOST_TEST(boundFrameBuffer() == previous->id());

    frameBuffer->attach(FrameBufferAttachment::COLOR0, TexturePtr(nullptr));
    BOOST_TEST(!frameBuffer->isComplete());
    BOOST_CHECK_THROW(frameBuffer->assertComplete(), IllegalStateException);

    FrameBuffer::bindDefault();
}

/**
 * Tests the ability to make render buffer attachments, while preserving the previously bound frame buffer
 */
BOOST_AUTO_TEST_CASE(FrameBuffer_renderBufferAttachment)
{
    FrameBufferPtr previous = FrameBuffer::create();
    FrameBufferPtr frameBuffer = FrameBuffer::create();
    RenderBufferConfig descriptor;
    descriptor.width = 4;
    descriptor.height = 4;
    RenderBufferPtr renderBuffer = RenderBuffer::create(descriptor);

    previous->bind();
    frameBuffer->attach(FrameBufferAttachment::DEPTH_STENCIL, renderBuffer);
    BOOST_TEST(boundFrameBuffer() == previous->id());
    
    BOOST_TEST(frameBuffer->isComplete());
    BOOST_REQUIRE_NO_THROW(frameBuffer->assertComplete());

    frameBuffer->attach(FrameBufferAttachment::DEPTH_STENCIL, RenderBufferPtr(nullptr));
    BOOST_TEST(!frameBuffer->isComplete());

    FrameBuffer::bindDefault();
}

/**
 * Tests the behavior of a frame buffer with no attachments
 */
BOOST_AUTO_TEST_CASE(FrameBuffer_incompleteWithoutAttachments)
{
    FrameBufferPtr frameBuffer = FrameBuffer::create();

    BOOST_TEST(!frameBuffer->isComplete());
    BOOST_CHECK_EXCEPTION(
        frameBuffer->assertComplete(),
        IllegalStateException,
        [](const IllegalStateException& exception)
        {
            return std::string(exception.what()) == "Frame buffer is incomplete: no images are attached";
        });
}
