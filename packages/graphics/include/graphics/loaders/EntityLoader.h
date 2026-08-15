#pragma once

#include <graphics/core/pointers/EntityGroupPtr.h>
#include <string>

/**
 * Loads model files into renderable TitanForge entities.
 * @author Nathaniel Rex
 */
class EntityLoader
{
public:

	/**
	 * Loads a model and converts its node hierarchy into TitanForge entities.
	 * @param path Path to the model, relative to the running executable
	 * @return An entity group corresponding to the imported scene's root node
	 * @throws InstantiationException on failure to import the entity
	 */
	static EntityGroupPtr load(const std::string& path);
};
