#ifndef OVIS_OPENGLDRV_BUFFEROBJECT_HH_INCLUDED
#define OVIS_OPENGLDRV_BUFFEROBJECT_HH_INCLUDED

#include "ovisgl.hh"

namespace ovis {
namespace opengldrv {
	
//! This class handles OpenGL 1.4 vertex buffer objects.
/*!
A vertex buffer object (VBO) is an OpenGL structure capable of storing
geometry data in video- and AGP-RAM. Hardware T&L performance is excellent
with VBOs, so they are chosen instead of vertex arrays and display lists.
Contrary to its name, a VBO can store both vertices and indices, the first one
being an array buffer, and the latter one an element array buffer.
	
\warning Always call the bind() method at least once before (un)mapping,
creating or deleting the VBO. Otherwise, the wrong VBO might be affected, or an
access violation may occur.
*/
	
class BufferObject
{
protected:
	GLuint m_BufferID;
	GLenum m_BufferType;
	GLvoid *m_pMappedPointer; // If this is zero, then the buffer is not mapped
public:
	//! Constructor. Prepares a VBO for creation.
	/*
	@param bufferType the buffer type:\n
	GL_ARRAY_BUFFER_ARB : array buffer storing vertices. The equivalent of a Direct3D(tm) vertex buffer.\n
	GL_ELEMENT_ARRAY_BUFFER_ARB = element array buffer storing indices. The equivalent of a Direct3D.(tm) index buffer
	*/
	BufferObject(const GLenum bufferType):m_BufferID(0),m_BufferType(bufferType),m_pMappedPointer(0) { glGenBuffersARB(1,&m_BufferID); }

	inline static void disableVBO(const GLenum bufferType) { glBindBufferARB(bufferType,0); }
	
	inline GLuint ID() const { return m_BufferID; }
	
	//! The destructor, automatically deallocates the VBO if not already done.
	~BufferObject() { del(); }
	
	//! Retrieves  the memory-mapped pointer.
	//\warning The returned pointer is only valid in a map()...unmap() block!
	inline GLvoid* mappedPointer() const { return m_pMappedPointer; }

	//! Creates a VBO.
	/*!
	@param size The VBO size, in <b>bytes</b>.
	@param data Pointer to a memory block. If this is a valid pointer, the data
	it points to will be copied to the VBO. If you do not want to send anything
	to the VBO in create(), just set this parameter to 0.
	@param usage This parameter is specified as one of nine enumerated values,
	indicating the expected application usage pattern of the data store.  The
	values are:\n
	\n
    <b>STREAM_DRAW_ARB</b>\n
	The data store contents will be specified once by the application, and used
	at most a few times as the source of a GL (drawing) command.\n
	\n
    <b>STREAM_READ_ARB</b>\n
	The data store contents will be specified once by reading data from the GL,
	and queried at most a few times by the application.\n
	\n
    <b>STREAM_COPY_ARB</b>\n
	The data store contents will be specified once by reading data from the GL,
	and used at most a few times as the source of a GL (drawing) command.\n
	\n
    <b>STATIC_DRAW_ARB</b>\n
	The data store contents will be specified once by the application, and used
	many times as the source for GL (drawing) commands.\n
	\n
    <b>STATIC_READ_ARB</b>\n
	The data store contents will be specified once by reading data from the GL,
	and queried many times by the application. \n
	\n
    <b>STATIC_COPY_ARB</b>\n
	The data store contents will be specified once by reading data from the GL,
	and used many times as the source for GL (drawing) commands.\n
	\n
    <b>DYNAMIC_DRAW_ARB</b>\n
	The data store contents will be respecified repeatedly by the application,
	and used many times as the source for GL (drawing) commands.\n
	\n
    <b>DYNAMIC_READ_ARB</b>\n
	The data store contents will be respecified repeatedly by reading data from
	the GL, and queried many times by the application.\n
	\n
    <b>DYNAMIC_COPY_ARB</b>\n
	The data store contents will be respecified repeatedly by reading data from
	the GL, and used many times as the source for GL (drawing) commands.\n
	
    "usage" is provided as a performance hint only.  The specified usage
    value does not constrain the actual usage pattern of the data store.\n
	\n
	Description for "usage" taken from the SGI VBO documentation. URL:
	http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_buffer_object.txt
	*/
	void create(GLsizeiptrARB size,const GLvoid *data,GLenum usage);
	
	//! Deallocates the VBO if present.
	/*! \warning Do not call this when the VBO is mapped!
	*/
	inline void del() { if (m_BufferID) glDeleteBuffersARB(1,&m_BufferID); m_BufferID=0; }
	
	//! Maps the VBO to a pointer, to enable I/O access.
	/*! This method maps the memory block of the VBO into ovis's memory
	space. This way, reading/writing from/to the VBO is possible. This method
	validates the pointer returned by mappedPointer().\n
	\n
	This is the equivalent to Direct3D(tm) VertexBuffer Lock().
	\warning In mapped mode, drawing from a VBO is not recommended.
	
	@param access Access mode. Valid values:\n
	\n
	<b>GL_READ_ONLY_ARB</b>\n
	Read-only access.\n
	\n
	<b>GL_WRITE_ONLY_ARB</b>\n
	Write-only access.\n
	\n
	<b>GL_READ_WRITE_ARB</b>\n
	Read-write access.\n
	*/
	GLvoid* map(GLenum access);
	
	//! Unmaps the VBO.
	/*! Removes the mapping, and thus re-enables safe drawing from the VBO.
	\warning The pointer returned by mappedPointer() is invalid after calling
	this method.\n
	\n
	This is the equivalent to Direct3D(tm) VertexBuffer Unlock().
	*/
	GLboolean unmap();
	
	//! Binds the VBO to OpenGL.
	/*! This method is absolutely necessary in order to use any of the other
	methods.
	\warning Always call bind() at least once before (un)mapping, creating or
	deleting the VBO. Otherwise, the wrong VBO might be affected, or an access
	violation may occur.
	*/
	inline void bind() { glBindBufferARB(m_BufferType,m_BufferID); }
};

}
}

#endif
