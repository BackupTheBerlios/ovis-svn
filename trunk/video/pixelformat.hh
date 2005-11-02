#ifndef OVIS_VIDEO_PIXELFORMAT_HH_INCLUDED
#define OVIS_VIDEO_PIXELFORMAT_HH_INCLUDED

#include "../base/dll.hh"
#include "channeldatatype.hh"

namespace ovis {
namespace video {

	enum RGBALayouts {
		RGBALayout_RGB,
		RGBALayout_BGR,
		RGBALayout_RGBA,
		RGBALayout_ARGB,
		RGBALayout_BGRA,
		RGBALayout_ABGR,
	};

	enum Pixelformat {
		// These are intended for internal pixel format in textures
		Pixelformat_RGB888=0,			// 24-bit RGB
		Pixelformat_XRGB8888=1,			// 32-bit RGB (the upper 8 bit are ignored)
		Pixelformat_XBGR8888=2,			// 32-bit BGR (the upper 8 bit are ignored)
		Pixelformat_ARGB8888=3,			// 32-bit ARGB
		Pixelformat_ABGR8888=4,			// 32-bit ABGR
		Pixelformat_RGB565=5,			// 16-bit RGB (R 5 bit G 6 bit B 5 bit)
		Pixelformat_XRGB1555=6,			// 15-bit RGB (5 bit per component, the MSB is ignored)
		Pixelformat_ARGB1555=7,			// 15-bit ARGB (5 bit per RGB component, the MSB is used for alpha)
		Pixelformat_ARGB4444=8,			// 16-bit ARGB (4 bit per component)
		Pixelformat_RGB332=9,
		Pixelformat_RGB444=10,
		Pixelformat_A2RGB101010=11,
		Pixelformat_A2BGR101010=12,
		Pixelformat_ARGB16161616=13,
		Pixelformat_FARGB16161616=14,
		Pixelformat_FARGB32323232=15,
		Pixelformat_D16=16,
		Pixelformat_D24=17,
		Pixelformat_D32=18,
		Pixelformat_D15S1=19,
		Pixelformat_D24S4=20,
		Pixelformat_D24S8=21,
		Pixelformat_FD24S8=22,
		Pixelformat_RGB_DXT1=23,
		Pixelformat_RGBA_DXT1=24,
		Pixelformat_RGBA_DXT3=25,
		Pixelformat_RGBA_DXT5=26,

		Pixelformat_NumFormats=27,

		// These are intended for data transfer, indicating the RGB order & the presence of an alpha channel
	/*	Pixelformat_RGB=400,
		Pixelformat_BGR,
		Pixelformat_RGBA,
		Pixelformat_ARGB,
		Pixelformat_BGRA,
		Pixelformat_ABGR,*/

		Pixelformat_Unknown=0xfffe,
		Pixelformat_None=0xffff
	};

	OVIS_API bool isDepthformat(const Pixelformat pixelformat);

	OVIS_API Pixelformat suggestPixelformat(const RGBALayouts rgbalayout,const Channeldatatype chtype,
		RGBALayouts& actualrgbalayout);
	OVIS_API unsigned int numRGBALayoutChannels(const RGBALayouts fmt);

}
}

#endif
