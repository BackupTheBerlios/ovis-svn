#ifndef OVIS_VIDEO_SIMPLEPICBUFFER_HH_INCLUDED
#define OVIS_VIDEO_SIMPLEPICBUFFER_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/itypes.hh"
#include "channeldatatype.hh"
#include "pixelformat.hh"
#include "picbuffer.hh"

namespace ovis {
namespace video {

	class OVIS_API SimplePicbuffer:public Picbuffer
	{
	public:
		SimplePicbuffer(const ovis_uint32 width,const ovis_uint32 height,
			const Channeldatatype channeldatatype,const RGBALayouts rgbalayout);

		SimplePicbuffer(const Picbuffer& src,const Channeldatatype channeldatatype,const RGBALayouts rgbalayout);

		// converts pixelformat if necessary
		void copyFrom(const Picbuffer& src);
		
		inline ovis_uint32 stride() const { return m_Width*bytesPerPixel(); }
		ovis_uint32 bytesPerPixel() const;

		inline ovis_uint32 width() const { return m_Width; }
		inline ovis_uint32 height() const { return m_Height; }

		inline RGBALayouts rgbalayout() const { return m_RGBALayout; }

	protected:
		ovis_uint32 m_Width,m_Height;
		RGBALayouts m_RGBALayout;
	};

}
}

#endif
