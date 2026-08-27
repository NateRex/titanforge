#pragma once
#include <graphics/core/shaders/pointers/SkyboxShaderPtr.h>
#include <graphics/core/shaders/Shader.h>

/**
 * Shader used to handle skyboxes
 * @author Nathaniel Rex
 */
class SkyboxShader : public Shader
{
public:

    /**
	 * Constructs a new SkyboxShader instance. This should typically only be done once, by the shader manager.
	 * @return The new SkyboxShader instance
	 */
    static SkyboxShaderPtr create()
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
