#include <graphics/objects/PostProcessing.h>
#include <graphics/core/renderer/DrawState.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/UnsupportedOperationException.h>

PostProcessing::PostProcessing(PostProcessMaterialPtr material): material(material)
{
}

PostProcessingPtr PostProcessing::create(PostProcessMaterialPtr material)
{
	if (!material)
	{
		throw IllegalArgumentException("Post-processing material cannot be null");
	}
	return std::shared_ptr<PostProcessing>(new PostProcessing(material));
}

void PostProcessing::traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	state.postProcessing.push_back(material.get());
	Entity::traverse(state, parentModel, parentNormal);
}

void PostProcessing::updatePosition(float x, float y, float z)
{
	throw UnsupportedOperationException("Position updates are not supported for post-processing effects");
}

void PostProcessing::updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	throw UnsupportedOperationException("Rotation updates are not supported for post-processing effects");
}

void PostProcessing::updateScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling updates are not supported for post-processing effects");
}