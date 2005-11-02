#include "simplepicbuffer.hh"

namespace ovis {
namespace video {

	SimplePicbuffer::SimplePicbuffer(const ovis_uint32 width,const ovis_uint32 height,
		const Channeldatatype channeldatatype,const RGBALayouts rgbalayout):
	Picbuffer(channeldatatype),m_Width(width),m_Height(height),m_RGBALayout(rgbalayout)
	{
		resize(stride()*height);
	}

	SimplePicbuffer::SimplePicbuffer(const Picbuffer& src,const Channeldatatype channeldatatype,
		const RGBALayouts rgbalayout):Picbuffer(channeldatatype),m_Width(0),m_Height(0),m_RGBALayout(rgbalayout)
	{
		copyFrom(src);
	}

	ovis_uint32 SimplePicbuffer::bytesPerPixel() const
	{
		return channeldatatypeSizeLookup(m_Channeldatatype)*numRGBALayoutChannels(m_RGBALayout);
	}

	void SimplePicbuffer::copyFrom(const Picbuffer& src)
	{
		if ((src.rgbalayout()==m_RGBALayout) && (src.channeldatatype()==channeldatatype())) {
			m_Width=src.width();
			m_Height=src.height();
			Datablock::clone(src);
		} else {
			int srcrofs=0,srcgofs=1,srcbofs=2,srcaofs=3,srcnumchannels=0;
			int dstrofs=0,dstgofs=1,dstbofs=2,dstaofs=3,dstnumchannels=0;
			switch (src.rgbalayout()) {
				case RGBALayout_RGB:srcrofs=0; srcgofs=1; srcbofs=2; srcaofs=-1; srcnumchannels=3; break;
				case RGBALayout_BGR:srcrofs=2; srcgofs=1; srcbofs=0; srcaofs=-1; srcnumchannels=3; break;
				case RGBALayout_RGBA:srcrofs=0; srcgofs=1; srcbofs=2; srcaofs=3; srcnumchannels=4; break;
				case RGBALayout_BGRA:srcrofs=2; srcgofs=1; srcbofs=0; srcaofs=3; srcnumchannels=4; break;
				case RGBALayout_ARGB:srcrofs=1; srcgofs=2; srcbofs=3; srcaofs=0; srcnumchannels=4; break;
				case RGBALayout_ABGR:srcrofs=3; srcgofs=2; srcbofs=1; srcaofs=0; srcnumchannels=4; break;
				default:break;
			}
			switch (rgbalayout()) {
				case RGBALayout_RGB:dstrofs=0; dstgofs=1; dstbofs=2; dstaofs=-1; dstnumchannels=3; break;
				case RGBALayout_BGR:dstrofs=2; dstgofs=1; dstbofs=0; dstaofs=-1; dstnumchannels=3; break;
				case RGBALayout_RGBA:dstrofs=0; dstgofs=1; dstbofs=2; dstaofs=3; dstnumchannels=4; break;
				case RGBALayout_BGRA:dstrofs=2; dstgofs=1; dstbofs=0; dstaofs=3; dstnumchannels=4; break;
				case RGBALayout_ARGB:dstrofs=1; dstgofs=2; dstbofs=3; dstaofs=0; dstnumchannels=4; break;
				case RGBALayout_ABGR:dstrofs=3; dstgofs=2; dstbofs=1; dstaofs=0; dstnumchannels=4; break;
				default:break;
			}

			if (src.channeldatatype()==channeldatatype()) {
				if ((m_Width!=src.width()) || (m_Height!=src.height())) {
					m_Width=src.width();
					m_Height=src.height();
					resize(stride()*height());
				}

				const ovis_uint8* pSrc=(src.data());
				const ovis_uint8* pSrcEnd=(src.data()+src.stride()*src.height());
				ovis_uint8* pDest=data();

				unsigned int chsize=channeldatatypeSizeLookup(m_Channeldatatype);

				for (;pSrc<pSrcEnd;pSrc+=srcnumchannels*chsize,pDest+=dstnumchannels*chsize) {
					memcpy(&pDest[dstrofs*chsize],&pSrc[srcrofs*chsize],chsize);
					memcpy(&pDest[dstgofs*chsize],&pSrc[srcgofs*chsize],chsize);
					memcpy(&pDest[dstbofs*chsize],&pSrc[srcbofs*chsize],chsize);

					if (dstaofs!=-1) {
						if (srcaofs==-1)
							memset(&pDest[dstaofs*chsize],0,chsize);
						else
							memcpy(&pDest[dstaofs*chsize],&pSrc[srcaofs*chsize],chsize);
					}

				}
			}
			// TODO: Convert channel datatypes
		}
	}

}
}
