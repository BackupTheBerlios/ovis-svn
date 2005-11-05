#include "tooglconvs.hh"

namespace ovis {
namespace opengldrv {

	GLint oglpixelformat(const video::Pixelformat ionpixelformat)
	{
		switch (ionpixelformat) {
			case video::Pixelformat_RGB888:return GL_RGB8;
			case video::Pixelformat_XRGB8888:return GL_RGB8;
			case video::Pixelformat_XBGR8888:return GL_RGB8;
			case video::Pixelformat_ARGB8888:return GL_RGBA8;
			case video::Pixelformat_ABGR8888:return GL_RGBA8;
			case video::Pixelformat_RGB565:return GL_RGB5;
			case video::Pixelformat_XRGB1555:return GL_RGB5;
			case video::Pixelformat_ARGB1555:return GL_RGB5_A1;
			case video::Pixelformat_ARGB4444:return GL_RGBA4;
			case video::Pixelformat_RGB332:return GL_R3_G3_B2;
			case video::Pixelformat_RGB444:return GL_RGB4;
			case video::Pixelformat_A2RGB101010:return GL_RGB10_A2;
			case video::Pixelformat_A2BGR101010:return GL_RGB10_A2;
			case video::Pixelformat_ARGB16161616:return GL_RGBA16;
			/*case video::Pixelformat_FARGB16161616:return D3DFMT_A16B16G16R16F; // TODO: Rename FARGB to FABGR
			case video::Pixelformat_FARGB32323232:return D3DFMT_A32B32G32R32F; // TODO: Rename FARGB to FABGR
			case video::Pixelformat_D16:return D3DFMT_D16;
			case video::Pixelformat_D24:return D3DFMT_D24X8;
			case video::Pixelformat_D32:return D3DFMT_D32;
			case video::Pixelformat_D15S1:return D3DFMT_D15S1;
			case video::Pixelformat_D24S4:return D3DFMT_D24X4S4;
			case video::Pixelformat_D24S8:return D3DFMT_D24X8;
			case video::Pixelformat_FD24S8:return D3DFMT_D24FS8;
			case video::Pixelformat_RGB_DXT1:return D3DFMT_DXT1;
			case video::Pixelformat_RGBA_DXT1:return D3DFMT_DXT2; // TODO: Examine this one
			case video::Pixelformat_RGBA_DXT3:return D3DFMT_DXT3;
			case video::Pixelformat_RGBA_DXT5:return D3DFMT_DXT5;*/
			default:return -1;
		}
	}

}
}
