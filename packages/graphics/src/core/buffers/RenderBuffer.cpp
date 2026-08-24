#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/core/PixelFormats.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

RenderBuffer::RenderBuffer(const RenderBufferConfig& config): _config(config)
{
    if (_config.width <= 0 || _config.height <= 0 || _config.samples <= 0)
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

RenderBufferPtr RenderBuffer::create(const RenderBufferConfig& config)
{
    return std::shared_ptr<RenderBuffer>(new RenderBuffer(config));
}

void RenderBuffer::allocate()
{
    int previous = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &previous);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
    int glFormat = toGLFormat(_config.format).internalFormat;
    if (_config.samples > 1)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, _config.samples, glFormat, _config.width, _config.height);
    }
    else {
        glRenderbufferStorage(GL_RENDERBUFFER, glFormat, _config.width, _config.height);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, previous);
}

void RenderBuffer::resize(unsigned int width, unsigned int height)
{
    if (width <= 0 || height <= 0)
    {
        throw IllegalArgumentException("Renderbuffer dimensions must be greater than zero");
    }

    _config.width = width;
    _config.height = height;
    allocate();
}