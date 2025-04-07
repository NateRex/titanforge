#pragma once
#include <graphics/materials/Material.h>
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/materials/pointers/BasicMaterialPtr.h>

/**
 * Material for drawing geometries in a flat or wireframe way.
 * @author Nathaniel Rex
 */
class BasicMaterial : public Material
{
public:

	/**
	 * Flag that, when true, forces the use of colors specified on geometry vertices (when available). Defaults
	 * to false.
	 */
	bool useVertexColors = false;

	/**
	 * Creates a new BasicMaterial instance
	 */
	static BasicMaterialPtr create();

private:

	/**
	 * Constructor
	 */
	BasicMaterial();
};