#include "../base/log.hh"
#include "ogldevice.hh"
#include "ogl2dtexture.hh"
#include "tooglconvs.hh"

namespace ovis {
namespace opengldrv {

	OGL2DTexture::OGL2DTexture(OGLDevice& ogldevice,const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 width,const ovis_uint32 height,
			const ovis_uint32 levels,const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool):
			video::Texture2D(format,identifier,texturesource),m_rOGLDevice(ogldevice),m_IsValid(true),m_IsDataOK(true)
	{
		glGenTextures(1,&m_GLHandle);
	
		m_Width=width;
		m_Height=height;
		m_ActualPixelformat=format;
		m_NumMipmaplevels=levels;

		m_Internalformat=oglpixelformat(format);
	
		setTexParameteri(GL_TEXTURE_MIN_FILTER,(levels==1) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
		setTexParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if (levels>1) setTexParameteri(GL_TEXTURE_MAX_LEVEL,levels-1);

		if (levels==0) {
			ovis_uint32 length=(width<height) ? width : height;

			if (length<=1) m_NumMipmaplevels=1;
			else {
				m_NumMipmaplevels=((ovis_uint32)( logf((float)length)/logf(2.0f) )) + 1;
			}
		}

		m_pDataSubmitted=new bool[m_NumMipmaplevels];
		for (ovis_uint32 lvl=0;lvl<m_NumMipmaplevels;++lvl) m_pDataSubmitted[lvl]=false;

	}

	OGL2DTexture::~OGL2DTexture()
	{
		delete [] m_pDataSubmitted;
		glDeleteTextures(1,&m_GLHandle);
	}
	
	video::Pixelformat OGL2DTexture::actualpixelformat() const
	{
		return m_ActualPixelformat;
	}

	bool OGL2DTexture::isValid() const
	{
		return m_IsValid;
	}

	bool OGL2DTexture::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGL2DTexture::dataIsOK()
	{
		m_IsDataOK=true;
	}
	
	void OGL2DTexture::transferData(const ovis_uint32 level,const base::Datablock& datablock,const ovis_uint32 pitch,
		const ovis_uint32 height,const video::RGBALayouts rgbalayout,const video::Channeldatatype channeldatatype)
	{
		GLenum channeltype;
		switch (channeldatatype) {
			case video::Channeldatatype_Int8:return;
			case video::Channeldatatype_Uint8:channeltype=GL_UNSIGNED_BYTE; break;
			case video::Channeldatatype_Int16:return;
			case video::Channeldatatype_Uint16:return;
			case video::Channeldatatype_Float16:return;
			case video::Channeldatatype_Float32:return;
			default:channeltype=GL_UNSIGNED_BYTE;
		}
		
		GLenum rgbaformat=GL_RGB;
		switch (rgbalayout) {
			case video::RGBALayout_RGB:rgbaformat=GL_RGB; break;
			case video::RGBALayout_BGR:rgbaformat=GL_BGR; break;
			case video::RGBALayout_RGBA:rgbaformat=GL_RGBA; break;
			case video::RGBALayout_ARGB:rgbaformat=GL_RGBA; break; // TODO: resolve this
			case video::RGBALayout_BGRA:rgbaformat=GL_BGRA; break;
			case video::RGBALayout_ABGR:rgbaformat=GL_BGRA; break; // TODO: resolve this
			default:break;
		}
		
		glbind();
		
		if (!m_pDataSubmitted[currentLevel()]) {
			glTexImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),m_Internalformat,
				m_Width,m_Height,0,rgbaformat,channeltype,datablock.data());
			m_pDataSubmitted[currentLevel()]=true;
		} else {
			glTexSubImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),0,0,m_Width,m_Height,rgbaformat,channeltype,datablock.data());
		}
	
		/*const base::Datablock *pBlock=&datablock;

		// TODO: Support for more formats
		GLenum type=GL_UNSIGNED_BYTE;
		if ((channeldatatype.size()==4) && !(channeldatatype.isInteger())) type=GL_FLOAT;

		GLenum format=GL_RGB;
		switch (pixelfmt) {
			case video::Pixelformat_RGB:format=GL_RGB; break;
			case video::Pixelformat_RGBA:format=GL_RGBA; break;
			case video::Pixelformat_BGR:format=GL_BGR; break;
			case video::Pixelformat_BGRA:format=GL_BGRA; break;
			default:break; // TODO: check other formats
		}

		glbind();

		if (!m_pDataSubmitted[currentLevel()]) {
			glTexImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),m_Internalformat,
				m_Transferwidth,m_Transferheight,0,format,type,pBlock->data());
			m_pDataSubmitted[currentLevel()]=true;
		} else {
			glTexSubImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),0,0,m_Transferwidth,m_Transferheight,
				format,type,pBlock->data());
		}
*/
	}

	ovis_uint32 OGL2DTexture::width() const
	{
		return m_Width;
	}

	ovis_uint32 OGL2DTexture::height() const
	{
		return m_Height;
	}

	ovis_uint32 OGL2DTexture::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void OGL2DTexture::copyFromFramebuffer(const ovis_uint32 x,const ovis_uint32 y)
	{
		glbind();

		if (!m_pDataSubmitted[currentLevel()]) {
			glCopyTexImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),m_Internalformat,x,y,
				m_Width,m_Height,0);
			m_pDataSubmitted[currentLevel()]=true;
		} else {
			glCopyTexSubImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),0,0,x,y,m_Width,m_Height);
		}
	}

	void OGL2DTexture::glbind()
	{
		glBindTexture(GL_TEXTURE_2D, m_GLHandle);
	}
	
	GLuint OGL2DTexture::handle() const
	{
		return m_GLHandle;
	}

}
}
