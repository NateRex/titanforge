#include <graphics/objects/PostProcessing.h>
#include <graphics/core/renderer/RenderState.h>
#include <common/exceptions/IllegalArgumentException.h>

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

void PostProcessing::traverse(RenderState& state, const RenderPass& pass, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	state.postProcessing.push_back(material.get());
	Entity::traverse(state, pass, parentModel, parentNormal);
}
