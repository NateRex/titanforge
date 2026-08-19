#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/core/PixelFormat.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

RenderBuffer::RenderBuffer(const RenderBufferDescriptor& descriptor): _descriptor(descriptor)
{
    if (_descriptor.width <= 0 || _descriptor.height <= 0 || _descriptor.samples <= 0)
    {
        throw IllegalArgumentException("Renderbuffer dimensions and sample count must be greater than zero");
    }

    glGenRenderbuffers(1, &_id);
    allocate();
}

RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &_id);
    _id = 0;
}

RenderBufferPtr RenderBuffer::create(const RenderBufferDescriptor& descriptor)
{
    return std::shared_ptr<RenderBuffer>(new RenderBuffer(descriptor));
}

void RenderBuffer::allocate()
{
    int previous = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &previous);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
    int glFormat = toGLFormat(_descriptor.format).internalFormat;
    if (_descriptor.samples > 1)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, _descriptor.samples, glFormat, _descriptor.width, _descriptor.height);
    }
    else {
        glRenderbufferStorage(GL_RENDERBUFFER, glFormat, _descriptor.width, _descriptor.height);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, previous);
}

void RenderBuffer::resize(unsigned int width, unsigned int height)
{
    if (width <= 0 || height <= 0)
    {
        throw IllegalArgumentException("Renderbuffer dimensions must be greater than zero");
    }

    _descriptor.width = width;
    _descriptor.height = height;
    allocate();
}