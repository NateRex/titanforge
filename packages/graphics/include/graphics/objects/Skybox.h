#pragma once
#include <graphics/objects/pointers/SkyboxPtr.h>
#include <graphics/objects/Mesh.h>
#include <graphics/materials/pointers/SkyboxMaterialPtr.h>
#include <graphics/textures/pointers/TextureCubePtr.h>
#include <cstddef>

/**
 * Holds data for an embedded skybox image
 * @author Nathaniel Rex
 */
struct EmbeddedImage
{
	/**
	 * Image data
	 */
    const unsigned char* data;

	/**
	 * Image size in bytes
	 */
    std::size_t size;

	/**
	 * Image name
	 */
    const char* name;
};

/**
 * A skybox is a special type of mesh consisting of a box geometry and a cube texture. When added to a scene, it will be rendered
 * as the background for the entire scene.
 * @author Nathaniel Rex
 */
class Skybox : public Mesh
{
public:

	/**
	 * Constructs a cube texture consisting of the default skybox imagery
	 * @return The cube texture consisting of the default skybox imagery
	 */
	static TextureCubePtr createDefaultTexture();

    /**
	 * Constructs a new skybox instance using the default TitanForge imagery
	 * @return The skybox instance
	 */
	static SkyboxPtr create();

    /**
     * Constructs a new skybox instance using a material containing custom imagery
     * @param material Skybox material
     * @return The skybox instance
     */
    static SkyboxPtr create(SkyboxMaterialPtr material);

    /**
	 * @copydoc Entity::updateScaling(float, float, float)
	 * @throws UnsupportedOperationException When called, as scaling of a skybox is not supported.
	 */
	void updateScaling(float x, float y, float z) override;

    /**
	 * @copydoc Entity::updatePosition(float, float, float)
	 * @throws UnsupportedOperationException When called, as changing position of a skybox is not supported.
	 */
	void updatePosition(float x, float y, float z) override;

    /**
	 * @copydoc Entity::updateRotation(float, float, float, float, float, float, float, float, float)
	 * @throws UnsupportedOperationException When called, as changing rotation of a skybox is not supported.
	 */
	void updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) override;

private:

    /**
	 * Constructor
	 * @param material Skybox material
	 */
	Skybox(SkyboxMaterialPtr material);
};