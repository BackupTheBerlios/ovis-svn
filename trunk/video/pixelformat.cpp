#include "pixelformat.hh"

namespace ovis {
namespace video {

	bool isDepthformat(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_D16:
			case Pixelformat_D24:
			case Pixelformat_D32:
			case Pixelformat_D15S1:
			case Pixelformat_D24S4:
			case Pixelformat_D24S8:
			case Pixelformat_FD24S8:
				return true;
			default:return false;
		}
	}

	unsigned int numRGBALayoutChannels(const RGBALayouts fmt)
	{
		switch (fmt) {
			case RGBALayout_RGB:return 3;
			case RGBALayout_BGR:return 3;
			case RGBALayout_ARGB:return 4;
			case RGBALayout_ABGR:return 4;
			case RGBALayout_RGBA:return 4;
			case RGBALayout_BGRA:return 4;
			default: break;
		}

		return 0;
	}

	Pixelformat suggestPixelformat(const RGBALayouts rgbalayout,const Channeldatatype chtype,
		RGBALayouts& actualrgbalayout)
	{
		switch (chtype) {
			case Channeldatatype_Uint8:
				switch (rgbalayout) {
					case RGBALayout_RGB:return Pixelformat_XRGB8888; actualrgbalayout=RGBALayout_RGB; break;
					case RGBALayout_BGR:return Pixelformat_XBGR8888; actualrgbalayout=RGBALayout_BGR; break;
					case RGBALayout_ARGB:return Pixelformat_ARGB8888; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_ABGR8888; actualrgbalayout=RGBALayout_ABGR; break;
					case RGBALayout_RGBA:return Pixelformat_ARGB8888; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_ABGR8888; actualrgbalayout=RGBALayout_ABGR; break;
					default:return Pixelformat_Unknown; break;
				} break;
			case Channeldatatype_Uint16:
				switch (rgbalayout) {
					case RGBALayout_ARGB:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_RGBA:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					default:return Pixelformat_Unknown; break;
				} break;
			case Channeldatatype_Float16:
				switch (rgbalayout) {
					case RGBALayout_ARGB:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_RGBA:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					default:return Pixelformat_Unknown; break;
				} break;
			case Channeldatatype_Float32:
				switch (rgbalayout) {
					case RGBALayout_ARGB:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_RGBA:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					default:return Pixelformat_Unknown; break;
				} break;
			default: return Pixelformat_Unknown;
		}

		return Pixelformat_Unknown;
	}

}
}
