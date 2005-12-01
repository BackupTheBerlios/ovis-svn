#include "material.hh"

namespace ovis {
namespace video {

	Material::Material():m_SpecularPower(0) {}
	Material::Material(const Material& m) { clone(m); }
	Material::~Material() {}

	void Material::clone(const Material& m)
	{
		m_DiffuseColor=m.diffuseColor();
		m_SpecularColor=m.specularColor();
		m_EmissiveColor=m.emissiveColor();
		m_SpecularPower=m.specularPower();
	}

	Material& Material::operator =(const Material& m)
	{
		clone(m);
		return *this;
	}

	const base::Float4& Material::diffuseColor() const
	{
		return m_DiffuseColor;
	}

	base::Float4& Material::diffuseColor()
	{
		return m_DiffuseColor;
	}

	const base::Float4& Material::specularColor() const
	{
		return m_SpecularColor;
	}

	base::Float4& Material::specularColor()
	{
		return m_SpecularColor;
	}

	const base::Float4& Material::emissiveColor() const
	{
		return m_EmissiveColor;
	}

	base::Float4& Material::emissiveColor()
	{
		return m_EmissiveColor;
	}

	float Material::specularPower() const
	{
		return m_SpecularPower;
	}

	void Material::specularPower(const float power)
	{
		m_SpecularPower=power;
	}


}
}
