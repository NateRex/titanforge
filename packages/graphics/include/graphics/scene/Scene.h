#pragma once
#include <graphics/scene/pointers/ScenePtr.h>
#include <graphics/entities/EntityGroup.h>

/**
 * The scene is the top-level entity group containing everything to be rendered
 * @author Nathaniel Rex
 */
class Scene : public EntityGroup {
public:

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