#ifndef OVIS_VIDEO_CUBEMAP_HH_INCLUDED
#define OVIS_VIDEO_CUBEMAP_HH_INCLUDED

#include "../base/itypes.hh"
#include "../base/refptr.hh"
#include "texture.hh"

namespace ovis {

namespace base {
	class Datablock;
}

namespace video {

	class Picbuffer;

	class OVIS_API Cubemap:public Texture
	{
	public:
		enum Face {
			FrontFace=0,
			BackFace,
			LeftFace,
			RightFace,
			TopFace,
			BottomFace
		};

		inline void currentFace(const Face face) { m_CurrentFace=face; }
		inline Face currentFace() const { return m_CurrentFace; }

		/*inline void currentLevel(const ovis_uint32 level) { m_CurrentLevel=level; }
		inline ovis_uint32 currentLevel() const { return m_CurrentLevel; }*/

		virtual ovis_uint32 edgelength() const=0;
		virtual ovis_uint32 numMipmaplevels() const=0;

		// TODO: transferData does not convert pixel formats and sizes yet

		virtual void transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
			const RGBALayouts rgbalayout,const Channeldatatype channeldatatype)=0;
		void transferPic(const ovis_uint32 level,const Picbuffer& pic);
	protected:
		Cubemap(const Pixelformat _pixelformat,const base::String &identifier,const base::String& texturesource):Texture(_pixelformat,identifier,texturesource),m_CurrentFace(FrontFace) {}

		Face m_CurrentFace;
	};

	typedef base::RefPtr<Cubemap> CubemapPtr;

}
}

#endif
