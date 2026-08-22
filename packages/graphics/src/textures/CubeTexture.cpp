#include <graphics/textures/CubeTexture.h>
#include <graphics/loaders/ImageLoader.h>
#include <common/exceptions/InstantiationException.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

CubeTexture::CubeTexture(const std::array<std::string, 6>& paths, bool flip)
{
    int width, height, channels;
    std::array<void*, 6> imageData{};

    // Create helper lambda for releasing images once finished with it
    auto releaseImages = [&imageData]() {
        for (void* data : imageData)
        {
            stbi_image_free(data);
        }
    };

    stbi_set_flip_vertically_on_load(flip);
    for (int i = 0; i < paths.size(); i++)
    {
        // Load image
        const std::string& path = paths[i];
        int faceWidth, faceHeight, faceChannels;
        unsigned char* data = stbi_load(resolvePath(path).c_str(), &faceWidth, &faceHeight, &faceChannels, 0);
        if (!data)
        {
            releaseImages();
            throw InstantiationException("Failed to load texture image: " + path);
        }

        // Assert width and height are equal
        imageData[i] = data;
        if (faceWidth != faceHeight)
        {
            releaseImages();
            throw InstantiationException("Cube texture faces must be square: " + path);
        }

        // Assert size and channels match previous imagery
        if (i == 0)
        {
            width = faceWidth;
            height = faceHeight;
            channels = faceChannels;
        }
        else if (faceWidth != width || faceHeight != height || faceChannels != channels)
        {
            releaseImages();
            throw InstantiationException("Cube texture faces must have matching dimensions and channel counts: " + path);
        }
    }

    _config.size = width;
    _config.generateMipmaps = false;
    switch (channels)
    {
        case 1: _config.format = PixelFormat::R8; break;
		case 2: _config.format = PixelFormat::RG8; break;
		case 3: _config.format = PixelFormat::RGB8; break;
		case 4: _config.format = PixelFormat::RGBA8; break;
		default: releaseImages(); throw InstantiationException("Unsupported texture channel count: " + std::to_string(channels));
    }

    glGenTextures(1, &_id);

    allocate(imageData);
    releaseImages();
}

CubeTexture::CubeTexture(const CubeTextureConfig& config, const std::array<void*, 6>& data): _config(config)
{
    if (_config.size <= 0)
    {
        throw IllegalArgumentException("Cube texture size must be greater than zero");
    }

    glGenTextures(1, &_id);
    allocate(data);
}

CubeTexture::~CubeTexture()
{
    glDeleteTextures(1, &_id);
    _id = 0;
}

CubeTexturePtr CubeTexture::create(const std::array<std::string, 6>& paths, bool flip)
{
    return std::shared_ptr<CubeTexture>(new CubeTexture(paths, flip));
}

CubeTexturePtr CubeTexture::create(const CubeTextureConfig& config, const std::array<void*, 6>& data)
{
    return std::shared_ptr<CubeTexture>(new CubeTexture(config, data));
}

void CubeTexture::allocate(const std::array<void*, 6>& data)
{
    const OpenGLPixelFormat format = toGLFormat(_config.format);

    int previousTexture = 0;
    int previousAlignment = 4;

    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &previousTexture);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

    applySampling();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned int i = 0; i < data.size(); i++)
    {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            format.internalFormat,
            _config.size,
            _config.size,
            0,
            format.format,
            format.type,
            data[i]);
    }

    if (_config.generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);
    glBindTexture(GL_TEXTURE_CUBE_MAP, previousTexture);
}

void CubeTexture::applySampling() const
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, toGLFilter(_config.sampling.minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, toGLFilter(_config.sampling.magFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, toGLWrap(_config.sampling.sWrap));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, toGLWrap(_config.sampling.tWrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, toGLWrap(_config.sampling.rWrap));
	glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, _config.sampling.borderColor);
}

void CubeTexture::resize(unsigned int size)
{
    if (size <= 0)
    {
        throw IllegalArgumentException("Cube texture size must be greater than zero");
    }

    _config.size = size;
    allocate({});
}
