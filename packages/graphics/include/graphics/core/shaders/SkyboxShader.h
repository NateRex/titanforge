#pragma once
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/pointers/ShaderPtr.h>

/**
 * Shader used to handle skyboxes
 * @author Nathaniel Rex
 */
class SkyboxShader : public Shader
{
public:

    /**
	 * Constructs a new SkyboxShader instance
	 * @return The new SkyboxShader instance
	 */
    static ShaderPtr create()
    {
        return std::shared_ptr<SkyboxShader>(new SkyboxShader());
    }

    void setCamera(Camera* camera) override;

    void setMaterial(const Material* material) override;

private:

    /**
     * Constructor
     */
    SkyboxShader();
};
