#include "picbuffer.hh"
#include "pixmap.hh"
#include "simplepicbuffer.hh"
#include "texture2d.hh"
#include <math.h>

namespace ovis {
namespace video {

	template <typename T> void template_ComputeMipmaps(Texture2D& tex,const Picbuffer& pic,const math::FilterBase& filter,const unsigned int numLevels)
	{	
		tex.currentLevel(0);
		tex.transferPic(0,pic);

		ovis_uint32 width=pic.width(),height=pic.height();
		ovis_uint32 levels=(numLevels>tex.numMipmaplevels()) ? numLevels : tex.numMipmaplevels();

		if (levels==0) {

			levels=tex.numMipmaplevels();
		}

		width>>=1; height>>=1;

		Pixmap<T> base(pic);

		for (unsigned int level=1;level<levels;++level) {
			Pixmap<T> mipmap(width,height);

			mipmap.filteredRescale(base,filter);
			tex.currentLevel(level);
			tex.transferPic(level,mipmap);

			width>>=1;
			height>>=1;
		}
	}

	void Texture2D::computeMipmaps(const Picbuffer& pic,const math::FilterBase& filter,const ovis_uint32 numLevels)
	{
		switch (pic.channeldatatype()) {
			case Channeldatatype_Uint8:template_ComputeMipmaps<ovis_uint8>(*this,pic,filter,numLevels); break;
			case Channeldatatype_Float32:template_ComputeMipmaps<float>(*this,pic,filter,numLevels); break;
			default:break;
		}
	}

	void Texture2D::transferPic(const ovis_uint32 level,const Picbuffer& pic)
	{
		transferData(level,pic,pic.width()*pic.bytesPerPixel(),pic.height(),pic.rgbalayout(),pic.channeldatatype());
	}

	Texture2D::Texture2D(const Pixelformat requestedpixelformat,const base::String&identifier,const base::String& texturesource):
	Texture(requestedpixelformat,identifier,texturesource),m_CurrentLevel(0) {}

}
}
