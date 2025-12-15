#pragma once
#include <graphics/scene/pointers/ScenePtr.h>
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/core/EntityGroup.h>

/**
 * The scene is the top-level entity group containing everything to be rendered
 * @author Nathaniel Rex
 */
class Scene : public EntityGroup {
public:

	/**
	 * Ambient lighting
	 */
	LightPtr ambientLighting;

	/**
	 * Constructs a new scene instance
	 * @return The new scene instance
	 */
	static ScenePtr create();

private:

	/**
	 * Constructor
	 */
	Scene();
};