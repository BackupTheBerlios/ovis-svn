#ifndef OVIS_VIDEO_LIGHTLIGHT_HH
#define OVIS_VIDEO_LIGHTLIGHT_HH

#include "light.hh"

namespace ovis {
namespace video {

	class OVIS_API Lightlist
	{
	public:
		virtual void addLight(Light* pLight);
		virtual void removeLight(Light* pLight);
		virtual void clearLights();
		virtual unsigned int numLights() const;
		virtual Light& light(const unsigned int lightnr);
		virtual const Light& light(const unsigned int lightnr) const;

		Lightlist();
		virtual ~Lightlist();
	protected:
		struct Internaldata;
		Internaldata *m_pInternaldata;
	};

}
}

#endif
