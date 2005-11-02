#ifndef OVIS_VIDEO_TEXTURE_HH_INCLUDED
#define OVIS_VIDEO_TEXTURE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/managedmemobj.hh"
#include "../base/refcounted.hh"
#include "../base/string.hh"
#include "channeldatatype.hh"
#include "pixelformat.hh"

namespace ovis {
namespace video {

	class OVIS_API Texture:public base::RefCounted
	{
	public:
		Pixelformat requestedpixelformat() const;
		virtual Pixelformat actualpixelformat() const=0;

		virtual bool isDataOK() const=0;
		virtual void dataIsOK()=0;

		virtual ~Texture() {}
		
		const base::String& texturesource() const;
	protected:
		Texture(const Pixelformat requestedpixelformat,const base::String& identifier,const base::String& texturesource);

		Pixelformat m_RequestedPixelformat;
		base::String m_Texturesource;
	};

}
}

#endif
