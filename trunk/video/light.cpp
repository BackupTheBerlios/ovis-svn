#include "light.hh"

namespace ovis {
namespace video {

	Light::Light(const base::String& identifier):Transformnode(identifier),
		m_Lighttype(Lighttype_Point),m_Range(0),
		m_ConstantAttenuation(0),m_LinearAttenuation(0),m_QuadraticAttenuation(0),m_Enabled(true) {}

	void Light::spawn(base::Streamable& datasource)
	{
	}

	void Light::serialize(base::Streamable& datasink) const
	{
	}

}
}
