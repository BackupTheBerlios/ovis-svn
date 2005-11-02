#ifndef OVIS_OPENGLDRV_OGLTEXTURE_HH_INCLUDED
#define OVIS_OPENGLDRV_OGLTEXTURE_HH_INCLUDED

#include "ovisgl.hh"

namespace ovis {
namespace opengldrv {

	class OGLDevice;

	class OVIS_API OGLTexture
	{
	protected:
		GLuint m_GLHandle;
		const GLenum m_Target;

		OGLDevice& m_OGLDevice;

	public:
		OGLTexture(const GLenum target,OGLDevice& ogldevice):m_Target(target),m_OGLDevice(ogldevice) { glGenTextures(1,&m_GLHandle); }
		~OGLTexture() { glDeleteTextures(1,&m_GLHandle); }

		inline GLuint handle() const { return m_GLHandle; }

		inline void bind() {
			glBindTexture(m_Target, m_GLHandle);
		}

		//void setAnisotropy(int level);

		inline void setTexParameteri(const GLenum paramName,const GLint param) {
			bind();
			glTexParameteri(m_Target,paramName,param);
		}
		inline void setTexParameterf(const GLenum paramName,const GLfloat param) {
			bind();
			glTexParameterf(m_Target,paramName,param);
		}
	};

}
}

#endif
