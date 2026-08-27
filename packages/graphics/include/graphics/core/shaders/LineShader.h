#pragma once
#include <graphics/core/shaders/pointers/LineShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Shader used to handle lines and their materials
 * @author Nathaniel Rex
 */
class LineShader : public Shader
{
public:

    /**
     * Creates a new LineShader instance
     * @return The LineShader instance
     */
    static LineShaderPtr create()
    {
        return std::shared_ptr<LineShader>(new LineShader());
    }

    void setItem(const RenderItem& item) override;

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    LineShader();
};
