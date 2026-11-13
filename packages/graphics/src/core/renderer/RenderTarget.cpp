#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/buffers/RenderBuffer.h>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/TextureCube.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

RenderTarget::RenderTarget(const RenderTargetConfig& config): _config(config)
{
    if (_config.samples == 0)
    {
        throw IllegalArgumentException("Render target sample count must be greater than zero");
    }
    if (usesCubeStorage() && _config.samples != 1)
    {
        throw IllegalArgumentException("Cube texture storage does not support multisampling");
    }
    if (_config.colorFormats.size() > 4)
    {
        throw IllegalArgumentException("Render targets support at most four color attachments");
    }
    if (_config.sizeMode == TargetSizeMode::FIXED)
    {
        build();
    }
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

    if (usesCubeStorage() && width != height)
    {
        throw IllegalArgumentException("Cube texture storage requires square dimensions");
    }

    if (!_frameBuffer)
    {
        _config.width = width;
        _config.height = height;
        build();
    }

    if (width == _config.width && height == _config.height)
    {
        return;
    }

    _config.width = width;
    _config.height = height;

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
    if (_config.width <= 0 || _config.height <= 0)
    {
        throw IllegalArgumentException("Render target dimensions must be greater than zero");
    }

    if (usesCubeStorage() && _config.width != _config.height)
    {
        throw IllegalArgumentException("Cube texture storage requires square dimensions");
    }

    _frameBuffer = FrameBuffer::create();

    // Set up color attachments
    std::vector<unsigned int> drawBuffers;
    for (int i = 0; i < _config.colorFormats.size(); i++)
    {
        if (isDepthFormat(_config.colorFormats[i]))
        {
            throw IllegalArgumentException("A color attachment must use a color format");
        }

        TextureSampling sampling;
        sampling.sWrap = sampling.tWrap = sampling.rWrap = TextureWrap::CLAMP_TO_EDGE;
        TexturePtr texture = createTexture(_config.colorFormats[i], sampling,
            _config.colorStorage == ColorStorage::TEXTURE_CUBE);
        _colorTextures.push_back(texture);
        _frameBuffer->attach(static_cast<FrameBufferAttachment>(static_cast<int>(FrameBufferAttachment::COLOR0) + i), texture);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    // Upload color data to framebuffer
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
    if (_config.depthStencilStorage != DepthStencilStorage::NONE)
    {
        if (!isDepthFormat(_config.depthStencilFormat))
        {
            throw IllegalArgumentException("Depth/stencil attachment must use a depth format");
        }

        const FrameBufferAttachment attachment = (_config.depthStencilFormat == PixelFormat::DEPTH24_STENCIL8)
            ? FrameBufferAttachment::DEPTH_STENCIL : FrameBufferAttachment::DEPTH;

        if (_config.depthStencilStorage == DepthStencilStorage::TEXTURE_2D
            || _config.depthStencilStorage == DepthStencilStorage::TEXTURE_CUBE)
        {
            _depthStencilRenderBuffer = nullptr;

            _depthStencilTexture = createTexture(_config.depthStencilFormat, _config.depthSampling,
                _config.depthStencilStorage == DepthStencilStorage::TEXTURE_CUBE);
            _frameBuffer->attach(attachment, _depthStencilTexture);
        }
        else
        {
            _depthStencilTexture = nullptr;

            RenderBufferConfig rbConfig;
            rbConfig.width = _config.width;
            rbConfig.height = _config.height;
            rbConfig.format = _config.depthStencilFormat;
            rbConfig.samples = _config.samples;

            _depthStencilRenderBuffer = RenderBuffer::create(rbConfig);
            _frameBuffer->attach(attachment, _depthStencilRenderBuffer);
        }
    }

    _frameBuffer->assertComplete();
}


bool RenderTarget::usesCubeStorage() const
{
    return _config.depthStencilStorage == DepthStencilStorage::TEXTURE_CUBE
        || (!_config.colorFormats.empty() && _config.colorStorage == ColorStorage::TEXTURE_CUBE);
}

TexturePtr RenderTarget::createTexture(PixelFormat format, const TextureSampling& sampling, bool cube) const
{
    if (cube)
    {
        TextureCubeConfig config;
        config.size = _config.width;
        config.format = format;
        config.sampling = sampling;
        return TextureCube::create(config);
    }
    else
    {
        Texture2DConfig config;
        config.width = _config.width;
        config.height = _config.height;
        config.format = format;
        config.samples = _config.samples;
        config.sampling = sampling;
        return Texture2D::create(config);
    }
}
