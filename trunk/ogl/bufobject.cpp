#include "bufobject.hh"

namespace ovis {
namespace opengldrv {
	
	BufferObject::BufferObject(const GLenum bufferType):m_BufferID(0),m_BufferType(bufferType),m_pMappedPointer(0)
	{
		glGenBuffersARB(1,&m_BufferID);
	}

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
