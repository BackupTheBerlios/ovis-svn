#include <vector>
#include <algorithm>

#include "lightlist.hh"

namespace ovis {
namespace video {

	struct Lightlist::Internaldata
	{
		std::vector<Light*> m_Lights;
	};

	Lightlist::Lightlist():m_pInternaldata(new Internaldata) {}
	Lightlist::~Lightlist() { delete m_pInternaldata; }

	void Lightlist::addLight(Light* pLight)
	{
		m_pInternaldata->m_Lights.push_back(pLight);
	}

	void Lightlist::removeLight(Light* pLight)
	{
		std::vector<Light*>::iterator it=
			std::find(m_pInternaldata->m_Lights.begin(),
			m_pInternaldata->m_Lights.end(),pLight);

		if (it!=m_pInternaldata->m_Lights.end())
			m_pInternaldata->m_Lights.erase(it);
	}

	void Lightlist::clearLights()
	{
		m_pInternaldata->m_Lights.clear();
	}

	unsigned int Lightlist::numLights() const
	{
		return (unsigned int)(m_pInternaldata->m_Lights.size());
	}

	Light& Lightlist::light(const unsigned int lightnr)
	{
		return *((m_pInternaldata->m_Lights)[lightnr]);
	}

	const Light& Lightlist::light(const unsigned int lightnr) const
	{
		return *((m_pInternaldata->m_Lights)[lightnr]);
	}

}
}
