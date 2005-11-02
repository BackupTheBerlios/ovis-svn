#ifndef OVIS_VIDEO_PICBUFFER_HH_INCLUDED
#define OVIS_VIDEO_PICBUFFER_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/itypes.hh"
#include "channeldatatype.hh"
#include "pixelformat.hh"

namespace ovis {
namespace video {

	class OVIS_API Picbuffer:public base::Datablock
	{
	public:
		Picbuffer():m_Channeldatatype(Channeldatatype_Uint8) {}
		Picbuffer(const Channeldatatype channeldatatype):
		m_Channeldatatype(channeldatatype) {}

		inline const Channeldatatype& channeldatatype() const { return m_Channeldatatype; }

		//! Stride, in BYTES (not pixels)
		virtual ovis_uint32 stride() const=0;
		virtual ovis_uint32 bytesPerPixel() const=0;

		virtual ovis_uint32 width() const=0;
		virtual ovis_uint32 height() const=0;

		/* Valid:
		Pixelformat_RGB,
		Pixelforamt_BGR,
		Pixelformat_RGBA,
		Pixelformat_ARGB,
		Pixelformat_BGRA,
		Pixelformat_ABGR
		*/
		virtual RGBALayouts rgbalayout() const=0;

	protected:
		Channeldatatype m_Channeldatatype;
	};

	OVIS_API bool transfer(const Picbuffer& src,Picbuffer& dest);
	OVIS_API bool calculateNormalmap(const Picbuffer& src,Picbuffer& dest);


}
}

#endif
