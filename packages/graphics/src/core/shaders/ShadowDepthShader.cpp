#include <graphics/core/shaders/ShadowDepthShader.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/core/buffers/GeometryBuffer.h>
#include <graphics/core/buffers/InstanceBuffer.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/textures/Texture.h>
#include <glad/glad.h>

constexpr const char* SHADOW_VERTEX = R"(
    #version 330 core

    layout(location = 0) in vec3 position;
    layout(location = 2) in vec4 color;
    layout(location = 3) in vec2 texCoord;
    layout(location = 4) in mat4 instanceModel;
    
    uniform mat4 model;
    uniform mat4 lightProjection;
    uniform bool instanced;
    out vec4 vertexColor;
    out vec2 uv;
    
    void main() {
        mat4 transform = model;
        if (instanced) transform *= instanceModel;
        gl_Position = lightProjection * transform * vec4(position, 1.0);
        vertexColor = color;
        uv = texCoord;
    }
)";

constexpr const char* SHADOW_FRAGMENT = R"(
    #version 330 core

    in vec4 vertexColor;
    in vec2 uv;
    
    uniform bool masked;
    uniform bool useVertexColor;
    uniform float alpha;
    uniform float cutoff;
    uniform bool hasTexture;
    uniform sampler2D colorTexture;
    
    void main() {
        if (masked) {
            float opacity = useVertexColor ? vertexColor.a : alpha;
            if (hasTexture) opacity *= texture(colorTexture, uv).a;
            if (opacity < cutoff) discard;
        }
    }
)";

ShadowDepthShader::ShadowDepthShader() : Shader("ShadowDepthShader", SHADOW_VERTEX, SHADOW_FRAGMENT) {}

void ShadowDepthShader::setProjection(const Matrix4& transform)
{
    ProgramBinding binding(this);
    glUniformMatrix4fv(getUniformLocation("lightProjection"), 1, GL_TRUE, transform.getValues());
}

void ShadowDepthShader::setItem(const DrawItem& item)
{
    ProgramBinding binding(this);

    const auto* material = static_cast<const MeshMaterial*>(item.material);
    const GeometryBuffer* buffer = item.getBuffer();

    glUniformMatrix4fv(getUniformLocation("model"), 1, GL_TRUE, item.modelTransform.getValues());
    glUniform1i(getUniformLocation("instanced"), item.instanceBuffer != nullptr);
    glUniform1i(getUniformLocation("masked"), material->getEffectiveAlphaMode() == AlphaMode::MASK);
    glUniform1i(getUniformLocation("useVertexColor"), material->useVertexColors && buffer->getGeometryAttributes().colors);
    glUniform1f(getUniformLocation("alpha"), material->color.alpha());
    glUniform1f(getUniformLocation("cutoff"), material->alphaCutoff);
    glUniform1i(getUniformLocation("hasTexture"), material->texture != nullptr);
    glUniform1i(getUniformLocation("colorTexture"), 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->texture ? material->texture->id() : 0);
}
