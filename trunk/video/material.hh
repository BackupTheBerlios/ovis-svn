#ifndef OVIS_VIDEO_MATERIAL_HH_INCLUDED
#define OVIS_VIDEO_MATERIAL_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/basememobj.hh"
#include "../base/numericarrays.hh"

#include "camera.hh"
#include "memindexstream.hh"
#include "memvertexstream.hh"
#include "renderer.hh"

namespace ovis {
namespace video {

	class Material
	{
	public:
		Material();
		Material(const Material& m);
		~Material();

		void clone(const Material& m);
		Material& operator =(const Material& m);

		const base::Float4& diffuseColor() const;
		base::Float4& diffuseColor();
		const base::Float4& specularColor() const;
		base::Float4& specularColor();
		const base::Float4& emissiveColor() const;
		base::Float4& emissiveColor();

		float specularPower() const;
		void specularPower(const float power);
	protected:
		base::Float4 m_DiffuseColor,m_SpecularColor,m_EmissiveColor;
		float m_SpecularPower;
	};

}
}

#endif
