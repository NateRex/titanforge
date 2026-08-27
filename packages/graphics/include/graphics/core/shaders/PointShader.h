#pragma once
#include <graphics/core/shaders/pointers/PointShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Shader used to handle point primitives and their materials
 * @author Nathaniel Rex
 */
class PointShader : public Shader
{
public:

    /**
     * Creates a new PointShader instance
     * @return The PointShader instance
     */
    static PointShaderPtr create()
    {
        return std::shared_ptr<PointShader>(new PointShader());
    }

    void setItem(const RenderItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    PointShader();
};
