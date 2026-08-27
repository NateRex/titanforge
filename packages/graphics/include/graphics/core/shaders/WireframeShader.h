#pragma once
#include <graphics/core/shaders/pointers/WireframeShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Shader used to handle wireframe materials applied to meshes
 * @author Nathaniel Rex
 */
class WireframeShader : public Shader
{
public:

    /**
     * Creates a new WireframeShader instance
     * @return The WireframeShader instance
     */
    static WireframeShaderPtr create()
    {
        return std::shared_ptr<WireframeShader>(new WireframeShader());
    }

    void setItem(const RenderItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    WireframeShader();
};
