#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/modes/MaterialMode.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>

/**
 * Tests item property updates when in material rendering mode
 */
BOOST_AUTO_TEST_CASE(MaterialRenderMode_apply)
{
	const BoxGeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
	const MeshMaterialPtr material = MeshMaterial::create();
	RenderItem item = { geometry.get(), material.get() };

	RenderMode::MATERIAL->apply(item);
	BOOST_TEST(item.visible);
	BOOST_CHECK(item.shader == ShaderId::MESH);
	BOOST_CHECK(item.layer == RenderLayer::OPAQUE);

	material->color = material->color.withAlpha(0.5f);
	RenderMode::MATERIAL->apply(item);
	BOOST_CHECK(item.layer == RenderLayer::TRANSPARENT);
}
