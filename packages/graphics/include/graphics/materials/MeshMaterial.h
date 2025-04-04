#pragma once
#include <graphics/materials/Material.h>
#include <graphics/materials/pointers/MeshMaterialPtr.h>

/**
 * Material for drawing mesh geometries
 * @author Nathaniel Rex
 */
class MeshMaterial : public Material
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
	MeshMaterial(const Color& color, const Texture* texture, bool useVertexColors);

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
		MeshMaterialPtr build() const;

	protected:

		/**
		 * Flag that, when true, forces the use of colors specified on geometry vertices (when available). Defaults to true.
		 */
		bool _useVertexColors = true;
	};
};