#pragma once
#include <graphics/lights/pointers/LightPtr.h>
#include <graphics/core/Entity.h>
#include <graphics/core/Color.h>

/**
 * A source of light that can be added to a scene. Lights take up no physical space, and instead affect the
 * shading of other entities.
 * @author Nathaniel Rex
 */
class Light : public Entity
{
public:

	/**
	 * Light color
	 */
	Color color;

	/**
	 * Constructs a new light instance
	 */
	static LightPtr create();

private:

	/**
	 * Constructor
	 */
	Light() : Entity(EntityType::LIGHT), color(Color::WHITE) {};
};