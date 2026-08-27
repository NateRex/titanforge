#include <graphics/lights/Light.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/core/renderer/RenderPass.h>
#include <common/exceptions/UnsupportedOperationException.h>

void Light::updateScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling not supported for lights");
}

void Light::traverse(RenderState& state, const RenderPass& pass, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	RenderLight renderLight;
	renderLight.light = this;
	renderLight.position = parentModel.transformPosition(getPosition());
	renderLight.direction = parentModel.transformDirection(getForwardVector()).normalize();
	state.lighting.lights.push_back(renderLight);

	Entity::traverse(state, pass, parentModel, parentNormal);
}
