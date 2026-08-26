#include <graphics/objects/Skybox.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/textures/TextureCube.h>
#include <graphics/loaders/ImageLoader.h>
#include <common/Utils.h>
#include <common/exceptions/InstantiationException.h>
#include <common/exceptions/UnsupportedOperationException.h>
#include "DefaultSkyboxData.h"

constexpr std::array<EmbeddedImage, 6> DEFAULT_SKYBOX_IMAGES {{
    { DefaultSkyboxData::right, DefaultSkyboxData::rightSize, "right" },
    { DefaultSkyboxData::left, DefaultSkyboxData::leftSize, "left" },
    { DefaultSkyboxData::top, DefaultSkyboxData::topSize, "top" },
    { DefaultSkyboxData::bottom, DefaultSkyboxData::bottomSize, "bottom" },
    { DefaultSkyboxData::front, DefaultSkyboxData::frontSize, "front" },
    { DefaultSkyboxData::back, DefaultSkyboxData::backSize, "back" }
}};

Skybox::Skybox(SkyboxMaterialPtr material): Mesh(BoxGeometry::create(2.f, 2.f, 2.f), material)
{

}

SkyboxPtr Skybox::create(SkyboxMaterialPtr material)
{
    return std::shared_ptr<Skybox>(new Skybox(material));
}

SkyboxPtr Skybox::create()
{
    SkyboxMaterialPtr material = SkyboxMaterial::create();
    material->texture = createDefaultTexture();
    return create(material);
}

void Skybox::traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	const SkyboxMaterialPtr skybox = std::static_pointer_cast<SkyboxMaterial>(material);
	if (skybox->texture)
	{
		state.environment.texture = std::static_pointer_cast<TextureCube>(skybox->texture).get();
		state.environment.color = skybox->color;
		state.environment.intensity = skybox->intensity;
		state.environment.rotation = skybox->rotation;
		state.environment.lod = skybox->lod;
	}

	Mesh::traverse(state, parentModel, parentNormal);
}

void Skybox::updateScaling(float x, float y, float z)
{
    throw UnsupportedOperationException("Scaling updates are not supported for skyboxes");
}

void Skybox::updatePosition(float x, float y, float z)
{
    throw UnsupportedOperationException("Position updates are not supported for skyboxes");
}

void Skybox::updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    throw UnsupportedOperationException("Rotation updates are not supported for skyboxes");
}

TextureCubePtr Skybox::createDefaultTexture()
{
    std::array<void*, 6> pixels{};
    auto release = [&pixels]() {
        for (void* face : pixels) stbi_image_free(face);
    };

    int size = 0;
    try
    {
        stbi_set_flip_vertically_on_load(false);
        for (std::size_t i = 0; i < DEFAULT_SKYBOX_IMAGES.size(); i++)
        {
            const EmbeddedImage& image = DEFAULT_SKYBOX_IMAGES[i];

            int width = 0, height = 0, sourceChannels = 0;
            pixels[i] = stbi_load_from_memory(
                image.data,
                static_cast<int>(image.size),
                &width,
                &height,
                &sourceChannels,
                3);

            if (!pixels[i]) throw InstantiationException("Failed to decode embedded default skybox face: " + std::string(image.name));
            if (width != height) throw InstantiationException("Embedded default skybox face is not square: " + std::string(image.name));
            
            if (i == 0)
            {
                size = width;
            }
            else if (width != size)
            {
                throw InstantiationException("Embedded default skybox faces have mismatched dimensions");
            }
        }

        // TitanForge does not currently run a fully-managed linear/sRGB render pipeline, so we use RGB8
        // instead of sRGB. Otherwise, it would decode darker than the same JPEGs loaded from disk.
        TextureCubeConfig config;
        config.size = static_cast<unsigned int>(size);
        config.format = PixelFormat::RGB8;

        TextureCubePtr texture = TextureCube::create(config, pixels);
        release();
        return texture;
    }
    catch (...)
    {
        release();
        throw;
    }
}
