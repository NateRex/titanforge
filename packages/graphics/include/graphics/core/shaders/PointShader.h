#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

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
    static ShaderPtr create()
    {
        return std::shared_ptr<PointShader>(new PointShader());
    }

    void setItem(const DrawItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    PointShader();
};
