#include <graphics/shaders/IShader.h>

/**
 * Vertex shader
 * @author Nathaniel Rex
 */
class VertexShader : public IShader
{
public:

	friend class ShaderManager;

private:

	/**
	 * Constructor
	 */
	VertexShader();
};