#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

RenderTarget::RenderTarget(const RenderTargetConfig& descriptor): _descriptor(descriptor)
{
    if (_descriptor.width <= 0 || _descriptor.height <= 0)
    {
        throw IllegalArgumentException("Render target dimensions must be greater than zero");
    }
    if (_descriptor.colorFormats.size() > 4)
    {
        throw IllegalArgumentException("Render targets support at most four color attachments");
    }

    build();
}

const std::vector<TexturePtr>& RenderTarget::colorTextures() const
{
    return _colorTextures;
}

void RenderTarget::resize(unsigned int width, unsigned int height)
{
    if (width <= 0 || height <= 0)
    {
        throw IllegalArgumentException("Render target dimensions must be greater than zero");
    }
    if (width == _descriptor.width && height == _descriptor.height)
    {
        return;
    }

    _descriptor.width = width;
    _descriptor.height = height;

    for (TexturePtr texture : _colorTextures)
    {
        texture->resize(width, height);
    }

    if (_depthStencilTexture)
    {
        _depthStencilTexture->resize(width, height);
    }

    if (_depthStencilRenderBuffer)
    {
        _depthStencilRenderBuffer->resize(width, height);
    }

    _frameBuffer->assertComplete();
}

TexturePtr RenderTarget::colorTexture(unsigned int index) const
{
    if (index >= _colorTextures.size())
    {
        throw IllegalArgumentException("Color attachment index is out of bounds");
    }
    return _colorTextures[index];
}

void RenderTarget::build()
{
    _frameBuffer = FrameBuffer::create();

    // Set up color attachments
    std::vector<unsigned int> drawBuffers;
    for (int i = 0; i < _descriptor.colorFormats.size(); i++)
    {
        if (isDepthFormat(_descriptor.colorFormats[i]))
        {
            throw IllegalArgumentException("A color attachment must use a color format");
        }

        TextureConfig texConfig;
        texConfig.width = _descriptor.width;
        texConfig.height = _descriptor.height;
        texConfig.format = _descriptor.colorFormats[i];
        texConfig.sampler.sWrap = TextureWrap::CLAMP_TO_EDGE;
        texConfig.sampler.tWrap = TextureWrap::CLAMP_TO_EDGE;

        TexturePtr texture = Texture::create(texConfig);
        _colorTextures.push_back(texture);
        _frameBuffer->attach(static_cast<FrameBufferAttachment>(static_cast<int>(FrameBufferAttachment::COLOR0) + i), texture);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    // Select draw buffers
    _frameBuffer->bind();
    if (drawBuffers.empty())
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else {
        glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    }
    FrameBuffer::bindDefault();

    // Set up depth-stencil attachments
    if (_descriptor.depthStencilStorage != DepthStencilStorage::NONE)
    {
        if (!isDepthFormat(_descriptor.depthStencilFormat))
        {
            throw IllegalArgumentException("Depth/stencil attachment must use a depth format");
        }

        const FrameBufferAttachment attachment = (_descriptor.depthStencilFormat == PixelFormat::DEPTH24_STENCIL8)
            ? FrameBufferAttachment::DEPTH_STENCIL : FrameBufferAttachment::DEPTH;

        if (_descriptor.depthStencilStorage == DepthStencilStorage::TEXTURE)
        {
            _depthStencilRenderBuffer = nullptr;

            TextureConfig texConfig;
            texConfig.width = _descriptor.width;
            texConfig.height = _descriptor.height;
            texConfig.format = _descriptor.depthStencilFormat;
            texConfig.sampler.sWrap = TextureWrap::CLAMP_TO_EDGE;
            texConfig.sampler.tWrap = TextureWrap::CLAMP_TO_EDGE;
            texConfig.sampler.minFilter = TextureFilter::NEAREST;
            texConfig.sampler.magFilter = TextureFilter::NEAREST;

            _depthStencilTexture = Texture::create(texConfig);
            _frameBuffer->attach(attachment, _depthStencilTexture);
        }
        else
        {
            _depthStencilTexture = nullptr;

            RenderBufferConfig rbConfig;
            rbConfig.width = _descriptor.width;
            rbConfig.height = _descriptor.height;
            rbConfig.format = _descriptor.depthStencilFormat;

            _depthStencilRenderBuffer = RenderBuffer::create(rbConfig);
            _frameBuffer->attach(attachment, _depthStencilRenderBuffer);
        }
    }

    _frameBuffer->assertComplete();
}
