#pragma once

#include <graphics/core/pointers/EntityGroupPtr.h>
#include <string>

/**
 * Loads model files into renderable meshes.
 * @author Nathaniel Rex
 */
class ModelLoader
{
	
public:

	/**
	 * Loads a model file while preserving its node hierarchy and local transforms.
	 * Assimp nodes become EntityGroups, and each mesh referenced by a node becomes
	 * a child Mesh with a BasicMaterial.
	 * @param path Path to a model file supported by Assimp
	 * @return The root entity group of the imported model
	 * @throws InstantiationException if the model cannot be imported
	 */
	static EntityGroupPtr load(const std::string& path);

};
