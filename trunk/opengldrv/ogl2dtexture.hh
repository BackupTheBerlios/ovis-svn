#ifndef OVIS_OPENGLDRV_OGL2DTEXTURE_HH_INCLUDED
#define OVIS_OPENGLDRV_OGL2DTEXTURE_HH_INCLUDED

#include <set>
#include <vector>
#include "ovisgl.hh"
#include "../base/dll.hh"
#include "../math/matrix.hh"
#include "../video/mempool.hh"
#include "../video/pixelformat.hh"
#include "../video/texture2d.hh"
//#include "ogltexture.hh"

namespace ovis {
namespace opengldrv {

	class OGLDevice;

	class OVIS_API OGL2DTexture:public video::Texture2D
	{
	public:
		OGL2DTexture(OGLDevice& ogldevice,const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 width,const ovis_uint32 height,
			const ovis_uint32 levels,const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);
		~OGL2DTexture();
	
		video::Pixelformat actualpixelformat() const;

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();
	
		void transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
			const ovis_uint32 height,const video::RGBALayouts rgbalayout,const video::Channeldatatype channeldatatype);

		ovis_uint32 width() const;
		ovis_uint32 height() const;
		ovis_uint32 numMipmaplevels() const;

		void copyFromFramebuffer(const ovis_uint32 x,const ovis_uint32 y);

		void glbind();
		GLuint handle() const;

		inline void setTexParameteri(const GLenum paramName,const GLint param) {
			glbind();
			glTexParameteri(GL_TEXTURE_2D,paramName,param);
		}
		inline void setTexParameterf(const GLenum paramName,const GLfloat param) {
			glbind();
			glTexParameterf(GL_TEXTURE_2D,paramName,param);
		}

	protected:
		OGLDevice& m_rOGLDevice;
		bool m_IsValid,m_IsDataOK;
		GLuint m_GLHandle;
	
		video::Pixelformat m_ActualPixelformat;
		GLint m_Internalformat;
		ovis_uint32 m_Width,m_Height,m_NumMipmaplevels;
		bool *m_pDataSubmitted;
	};

}
}

#endif
