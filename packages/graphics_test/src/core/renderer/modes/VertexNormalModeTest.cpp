#include <boost/test/unit_test.hpp>
#include <graphics/core/renderer/modes/VertexNormalMode.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/geometry/PointGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PointMaterial.h>
#include <math/Vector3.h>

/**
 * Tests item property updates when in vertex normal rendering mode
 */
BOOST_AUTO_TEST_CASE(VertexNormalRenderMode_apply)
{
    // Test on mesh, containing vertex normals
	const BoxGeometryPtr meshGeometry = BoxGeometry::create(1.f, 1.f, 1.f);
	const MeshMaterialPtr meshMaterial = MeshMaterial::create();
	RenderItem meshItem = { meshGeometry.get(), meshMaterial.get() };

	RenderMode::VERTEX_NORMALS->apply(meshItem);
	BOOST_TEST(meshItem.visible);
	BOOST_CHECK(meshItem.shader == ShaderId::VERTEX_NORMALS);
	BOOST_CHECK(meshItem.layer == RenderLayer::OPAQUE);

    // Test on point, which does not contain any vertex normals
	const PointGeometryPtr pointGeometry = PointGeometry::create(Vector3::ZERO);
	const PointMaterialPtr pointMaterial = PointMaterial::create();
	RenderItem pointItem = { pointGeometry.get(), pointMaterial.get() };

	RenderMode::VERTEX_NORMALS->apply(pointItem);
    BOOST_TEST(!pointItem.visible);
}
