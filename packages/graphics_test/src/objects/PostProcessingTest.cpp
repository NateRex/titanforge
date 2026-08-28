#include <boost/test/unit_test.hpp>
#include <graphics/objects/PostProcessing.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/core/renderer/DrawState.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/UnsupportedOperationException.h>

/**
 * Tests creation of a post-processing object
 */
BOOST_AUTO_TEST_CASE(PostProcessing_create)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	PostProcessingPtr postProcessing = PostProcessing::create(material);
	BOOST_TEST(postProcessing->material == material);
}

/**
 * Tests rejection of a null material
 */
BOOST_AUTO_TEST_CASE(PostProcessing_rejectsNullMaterial)
{
	BOOST_CHECK_THROW(PostProcessing::create(nullptr), IllegalArgumentException);
}

/**
 * Tests that post-processing is added to the state during scene traversal
 */
BOOST_AUTO_TEST_CASE(PostProcessing_traversal)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	PostProcessingPtr postProcessing = PostProcessing::create(material);
	
	DrawState state;
	postProcessing->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
	BOOST_TEST(state.postProcessing.size() == 1);
	BOOST_TEST(state.postProcessing[0] == material.get());
}

/**
 * Tests that an exception occurs when trying to position, rotate, or scale a post-processing effect
 */
BOOST_AUTO_TEST_CASE(PostProcessing_transformExceptions)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	PostProcessingPtr postProcessing = PostProcessing::create(material);

	BOOST_REQUIRE_THROW(postProcessing->setPosition(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(postProcessing->setRotation(Matrix3::IDENTITY), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(postProcessing->setScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
}
