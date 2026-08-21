#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalStateException.h>
#include <glad/glad.h>

unsigned int toGLAttachment(FrameBufferAttachment attachment)
{
    switch (attachment)
    {
        case FrameBufferAttachment::COLOR0: return GL_COLOR_ATTACHMENT0;
        case FrameBufferAttachment::COLOR1: return GL_COLOR_ATTACHMENT1;
		case FrameBufferAttachment::COLOR2: return GL_COLOR_ATTACHMENT2;
        case FrameBufferAttachment::COLOR3: return GL_COLOR_ATTACHMENT3;
		case FrameBufferAttachment::DEPTH: return GL_DEPTH_ATTACHMENT;
        case FrameBufferAttachment::STENCIL: return GL_STENCIL_ATTACHMENT;
		case FrameBufferAttachment::DEPTH_STENCIL: return GL_DEPTH_STENCIL_ATTACHMENT;
        default: return GL_DEPTH_STENCIL_ATTACHMENT;
    }
}

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &_id);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &_id);
}

FrameBufferPtr FrameBuffer::create()
{
    return std::shared_ptr<FrameBuffer>(new FrameBuffer());
}

void FrameBuffer::bindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void FrameBuffer::attach(FrameBufferAttachment attachment, const TexturePtr texture)
{
    int previous = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previous);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, toGLAttachment(attachment), GL_TEXTURE_2D, texture ? texture->id() : 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, previous);
}

void FrameBuffer::attach(FrameBufferAttachment attachment, const RenderBufferPtr renderBuffer)
{
    int previous = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previous);

    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, toGLAttachment(attachment), GL_RENDERBUFFER, renderBuffer ? renderBuffer->id() : 0);
    glBindFramebuffer(GL_FRAMEBUFFER, previous);
}

bool FrameBuffer::isComplete() const
{
    return status() == "complete";
}

void FrameBuffer::assertComplete() const
{
    const std::string statusMsg = status();
    if (statusMsg != "complete")
    {
        throw IllegalStateException("Frame buffer is incomplete: " + statusMsg);
    }
}

std::string FrameBuffer::status() const
{
    int previous = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previous);

    bind();
    unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, previous);

    switch (status)
    {
        case GL_FRAMEBUFFER_COMPLETE: return "complete";
        case GL_FRAMEBUFFER_UNDEFINED: return "default framebuffer is undefined";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "an attachment is incomplete";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "no images are attached";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "a draw buffer has no attachment";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "the read buffer has no attachment";
		case GL_FRAMEBUFFER_UNSUPPORTED: return "the attachment format combination is unsupported";
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "attachment sample counts do not match";
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "layered attachments are inconsistent";
		default: return "unknown frame buffer status";
    }
}