#ifndef OVIS_VIDEO_PROGRAM_HH_INCLUDED
#define OVIS_VIDEO_PROGRAM_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/dll.hh"
#include "../base/numericarrays.hh"
#include "../base/itypes.hh"
#include "../base/managedmemobj.hh"
#include "../base/string.hh"
#include "cubemap.hh"
#include "texture2d.hh"

namespace ovis {
namespace video {

	class Videodevice;

	class OVIS_API Program:public base::ManagedMemObj
	{
	public:
		virtual ~Program();

		inline const base::String& identifier() const { return m_Identifier; }
		inline const base::String& format() const { return m_Format; }
		inline const base::String& entrypoint() const { return m_Entrypoint; }

		virtual void set()=0;

		virtual void setFloat(const base::String& varname,const float)=0;
		virtual void setFloat2(const base::String& varname,const base::Float2&)=0;
		virtual void setFloat3(const base::String& varname,const base::Float3&)=0;
		virtual void setFloat4(const base::String& varname,const base::Float4&)=0;
		virtual void setFloat3x3(const base::String& varname,const base::Float3x3&)=0;
		virtual void setFloat4x3(const base::String& varname,const base::Float4x3&)=0;
		virtual void setFloat3x4(const base::String& varname,const base::Float3x4&)=0;
		virtual void setFloat4x4(const base::String& varname,const base::Float4x4&)=0;
		virtual void bind2DTexture(const base::String& varname,const Texture2D* texVar)=0;
		virtual void bindCubemap(const base::String& varname,const Cubemap* texVar)=0;

		virtual bool isValid() const=0;

	protected:
		Program(Videodevice &rVideodevice,const base::String& identifier,const base::String& entrypoint,const base::String& format);

		base::String m_Identifier,m_Entrypoint,m_Format;
		Videodevice& m_rVideodevice;
	};

}
}

#endif
