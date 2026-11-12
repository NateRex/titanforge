#include <graphics/core/renderer/ShadowPass.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/renderer/ScopedRenderState.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/ShadowDepthShader.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/lights/SpotLight.h>
#include <common/Utils.h>
#include <glad/glad.h>
#include <algorithm>
#include <cmath>
#include <iostream>

void ShadowPass::clear()
{
    _targets.clear();
}

void ShadowPass::render(DrawState& state, unsigned int budget)
{
    ScopedRenderState restore;

    const ShadowHardwareLimits limits = queryHardwareLimits();
    budget = std::min({budget, limits.shadowSlots, MAX_SHADOW_LIGHTS});

    unsigned int selected = 0, count = 0, maps2D = 0, mapsCube = 0;
    for (LightInstance& instance : state.lights)
    {
        instance.shadowMap.reset();
        instance.shadowIndex = -1;
        instance.shadowTransform = Matrix4::IDENTITY;
        if (!instance.light || instance.light->lightType == LightType::AMBIENT)
        {
            // Ambient lights do not cast shadows
            continue;
        }

        if (selected++ >= MAX_LIGHTS)
        {
            // Only support a fixed max number of lights
            continue;
        }

        const Light& light = *instance.light;
        const bool point = light.lightType == LightType::POINT;
        if (!light.shadow.enabled) continue;
        const unsigned int slot = point ? MAX_SHADOW_MAPS_2D + mapsCube : maps2D;
        if ((point ? mapsCube >= MAX_SHADOW_MAPS_CUBE : maps2D >= MAX_SHADOW_MAPS_2D)
            || slot >= limits.shadowSlots) continue;
        
        Matrix4 transforms[6];
        const int maxSize = point ? limits.maxCubeSize : limits.max2DSize;
        if (count >= budget || !buildTransforms(instance, maxSize, transforms))
        {
            continue;
        }

        try
        {
            RenderTargetPtr target = prepareTarget(count, light);
            renderDepth(state, instance, transforms, *target);

            if (count == _targets.size()) _targets.push_back(target);
            else _targets[count] = target;
            instance.shadowMap = target->depthStencilTexture();
            instance.shadowTransform = point ? Matrix4::IDENTITY : transforms[0];
            instance.shadowIndex = point ? mapsCube++ : maps2D++;
            ++count;
        }
        catch (const std::exception& error)
        {
            std::cerr << std::string("Shadow generation failed for light; illuminating without shadows: ") + error.what() << std::endl;
        }
    }
    
    _targets.resize(count);
}

ShadowHardwareLimits ShadowPass::queryHardwareLimits()
{
    GLint textureUnits, combinedUnits, textureSize, cubeSize, maxViewport[2];
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &cubeSize);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewport);
    const auto slots = static_cast<unsigned int>(std::max(0,
        std::min(textureUnits, combinedUnits) - static_cast<int>(FIRST_SHADOW_TEXTURE_UNIT)));
    textureSize = std::min({textureSize, maxViewport[0], maxViewport[1]});
    return {slots, textureSize, std::min(textureSize, cubeSize)};
}

RenderTargetPtr ShadowPass::prepareTarget(unsigned int index, const Light& light) const
{
    const bool point = light.lightType == LightType::POINT;
    const auto storage = point ? DepthStencilStorage::TEXTURE_CUBE : DepthStencilStorage::TEXTURE_2D;
    RenderTargetPtr target = index < _targets.size() ? _targets[index] : nullptr;
    if (!target || target->config().width != light.shadow.width
        || target->config().height != light.shadow.height
        || target->config().depthStencilStorage != storage)
    {
        RenderTargetConfig config;
        config.sizeMode = TargetSizeMode::FIXED;
        config.width = light.shadow.width;
        config.height = light.shadow.height;
        config.colorFormats.clear();
        config.depthStencilStorage = storage;
        config.depthStencilFormat = PixelFormat::DEPTH32F;
        if (!point)
        {
            config.depthSampling.sWrap = config.depthSampling.tWrap = TextureWrap::CLAMP_TO_BORDER;
            std::fill_n(config.depthSampling.borderColor, 4, 1.f);
        }
        target = RenderTarget::create(config);
    }
    return target;
}

void ShadowPass::renderDepth(const DrawState& state, const LightInstance& instance,
    const Matrix4* transforms, const RenderTarget& target)
{
    target.frameBuffer()->bind();
    glViewport(0, 0, target.config().width, target.config().height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto shader = std::static_pointer_cast<ShadowDepthShader>(ShaderManager::getShader(ShaderId::SHADOW_DEPTH));
    shader->activate();
    if (instance.light->lightType == LightType::POINT)
        shader->setProjections(transforms, instance.position, instance.light->shadow.far);
    else shader->setProjection(transforms[0]);
    renderCasters(state, *shader);
}

void ShadowPass::renderCasters(const DrawState& state, ShadowDepthShader& shader)
{
    for (const DrawItem& item : state.items)
    {
        if (!item.material || item.material->materialType != MaterialType::MESH)
        {
            // Shadows only casted by mesh objects
            continue;
        }

        const auto& material = static_cast<const MeshMaterial&>(*item.material);
        if (!material.castShadow || material.getEffectiveAlphaMode() == AlphaMode::BLEND)
        {
            // Transparent or semi-transparent objects do not cast shadows
            continue;
        }

        if (!material.shadowCullFrontFaces && material.cullingMode == CullingMode::NONE)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(material.shadowCullFrontFaces || material.cullingMode == CullingMode::FRONT
                ? GL_FRONT : GL_BACK);
        }
        shader.setItem(item);
        item.drawGeometry();
    }
}

bool ShadowPass::buildTransforms(const LightInstance& instance, int maxSize, Matrix4* transforms)
{
    const Light& light = *instance.light;
    const ShadowConfig& config = light.shadow;
    const auto invalid = [&light](const char* reason) {
        if (light.lightType == LightType::POINT)
            std::cerr << "Point shadow " << reason << "; illuminating without shadows" << std::endl;
        return false;
    };

    if (config.width <= 0 || config.height <= 0)
    {
        return invalid("dimensions must be positive");
    }

    if (config.width > maxSize || config.height > maxSize)
    {
        return invalid("dimensions exceed the supported texture or viewport size");
    }

    if (!std::isfinite(config.near) || !std::isfinite(config.far) || config.near <= 0.f
        || config.far <= config.near)
    {
        return invalid("clipping interval requires finite 0 < near < far");
    }

    if (light.lightType == LightType::POINT)
    {
        if (config.width != config.height)
        {
            return invalid("cubemap dimensions must be square (width must equal height)");
        }

        if (!instance.position.isFinite())
        {
            return invalid("position must be finite");
        }

        // OpenGL cube face order: +X, -X, +Y, -Y, +Z, -Z.
        const Vector3 directions[] = {Vector3::XHAT, Vector3::MINUS_XHAT,
            Vector3::YHAT, Vector3::MINUS_YHAT, Vector3::ZHAT, Vector3::MINUS_ZHAT};
        const Vector3 ups[] = {Vector3::MINUS_YHAT, Vector3::MINUS_YHAT,
            Vector3::ZHAT, Vector3::MINUS_ZHAT, Vector3::MINUS_YHAT, Vector3::MINUS_YHAT};
        const Matrix4 projection = Matrix4::fromPerspective(deg2Rad(90.f), 1.f, config.near, config.far);

        for (int face = 0; face < 6; ++face)
        {
            const auto forward = directions[face];
            transforms[face] = projection.multiply(buildTransform(instance.position, forward, ups[face]));
            for (int i = 0; i < 16; ++i)
                if (!std::isfinite(transforms[face][i]))
                    return invalid("settings produce a non-finite face transform");
        }

        return true;
    }

    if (!instance.direction.isFinite() || instance.direction.dot(instance.direction) < 0.00001f)
    {
        return false;
    }
    
    const Vector3 forward = instance.direction.normalize();
    Vector3 position = instance.position;
    Matrix4 proj;
    if (light.lightType == LightType::DIRECTIONAL)
    {
        const auto& volume = config.directional;
        if (!volume.center.isFinite() || !std::isfinite(volume.width) || !std::isfinite(volume.height)
            || volume.width <= 0.f || volume.height <= 0.f)
        {
            return false;
        }

        // The configured center lies halfway through the near/far depth interval.
        position = volume.center.minus(forward.scale(config.near * 0.5f + config.far * 0.5f));
        proj = Matrix4::fromOrthographic(volume.width, volume.height, config.near, config.far);
    }
    else
    {
        const float angle = static_cast<const SpotLight&>(light).outerAngle;
        if (!position.isFinite() || !std::isfinite(angle) || angle <= 0.f || angle >= 90.f)
        {
            return false;
        }

        // The cone is circular even when the depth texture has rectangular texels.
        proj = Matrix4::fromPerspective(deg2Rad(2.f * angle), 1.f, config.near, config.far);
    }

    const Vector3 up = std::abs(forward.y) > 0.99f ? Vector3::ZHAT : Vector3::YHAT;
    transforms[0] = proj.multiply(buildTransform(position, forward, up));

    for (int i = 0; i < 16; ++i)
    {
        if (!std::isfinite(transforms[0][i]))
        {
            return false;
        }
    }

    return true;
}

Matrix4 ShadowPass::buildTransform(const Vector3& position, const Vector3& forward, const Vector3& up)
{
    const Vector3 right = forward.cross(up).normalize();
    const Vector3 vertical = right.cross(forward);
    return Matrix4(right.x, right.y, right.z, -right.dot(position),
        vertical.x, vertical.y, vertical.z, -vertical.dot(position),
        -forward.x, -forward.y, -forward.z, forward.dot(position),
        0.f, 0.f, 0.f, 1.f);
}
