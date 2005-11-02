#ifndef OVIS_VIDEO_TEXTURE2D_HH_INCLUDED
#define OVIS_VIDEO_TEXTURE2D_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/itypes.hh"
#include "../base/refptr.hh"
#include "pixelformat.hh"
#include "texture.hh"

namespace ovis {

namespace base {
	class Datablock;
}

namespace math {
	class FilterBase;
}

namespace video {

	class Picbuffer;

	class OVIS_API Texture2D:public Texture
	{
	public:
	//	virtual ovis_uint32 numLevels() const=0;

		/// Computes mipmaps for this texture using the resizing filter "filter"
		void computeMipmaps(const Picbuffer& pic,const math::FilterBase& filter,const unsigned int numLevels=0);

		// Copies pixel data from "datablock"
		virtual void transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
			const ovis_uint32 height,const RGBALayouts rgbalayout,const Channeldatatype channeldatatype)=0;

		// Copies pixel data from picbuffer "pic"
		void transferPic(const ovis_uint32 level,const Picbuffer& pic);

		inline void currentLevel(const ovis_uint32 level) { m_CurrentLevel=level; }
		inline ovis_uint32 currentLevel() const { return m_CurrentLevel; }

		virtual ovis_uint32 width() const=0;
		virtual ovis_uint32 height() const=0;
		virtual ovis_uint32 numMipmaplevels() const=0;

		virtual void copyFromFramebuffer(const ovis_uint32 x,const ovis_uint32 y)=0;

		//inline void transferWidth(const ovis_uint32 _width) { m_Transferwidth=(_width==0) ? width() : _width; }
		//inline void transferHeight(const ovis_uint32 _height) { m_Transferheight=(_height==0) ? height() : _height; }
	protected:

		Texture2D(const Pixelformat requestedpixelformat,const base::String&identifier,const base::String& texturesource);

		ovis_uint32 m_CurrentLevel/*,m_Transferwidth,m_Transferheight*/;
	};

	typedef base::RefPtr<Texture2D> Texture2DPtr;

}
}

#endif
