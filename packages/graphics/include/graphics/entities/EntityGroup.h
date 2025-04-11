#pragma once
#include <graphics/entities/pointers/EntityGroupPtr.h>
#include <graphics/entities/Entity.h>

/**
 * An EntityGroup groups one or more entities, with the ability to perform an additional coordinate transformation.
 * @author Nathaniel Rex
 */
class EntityGroup : public Entity {
public:

	/**
	 * Constructs a new entity group instance
	 * @return The new entity group instance
	 */
	static EntityGroupPtr create();

private:

	/**
	 * Constructor
	 */
	EntityGroup();
};