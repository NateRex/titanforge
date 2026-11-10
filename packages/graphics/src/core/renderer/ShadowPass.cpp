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

    GLint textureUnits, combinedUnits, maxSize, maxViewport[2];
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewport);

    maxSize = std::min({maxSize, maxViewport[0], maxViewport[1]});
    const unsigned int hardwareBudget = std::max(0, std::min(textureUnits, combinedUnits) - static_cast<int>(FIRST_SHADOW_TEXTURE_UNIT));
    budget = std::min({budget, hardwareBudget, MAX_SHADOW_LIGHTS});

    unsigned int selected = 0, count = 0;
    for (LightInstance& instance : state.lights)
    {
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
        // TODO: Point-light shadows require a separate cubemap rendering capability.
        if (light.lightType == LightType::POINT || !light.shadow.enabled) continue;
        
        Matrix4 transform;
        if (count >= budget || !buildTransform(instance, maxSize, transform))
        {
            continue;
        }
        
        try
        {
            if (count == _targets.size()) 
            {
                _targets.push_back(nullptr);
            }

            // Re-use or set up new render target at slot count
            RenderTargetPtr target = _targets[count];
            if (!target || target->config().width != light.shadow.width || target->config().height != light.shadow.height)
            {
                RenderTargetConfig config;
                config.sizeMode = TargetSizeMode::FIXED;
                config.width = light.shadow.width;
                config.height = light.shadow.height;
                config.colorFormats.clear();
                config.depthStencilStorage = DepthStencilStorage::TEXTURE;
                config.depthStencilFormat = PixelFormat::DEPTH32F;
                target = RenderTarget::create(config);
            }

            target->frameBuffer()->bind();
            glViewport(0, 0, light.shadow.width, light.shadow.height);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
            glClearDepth(1.0);
            glClear(GL_DEPTH_BUFFER_BIT);

            auto shader = std::static_pointer_cast<ShadowDepthShader>(ShaderManager::getShader(ShaderId::SHADOW_DEPTH));
            shader->activate();
            shader->setProjection(transform);

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

                if (material.cullingMode == CullingMode::NONE)
                {
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(material.cullingMode == CullingMode::FRONT ? GL_FRONT : GL_BACK);
                }
                shader->setItem(item);
                item.drawGeometry();
            }

            instance.shadowMap = target->depthStencilTexture();
            instance.shadowTransform = transform;
            instance.shadowIndex = count++;
        }
        catch (const std::exception& error)
        {
            std::cerr << std::string("Shadow generation failed for light; illuminating without shadows: ") + error.what() << std::endl;
        }
    }
    
    _targets.resize(count);
}

bool ShadowPass::buildTransform(const LightInstance& instance, int maxSize, Matrix4& transform)
{
    const Light& light = *instance.light;
    const ShadowConfig& config = light.shadow;
    if (config.width <= 0 || config.height <= 0 || config.width > maxSize || config.height > maxSize
        || !std::isfinite(config.near) || !std::isfinite(config.far) || config.near <= 0.f
        || config.far <= config.near || !instance.direction.isFinite() || instance.direction.dot(instance.direction) < 0.00001f)
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
    const Vector3 right = forward.cross(up).normalize();
    const Vector3 vertical = right.cross(forward);

    Matrix4 view(
        right.x, right.y, right.z, -right.dot(position),
        vertical.x, vertical.y, vertical.z, -vertical.dot(position),
        -forward.x, -forward.y, -forward.z, forward.dot(position),
        0.f, 0.f, 0.f, 1.f);
    transform = proj.multiply(view);

    for (int i = 0; i < 16; ++i)
    {
        if (!std::isfinite(transform[i]))
        {
            return false;
        }
    }

    return true;
}