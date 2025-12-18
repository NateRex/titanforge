#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/lights/pointers/AmbientLightPtr.h>
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/objects/pointers/MeshPtr.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
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
 * Aggregation of all the lights in the scene for a single render pass
 * @author Nathaniel Rex
 */
struct Lighting {

	/**
	 * Ambient lighting
	 */
	AmbientLightPtr ambient = nullptr;

	/**
	 * Positional lighting
	 */
	LightPtr positional = nullptr;
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
	 * Ambient lighting
	 */
	Lighting lighting;

	/**
	 * The items to be drawn this frame
	 */
	std::vector<RenderItem> items;
};