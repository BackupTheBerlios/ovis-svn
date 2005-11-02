#include "bufobject.hh"
#include "ogldevice.hh"
#include "oglvertexstream.hh"
#include "../video/vertexformat.hh"

namespace ovis {
namespace opengldrv {

	OGLVertexstream::OGLVertexstream(OGLDevice& rOGLDevice,const ovis_uint32 numVertices,
	const video::Vertexformat& format,const ovis_uint32 flags,video::Vertexstream::Vertexsource *pSource):
	Vertexstream(format,numVertices,pSource),
	m_pBuffer(new BufferObject(GL_ARRAY_BUFFER_ARB)),m_rOGLDevice(rOGLDevice),m_IsDataOK(true)
	{
		GLenum usage;
		
		if (flags& video::Streamflags_Dynamic) {
			usage=GL_DYNAMIC_DRAW_ARB;
		} else usage=GL_STATIC_DRAW_ARB;

		m_pBuffer->bind();
		m_pBuffer->create(static_cast<GLsizeiptrARB>(m_Format.stride()*numVertices),0,usage);

		m_Pointers.m_CurrentVtx=0;
		m_Pointers.m_pp1DTexcoords=m_pp1DTexcoords;
		m_Pointers.m_pp2DTexcoords=m_pp2DTexcoords;
		m_Pointers.m_pp3DTexcoords=m_pp3DTexcoords;
	}

	OGLVertexstream::~OGLVertexstream()
	{
		delete m_pBuffer;
	}

	bool OGLVertexstream::isValid() const
	{
		return (m_pBuffer!=0);
	}

	bool OGLVertexstream::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLVertexstream::dataIsOK() { m_IsDataOK=true; }

	void *OGLVertexstream::mappedPointer()
	{
		return m_pBuffer->mappedPointer();
	}

	bool OGLVertexstream::isMapped() const
	{
		return m_pBuffer->mappedPointer()!=0;
	}

	void OGLVertexstream::bind()
	{
		// IMPORTANT: FIRST bind the vbo, THEN set the pointers!
		m_pBuffer->bind();

		ovis_uint8* offset=0;
		int numtexcoord=0;
		int texsize=0;

		for (ovis_uint32 entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			video::VertexFormatEntry vfentry=m_Format.entry(entrynr);

			texsize=0;
			switch (vfentry) {
				case video::VertexFormatEntry_Position:
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(3,GL_FLOAT,m_Format.stride(),(const GLvoid*)offset);
					break;
				case video::VertexFormatEntry_Normal:
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT,m_Format.stride(),(const GLvoid*)offset);
					break;
				case video::VertexFormatEntry_Diffuse:
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4,GL_UNSIGNED_BYTE,m_Format.stride(),(const GLvoid*)offset);
					break;
				case video::VertexFormatEntry_Specular:continue;
					break;
				case video::VertexFormatEntry_Texcoord1D:
					texsize=1;
				case video::VertexFormatEntry_Texcoord2D:
					if (texsize==0) texsize=2;
				case video::VertexFormatEntry_Texcoord3D:
					if (texsize==0) texsize=3;
					glActiveTextureARB(GL_TEXTURE0_ARB+numtexcoord);
					glClientActiveTextureARB(GL_TEXTURE0_ARB+numtexcoord);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(texsize,GL_FLOAT,m_Format.stride(),(const GLvoid*)offset);
					++numtexcoord;
					break;
			}
			offset+=video::vertexFormatEntrySizeLookup(vfentry);
		}
	}

	void OGLVertexstream::map(const ovis_uint32 flags)
	{
		GLenum access=GL_READ_WRITE_ARB;

	/*	enum Mapflags {
			Map_Writeonly=1,
			Map_Readonly=2,
			Map_Discard=4,
			Map_Nooverwrite=8
		};
		
			@param access Access mode. Valid values:\n
		\n
		<b>GL_READ_ONLY_ARB</b>\n
		Read-only access.\n
		\n
		<b>GL_WRITE_ONLY_ARB</b>\n
		Write-only access.\n
		\n
		<b>GL_READ_WRITE_ARB</b>\n
		Read-write access.\n*/

		if (flags&video::Map_Readonly) {
			access=GL_READ_ONLY_ARB;
		} else if (flags&video::Map_Writeonly) {
			access=GL_WRITE_ONLY_ARB;
		}

		m_pBuffer->bind();
		m_pBuffer->map(access);

		calculatePointers(m_Pointers);
	}

	void OGLVertexstream::unmap()
	{
		m_pBuffer->bind();
		m_pBuffer->unmap();
	}

	void OGLVertexstream::calculatePointers(video::VertexiteratorPointers& vitp)
	{
		if ((vitp.m_CurrentVtx>=capacity()) || !isMapped()) return;

		ovis_uint8* pPtr=((ovis_uint8*)m_pBuffer->mappedPointer())+m_Format.stride()*vitp.m_CurrentVtx;
		int numtexcoords[3]={0,0,0};
		for (unsigned long entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			video::VertexFormatEntry entry=m_Format.entry(entrynr);
			switch (entry) {
				case video::VertexFormatEntry_Position:vitp.m_pPosition=(math::Vector3f*)pPtr; break;
				case video::VertexFormatEntry_Normal:vitp.m_pNormal=(math::Vector3f*)pPtr; break;
				case video::VertexFormatEntry_Texcoord1D:vitp.m_pp1DTexcoords[numtexcoords[0]++]=(float*)pPtr; break;
				case video::VertexFormatEntry_Texcoord2D:vitp.m_pp2DTexcoords[numtexcoords[1]++]=(math::Vector2f*)pPtr; break;
				case video::VertexFormatEntry_Texcoord3D:vitp.m_pp3DTexcoords[numtexcoords[2]++]=(math::Vector3f*)pPtr; break;
				case video::VertexFormatEntry_Diffuse:vitp.m_pDiffuseColor=(ovis_uint32*)pPtr;
				case video::VertexFormatEntry_Specular:vitp.m_pSpecularColor=(ovis_uint32*)pPtr;
				default:break;
			}
			pPtr+=video::vertexFormatEntrySizeLookup(entry);
		}
	}

	const math::Vector3f& OGLVertexstream::position(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	const math::Vector3f& OGLVertexstream::normal(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	ovis_uint32 OGLVertexstream::diffuseColor(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		return *((ovis_uint32*)pPtr);
	}

	ovis_uint32 OGLVertexstream::specularColor(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		return *((ovis_uint32*)pPtr);
	}

	float OGLVertexstream::texcoord1D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((float*)pPtr);
	}

	const math::Vector2f& OGLVertexstream::texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector2f*)pPtr);
	}

	const math::Vector3f& OGLVertexstream::texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	void OGLVertexstream::position(const ovis_uint32 vtxindex,const float x,const float y,const float z)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void OGLVertexstream::position(const ovis_uint32 vtxindex,const math::Vector3f& newPosition)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newPosition.x();
		((math::Vector3f*)pPtr)->y()=newPosition.y();
		((math::Vector3f*)pPtr)->z()=newPosition.z();
	}

	void OGLVertexstream::normal(const ovis_uint32 vtxindex,const float x,const float y,const float z)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void OGLVertexstream::normal(const ovis_uint32 vtxindex,const math::Vector3f& newNormal)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newNormal.x();
		((math::Vector3f*)pPtr)->y()=newNormal.y();
		((math::Vector3f*)pPtr)->z()=newNormal.z();
	}

	void OGLVertexstream::diffuseColor(const ovis_uint32 vtxindex,const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ovis_uint32*)pPtr)=(((ovis_uint32)a)<<24)|(((ovis_uint32)b)<<16)|(((ovis_uint32)g)<<8)|((ovis_uint32)r);
	}

	void OGLVertexstream::diffuseColor(const ovis_uint32 vtxindex,const ovis_uint32 color)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		// TODO: Correct the RGBA order
		*((ovis_uint32*)pPtr)=color;
	}

	void OGLVertexstream::specularColor(const ovis_uint32 vtxindex,const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ovis_uint32*)pPtr)=(((ovis_uint32)a)<<24)|(((ovis_uint32)b)<<16)|(((ovis_uint32)g)<<8)|((ovis_uint32)r);
	}

	void OGLVertexstream::specularColor(const ovis_uint32 vtxindex,const ovis_uint32 color)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		// TODO: Correct the RGBA order
		*((ovis_uint32*)pPtr)=color;
	}

	void OGLVertexstream::texcoord1D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float newTexcoord1D)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		*((float*)pPtr)=newTexcoord1D;
	}

	void OGLVertexstream::texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float u,const float v)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
	}

	void OGLVertexstream::texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord2D.x();
		((float*)pPtr)[1]=newTexcoord2D.y();
	}

	void OGLVertexstream::texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float u,const float v,const float w)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
		((float*)pPtr)[2]=w;
	}

	void OGLVertexstream::texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord3D.x();
		((float*)pPtr)[1]=newTexcoord3D.y();
		((float*)pPtr)[2]=newTexcoord3D.z();
	}

}
}
