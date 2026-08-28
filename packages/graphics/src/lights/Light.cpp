#include <graphics/lights/Light.h>
#include <graphics/core/renderer/DrawState.h>
#include <common/exceptions/UnsupportedOperationException.h>

void Light::updateScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling not supported for lights");
}

void Light::traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	LightInstance light;
	light.light = this;
	light.position = parentModel.transformPosition(getPosition());
	light.direction = parentModel.transformDirection(getForwardVector()).normalize();
	state.lights.push_back(light);

	Entity::traverse(state, parentModel, parentNormal);
}
