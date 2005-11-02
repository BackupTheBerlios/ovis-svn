#include "bufobject.hh"

namespace ovis {
namespace opengldrv {

	void BufferObject::create(GLsizeiptrARB size,const GLvoid *data,GLenum usage)
	{
		bind();
		glBufferDataARB(m_BufferType,size,data,usage);
	}

	GLvoid* BufferObject::map(GLenum access)
	{
		m_pMappedPointer=glMapBufferARB(m_BufferType,access);
		return m_pMappedPointer;
	}

	GLboolean BufferObject::unmap()
	{
		m_pMappedPointer=0;
		return glUnmapBufferARB(m_BufferType);
	}

}
}
