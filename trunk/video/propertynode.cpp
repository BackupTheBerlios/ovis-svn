#include "propertynode.hh"

namespace ovis {
namespace video {

	Propertynode::Propertynode(const base::String& identifier):Scenenode(identifier)
	{
	}

	Propertynode::Propertynode(const Propertynode &srcPropertynode):Scenenode(srcPropertynode)
	{
		clone(srcPropertynode);
	}

	Propertynode::~Propertynode()
	{
	}

	void Propertynode::onVisit(Visitcontext& visitcontext)
	{
	}

	void Propertynode::spawn(base::Streamable& datasource)
	{
	}

	void Propertynode::serialize(base::Streamable& datasink) const
	{
	}

	const char* Propertynode::type() const
	{
		return "Propertynode";
	}

	void Propertynode::clone(const Propertynode &srcPropertynode)
	{
		Scenenode::clone(srcPropertynode);
		m_Properties.clone(srcPropertynode.properties());
	}

	Propertynode& Propertynode::operator =(const Propertynode& src)
	{
		clone(src); return *this;
	}

	Propertytable& Propertynode::properties()
	{
		return m_Properties;
	}

	const Propertytable& Propertynode::properties() const
	{
		return m_Properties;
	}

}
}
