#pragma once
#include <graphics/materials/Material.h>
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/materials/pointers/BasicMaterialPtr.h>

/**
 * Material for drawing geometries in a flat or wireframe way.
 * @author Nathaniel Rex
 */
class BasicMaterial : public Material
{
public:

	/**
	 * Flag that, when true, forces the use of colors specified on geometry vertices (when available)
	 */
	const bool useVertexColors;

private:

	/**
	 * Constructor
	 * @param color Color
	 * @param texture Texture. Can be null.
	 * @param useVertexColors Flag that, when true, forces the use of colors specified on geometry vertices
	 * (when available)
	 */
	BasicMaterial(const Color& color, const TexturePtr texture, bool useVertexColors);

public:

	/**
	 * Builder used to construct a MeshMaterial instance
	 * @author Nathaniel Rex
	 */
	class Builder : public Material::Builder<Builder>
	{
	public:

		/**
		 * Sets the flag that, when true, forces the use of colors specified on geometry vertices (when available)
		 * @param shouldUseVertexColors True if colors specified on geometry vertices should be used when available. False
		 * otherwise.
		 * @return This builder
		 */
		Builder& useVertexColors(bool shouldUseVertexColors);

		/**
		 * @return A material constructed from the data stored in this builder
		 */
		BasicMaterialPtr build() const;

	protected:

		/**
		 * Flag that, when true, forces the use of colors specified on geometry vertices (when available). Defaults to true.
		 */
		bool _useVertexColors = true;
	};
};