#include <boost/test/unit_test.hpp>
#include <graphics/objects/PostProcessing.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/core/renderer/RenderPass.h>
#include <graphics/core/renderer/RenderState.h>
#include <common/exceptions/IllegalArgumentException.h>

BOOST_AUTO_TEST_CASE(PostProcessing_create)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	PostProcessingPtr postProcessing = PostProcessing::create(material);
	BOOST_TEST(postProcessing->material == material);
}

BOOST_AUTO_TEST_CASE(PostProcessing_rejectsNullMaterial)
{
	BOOST_CHECK_THROW(PostProcessing::create(nullptr), IllegalArgumentException);
}

BOOST_AUTO_TEST_CASE(PostProcessing_materialModeTraversal)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	PostProcessingPtr postProcessing = PostProcessing::create(material);
	
	RenderPass pass;
	pass.mode = RenderMode::MATERIAL;
	RenderState state;
	postProcessing->traverse(state, pass, Matrix4::IDENTITY, Matrix3::IDENTITY);

	BOOST_REQUIRE_EQUAL(state.postProcessing.size(), 1);
	BOOST_TEST(state.postProcessing[0] == material.get());
}
