#pragma once
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/objects/pointers/MeshPtr.h>
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
	 * Local-to-world transformation, accounting for all parent entities of the mesh.
	 */
	Matrix4 local2World = Matrix4::IDENTITY;
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
	 * The items to be drawn this frame
	 */
	std::vector<RenderItem> items;
};