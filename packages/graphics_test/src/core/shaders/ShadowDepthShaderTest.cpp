#include <boost/test/unit_test.hpp>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/ShadowDepthShader.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/loaders/TextureLoader.h>
#include <math/Matrix4.h>
#include <memory>

/**
 * Tests the ability to set a light projection matrix.
 */
BOOST_AUTO_TEST_CASE(ShadowDepthShader_setProjection)
{
    Matrix4 projection = Matrix4::fromOrthographic(20.f, 10.f, 0.1f, 100.f);
    auto shader = std::static_pointer_cast<ShadowDepthShader>(ShaderManager::getShader(ShaderId::SHADOW_DEPTH));
    BOOST_REQUIRE_NO_THROW(shader->setProjection(projection));
}

/**
 * Tests the ability to set a render item consisting of a basic colored material.
 */
BOOST_AUTO_TEST_CASE(ShadowDepthShader_setColorMaterial)
{
    MeshMaterialPtr material = MeshMaterial::create();
    GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);

    DrawItem item;
    item.material = material.get();
    item.geometryBuffer = geometry->getBuffer();
    item.modelTransform = Matrix4::fromTranslation(Vector3(1.f, 2.f, -3.f));

    ShaderPtr shader = ShaderManager::getShader(ShaderId::SHADOW_DEPTH);
    BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a masked render item containing a texture.
 */
BOOST_AUTO_TEST_CASE(ShadowDepthShader_setTextureMaterial)
{
    MeshMaterialPtr material = MeshMaterial::create();
    material->texture = TextureLoader::load("assets/container.jpg");
    material->alphaMode = AlphaMode::MASK;
    material->alphaCutoff = 0.25f;
    material->color = Color::WHITE.withAlpha(0.75f);
    GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);

    DrawItem item;
    item.material = material.get();
    item.geometryBuffer = geometry->getBuffer();

    ShaderPtr shader = ShaderManager::getShader(ShaderId::SHADOW_DEPTH);
    BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a masked render item containing vertex colors.
 */
BOOST_AUTO_TEST_CASE(ShadowDepthShader_setVertexColorMaterial)
{
    MeshMaterialPtr material = MeshMaterial::create();
    material->alphaMode = AlphaMode::MASK;
    material->useVertexColors = true;
    GeometryPtr geometry = Geometry::create(PrimitiveType::TRIANGLES);
    const float vertices[] = {-1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f};
    const float colors[] = {1.f, 0.f, 0.f, 0.25f, 0.f, 1.f, 0.f, 0.5f, 0.f, 0.f, 1.f, 1.f};
    geometry->setVertices(vertices, 3);
    geometry->setColors(colors, 3);

    DrawItem item;
    item.material = material.get();
    item.geometryBuffer = geometry->getBuffer();

    ShaderPtr shader = ShaderManager::getShader(ShaderId::SHADOW_DEPTH);
    BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}

/**
 * Tests the ability to set a render item containing instanced geometry.
 */
BOOST_AUTO_TEST_CASE(ShadowDepthShader_setInstancedItem)
{
    MeshMaterialPtr material = MeshMaterial::create();
    GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
    std::unique_ptr<InstanceBuffer> buffer(geometry->createInstanceBuffer());
    buffer->setTransforms({Matrix4::IDENTITY, Matrix4::fromTranslation(Vector3(1.f, 0.f, 0.f))});

    DrawItem item;
    item.material = material.get();
    item.instanceBuffer = buffer.get();
    item.modelTransform = Matrix4::fromTranslation(Vector3(0.f, 0.f, -3.f));

    ShaderPtr shader = ShaderManager::getShader(ShaderId::SHADOW_DEPTH);
    BOOST_REQUIRE_NO_THROW(shader->setItem(item));
}
