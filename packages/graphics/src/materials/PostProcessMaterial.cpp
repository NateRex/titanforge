#include <graphics/materials/PostProcessMaterial.h>

PostProcessMaterial::PostProcessMaterial(): Material(MaterialType::POST_PROCESS)
{
}

PostProcessMaterialPtr PostProcessMaterial::create()
{
	return std::shared_ptr<PostProcessMaterial>(new PostProcessMaterial());
}
