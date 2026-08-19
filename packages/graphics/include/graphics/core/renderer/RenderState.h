#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/objects/pointers/MeshPtr.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <vector>

/**
 * A flattened, render-ready description of a single mesh instance obtained during scene traversal
 * @author Nathaniel Rex
 */
struct RenderItem {

	/**
	 * Mesh object
	 */
	MeshPtr mesh = nullptr;

	/**
	 * Local-to-world transformation for vertices, accounting for all parent entities of the mesh.
	 */
	Matrix4 modelTransform = Matrix4::IDENTITY;

	/**
	 * Local-to-world transformation for vertex normals, accounting for all parent entities of the mesh.
	 */
	Matrix3 normalTransform = Matrix3::IDENTITY;
};

/**
 * A flattened, render-ready description of a light instance.
 */
struct RenderLight {

	/**
	 * Light providing this instance's color and attenuation properties.
	 */
	LightPtr light = nullptr;

	/**
	 * Position in world space.
	 */
	Vector3 position = Vector3::ZERO;

	/**
	 * Direction in which the light's rays travel, in world space.
	 */
	Vector3 direction = Vector3::ZERO;
};


/**
 * Aggregation of all the lights in the scene for a single render pass
 * @author Nathaniel Rex
 */
struct Lighting {

	/**
	 * Lights affecting this render pass. The shader consumes up to its supported
	 * maximum; keeping the full list here allows selection policies to be added
	 * without constraining the scene itself.
	 */
	std::vector<RenderLight> lights;
};


/**
 * Aggregation of all rendering data required for a single render pass
 * @author Nathaniel rex
 */
struct RenderState {

	/**
	 * Camera
	 */
	CameraPtr camera = nullptr;

	/**
	 * Lighting for the render pass
	 */
	Lighting lighting;

	/**
	 * The items to be drawn this frame
	 */
	std::vector<RenderItem> items;
};
