#ifndef OVIS_VIDEO_COLORSCALE_HH_INCLUDED
#define OVIS_VIDEO_COLORSCALE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/itypes.hh"

namespace ovis {
namespace video {

	class OVIS_API Colorscale
	{
	public:
		Colorscale();
		~Colorscale();

		void addColor(const float position,const ovis_uint8 a,const ovis_uint8 b,const ovis_uint8 g,const ovis_uint8 r);
		void addColor(const float position,const ovis_uint32 abgr);

		ovis_uint32 lerpColor(const float position) const;
		void lerpColor(const float position,ovis_uint8 &a,ovis_uint8 &b,ovis_uint8 &g,ovis_uint8 &r) const;

	protected:
		struct Colorlist;
		Colorlist *m_pColors;
	};

}
}

#endif
