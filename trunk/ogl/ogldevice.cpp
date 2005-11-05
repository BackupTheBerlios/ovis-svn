#include <string>
#include "../base/log.hh"
#include "oglindexstream.hh"
#include "ogldevice.hh"
#include "ogl2dtexture.hh"
#include "oglvertexstream.hh"

namespace ovis {
namespace opengldrv {



	void *OGLDevice::getGLExtensionProcAddress(const char *name) const
	{
		void *pProc;
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
		pProc=(void*)wglGetProcAddress(name);
#else

		pProc=(void*)glXGetProcAddress((const GLubyte*)name);
		//	pProc=(void*)glXGetProcAddressARB((const GLubyte*)name);

#endif
		if (!pProc) base::log("OGLDevice::getGLExtensionProcAddress()",base::Warning) << "WARN: Extension proc " << name << " not found\n";
		return pProc;
	}

	void OGLDevice::parseExtensionString()
	{
		std::string extensions((const char*)glGetString(GL_EXTENSIONS));

		std::string token;
		std::string::size_type pos=0,lastpos=0;

		while ((pos=extensions.find_first_of(' ',lastpos))!=std::string::npos) {
			token=extensions.substr(lastpos,pos-lastpos);
			lastpos=pos+1;
			m_pSupportedExtensions->insert(token);
		
			//base::log("OGLDevice::parseExtensionString()",base::Message) << token << " supported\n";
		}
	}

	bool OGLDevice::extensionSupported(const std::string& extensionName) const
	{
		return (m_pSupportedExtensions->find(extensionName)!=
			m_pSupportedExtensions->end());
	}

	void OGLDevice::initializeExtensions()
	{
		initializeOpenGL12();
		initializeARBVertexProgram();
		initializeARBImaging();
		initializeARBMultitexture();
		initializeARBTextureCompression();
		initializeARBOcclusionQuery();
		initializeNVOcclusionQuery();
		initializeVBO();
	}

	void OGLDevice::initializeOpenGL12()
	{
#ifdef OVIS_IMPLEMENT_GL_VERSION_1_2
		glTexImage3D = (glTexImage3DPROC) getGLExtensionProcAddress("glTexImage3D");
		glTexSubImage3D = (glTexSubImage3DPROC) getGLExtensionProcAddress("glTexSubImage3D");
		glCopyTexSubImage3D = (glCopyTexSubImage3DPROC) getGLExtensionProcAddress("glCopyTexSubImage3D");
		glDrawRangeElements = (glDrawRangeElementsPROC) getGLExtensionProcAddress("glDrawRangeElements");
#endif /* GL_VERSION_1_2 */
	}

	void OGLDevice::initializeARBImaging()
	{
		if (!extensionSupported("GL_ARB_imaging")) {
			/*localLogger() << "WARN: No ARB_imaging support detected";*/ return;
		}
	
#ifdef OVIS_IMPLEMENT_GL_ARB_IMAGING
		glBlendColor = (glBlendColorPROC) getGLExtensionProcAddress("glBlendColor");
		glBlendEquation = (glBlendEquationPROC) getGLExtensionProcAddress("glBlendEquation");
		glColorTable = (glColorTablePROC) getGLExtensionProcAddress("glColorTable");
		glColorTableParameterfv = (glColorTableParameterfvPROC) getGLExtensionProcAddress("glColorTableParameterfv");
		glColorTableParameteriv = (glColorTableParameterivPROC) getGLExtensionProcAddress("glColorTableParameteriv");
		glCopyColorTable = (glCopyColorTablePROC) getGLExtensionProcAddress("glCopyColorTable");
		glGetColorTable = (glGetColorTablePROC) getGLExtensionProcAddress("glGetColorTable");
		glGetColorTableParameterfv = (glGetColorTableParameterfvPROC) getGLExtensionProcAddress("glGetColorTableParameterfv");
		glGetColorTableParameteriv = (glGetColorTableParameterivPROC) getGLExtensionProcAddress("glGetColorTableParameteriv");
		glColorSubTable = (glColorSubTablePROC) getGLExtensionProcAddress("glColorSubTable");
		glCopyColorSubTable = (glCopyColorSubTablePROC) getGLExtensionProcAddress("glCopyColorSubTable");
		glConvolutionFilter1D = (glConvolutionFilter1DPROC) getGLExtensionProcAddress("glConvolutionFilter1D");
		glConvolutionFilter2D = (glConvolutionFilter2DPROC) getGLExtensionProcAddress("glConvolutionFilter2D");
		glConvolutionParameterf = (glConvolutionParameterfPROC) getGLExtensionProcAddress("glConvolutionParameterf");
		glConvolutionParameterfv = (glConvolutionParameterfvPROC) getGLExtensionProcAddress("glConvolutionParameterfv");
		glConvolutionParameteri = (glConvolutionParameteriPROC) getGLExtensionProcAddress("glConvolutionParameteri");
		glConvolutionParameteriv = (glConvolutionParameterivPROC) getGLExtensionProcAddress("glConvolutionParameteriv");
		glCopyConvolutionFilter1D = (glCopyConvolutionFilter1DPROC) getGLExtensionProcAddress("glCopyConvolutionFilter1D");
		glCopyConvolutionFilter2D = (glCopyConvolutionFilter2DPROC) getGLExtensionProcAddress("glCopyConvolutionFilter2D");
		glGetConvolutionFilter = (glGetConvolutionFilterPROC) getGLExtensionProcAddress("glGetConvolutionFilter");
		glGetConvolutionParameterfv = (glGetConvolutionParameterfvPROC) getGLExtensionProcAddress("glGetConvolutionParameterfv");
		glGetConvolutionParameteriv = (glGetConvolutionParameterivPROC) getGLExtensionProcAddress("glGetConvolutionParameteriv");
		glGetSeparableFilter = (glGetSeparableFilterPROC) getGLExtensionProcAddress("glGetSeparableFilter");
		glSeparableFilter2D = (glSeparableFilter2DPROC) getGLExtensionProcAddress("glSeparableFilter2D");
		glGetHistogram = (glGetHistogramPROC) getGLExtensionProcAddress("glGetHistogram");
		glGetHistogramParameterfv = (glGetHistogramParameterfvPROC) getGLExtensionProcAddress("glGetHistogramParameterfv");
		glGetHistogramParameteriv = (glGetHistogramParameterivPROC) getGLExtensionProcAddress("glGetHistogramParameteriv");
		glGetMinmax = (glGetMinmaxPROC) getGLExtensionProcAddress("glGetMinmax");
		glGetMinmaxParameterfv = (glGetMinmaxParameterfvPROC) getGLExtensionProcAddress("glGetMinmaxParameterfv");
		glGetMinmaxParameteriv = (glGetMinmaxParameterivPROC) getGLExtensionProcAddress("glGetMinmaxParameteriv");
		glHistogram = (glHistogramPROC) getGLExtensionProcAddress("glHistogram");
		glMinmax = (glMinmaxPROC) getGLExtensionProcAddress("glMinmax");
		glResetHistogram = (glResetHistogramPROC) getGLExtensionProcAddress("glResetHistogram");
		glResetMinmax = (glResetMinmaxPROC) getGLExtensionProcAddress("glResetMinmax");
#endif /* GL_ARB_imaging */
	}

	void OGLDevice::initializeARBMultitexture()
	{
		if (!extensionSupported("GL_ARB_multitexture")) {
			//localLogger() << "WARN: No ARB multitexture support detected";
			return;
		}
	
#ifdef OVIS_IMPLEMENT_GL_ARB_MULTITEXTURE
		glActiveTextureARB = (glActiveTextureARBPROC) getGLExtensionProcAddress("glActiveTextureARB");
		glClientActiveTextureARB = (glClientActiveTextureARBPROC) getGLExtensionProcAddress("glClientActiveTextureARB");

		glMultiTexCoord1dARB = (glMultiTexCoord1dARBPROC) getGLExtensionProcAddress("glMultiTexCoord1dARB");
		glMultiTexCoord1dvARB = (glMultiTexCoord1dvARBPROC) getGLExtensionProcAddress("glMultiTexCoord1dvARB");
		glMultiTexCoord1fARB = (glMultiTexCoord1fARBPROC) getGLExtensionProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord1fvARB = (glMultiTexCoord1fvARBPROC) getGLExtensionProcAddress("glMultiTexCoord1fvARB");
		glMultiTexCoord1iARB = (glMultiTexCoord1iARBPROC) getGLExtensionProcAddress("glMultiTexCoord1iARB");
		glMultiTexCoord1ivARB = (glMultiTexCoord1ivARBPROC) getGLExtensionProcAddress("glMultiTexCoord1ivARB");
		glMultiTexCoord1sARB = (glMultiTexCoord1sARBPROC) getGLExtensionProcAddress("glMultiTexCoord1sARB");
		glMultiTexCoord1svARB = (glMultiTexCoord1svARBPROC) getGLExtensionProcAddress("glMultiTexCoord1svARB");

		glMultiTexCoord2dARB = (glMultiTexCoord2dARBPROC) getGLExtensionProcAddress("glMultiTexCoord2dARB");
		glMultiTexCoord2dvARB = (glMultiTexCoord2dvARBPROC) getGLExtensionProcAddress("glMultiTexCoord2dvARB");
		glMultiTexCoord2fARB = (glMultiTexCoord2fARBPROC) getGLExtensionProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord2fvARB = (glMultiTexCoord2fvARBPROC) getGLExtensionProcAddress("glMultiTexCoord2fvARB");
		glMultiTexCoord2iARB = (glMultiTexCoord2iARBPROC) getGLExtensionProcAddress("glMultiTexCoord2iARB");
		glMultiTexCoord2ivARB = (glMultiTexCoord2ivARBPROC) getGLExtensionProcAddress("glMultiTexCoord2ivARB");
		glMultiTexCoord2sARB = (glMultiTexCoord2sARBPROC) getGLExtensionProcAddress("glMultiTexCoord2sARB");
		glMultiTexCoord2svARB = (glMultiTexCoord2svARBPROC) getGLExtensionProcAddress("glMultiTexCoord2svARB");

		glMultiTexCoord3dARB = (glMultiTexCoord3dARBPROC) getGLExtensionProcAddress("glMultiTexCoord3dARB");
		glMultiTexCoord3dvARB = (glMultiTexCoord3dvARBPROC) getGLExtensionProcAddress("glMultiTexCoord3dvARB");
		glMultiTexCoord3fARB = (glMultiTexCoord3fARBPROC) getGLExtensionProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord3fvARB = (glMultiTexCoord3fvARBPROC) getGLExtensionProcAddress("glMultiTexCoord3fvARB");
		glMultiTexCoord3iARB = (glMultiTexCoord3iARBPROC) getGLExtensionProcAddress("glMultiTexCoord3iARB");
		glMultiTexCoord3ivARB = (glMultiTexCoord3ivARBPROC) getGLExtensionProcAddress("glMultiTexCoord3ivARB");
		glMultiTexCoord3sARB = (glMultiTexCoord3sARBPROC) getGLExtensionProcAddress("glMultiTexCoord3sARB");
		glMultiTexCoord3svARB = (glMultiTexCoord3svARBPROC) getGLExtensionProcAddress("glMultiTexCoord3svARB");

		glMultiTexCoord4dARB = (glMultiTexCoord4dARBPROC) getGLExtensionProcAddress("glMultiTexCoord4dARB");
		glMultiTexCoord4dvARB = (glMultiTexCoord4dvARBPROC) getGLExtensionProcAddress("glMultiTexCoord4dvARB");
		glMultiTexCoord4fARB = (glMultiTexCoord4fARBPROC) getGLExtensionProcAddress("glMultiTexCoord4fARB");
		glMultiTexCoord4fvARB = (glMultiTexCoord4fvARBPROC) getGLExtensionProcAddress("glMultiTexCoord4fvARB");
		glMultiTexCoord4iARB = (glMultiTexCoord4iARBPROC) getGLExtensionProcAddress("glMultiTexCoord4iARB");
		glMultiTexCoord4ivARB = (glMultiTexCoord4ivARBPROC) getGLExtensionProcAddress("glMultiTexCoord4ivARB");
		glMultiTexCoord4sARB = (glMultiTexCoord4sARBPROC) getGLExtensionProcAddress("glMultiTexCoord4sARB");
		glMultiTexCoord4svARB = (glMultiTexCoord4svARBPROC) getGLExtensionProcAddress("glMultiTexCoord4svARB");
#endif /* GL_ARB_multitexture */
	}

	void OGLDevice::initializeARBVertexProgram()
	{
		if (!extensionSupported("GL_ARB_vertex_program")) {
			/*localLogger() << "WARN: No ARB vertex program support detected";*/ return;
		}

#ifdef OVIS_IMPLEMENT_GL_ARB_VERTEX_PROGRAM
		glVertexAttrib1sARB = (glVertexAttrib1sARBPROC) getGLExtensionProcAddress("glVertexAttrib1sARB");
		glVertexAttrib1fARB = (glVertexAttrib1fARBPROC) getGLExtensionProcAddress("glVertexAttrib1fARB");
		glVertexAttrib1dARB = (glVertexAttrib1dARBPROC) getGLExtensionProcAddress("glVertexAttrib1dARB");
		glVertexAttrib2sARB = (glVertexAttrib2sARBPROC) getGLExtensionProcAddress("glVertexAttrib2sARB");
		glVertexAttrib2fARB = (glVertexAttrib2fARBPROC) getGLExtensionProcAddress("glVertexAttrib2fARB");
		glVertexAttrib2dARB = (glVertexAttrib2dARBPROC) getGLExtensionProcAddress("glVertexAttrib2dARB");
		glVertexAttrib3sARB = (glVertexAttrib3sARBPROC) getGLExtensionProcAddress("glVertexAttrib3sARB");
		glVertexAttrib3fARB = (glVertexAttrib3fARBPROC) getGLExtensionProcAddress("glVertexAttrib3fARB");
		glVertexAttrib3dARB = (glVertexAttrib3dARBPROC) getGLExtensionProcAddress("glVertexAttrib3dARB");
		glVertexAttrib4sARB = (glVertexAttrib4sARBPROC) getGLExtensionProcAddress("glVertexAttrib4sARB");
		glVertexAttrib4fARB = (glVertexAttrib4fARBPROC) getGLExtensionProcAddress("glVertexAttrib4fARB");
		glVertexAttrib4dARB = (glVertexAttrib4dARBPROC) getGLExtensionProcAddress("glVertexAttrib4dARB");
		glVertexAttrib4NubARB = (glVertexAttrib4NubARBPROC) getGLExtensionProcAddress("glVertexAttrib4NubARB");
		glVertexAttrib1svARB = (glVertexAttrib1svARBPROC) getGLExtensionProcAddress("glVertexAttrib1svARB");
		glVertexAttrib1fvARB = (glVertexAttrib1fvARBPROC) getGLExtensionProcAddress("glVertexAttrib1fvARB");
		glVertexAttrib1dvARB = (glVertexAttrib1dvARBPROC) getGLExtensionProcAddress("glVertexAttrib1dvARB");
		glVertexAttrib2svARB = (glVertexAttrib2svARBPROC) getGLExtensionProcAddress("glVertexAttrib2svARB");
		glVertexAttrib2fvARB = (glVertexAttrib2fvARBPROC) getGLExtensionProcAddress("glVertexAttrib2fvARB");
		glVertexAttrib2dvARB = (glVertexAttrib2dvARBPROC) getGLExtensionProcAddress("glVertexAttrib2dvARB");
		glVertexAttrib3svARB = (glVertexAttrib3svARBPROC) getGLExtensionProcAddress("glVertexAttrib3svARB");
		glVertexAttrib3fvARB = (glVertexAttrib3fvARBPROC) getGLExtensionProcAddress("glVertexAttrib3fvARB");
		glVertexAttrib3dvARB = (glVertexAttrib3dvARBPROC) getGLExtensionProcAddress("glVertexAttrib3dvARB");
		glVertexAttrib4bvARB = (glVertexAttrib4bvARBPROC) getGLExtensionProcAddress("glVertexAttrib4bvARB");
		glVertexAttrib4svARB = (glVertexAttrib4svARBPROC) getGLExtensionProcAddress("glVertexAttrib4svARB");
		glVertexAttrib4ivARB = (glVertexAttrib4ivARBPROC) getGLExtensionProcAddress("glVertexAttrib4ivARB");
		glVertexAttrib4ubvARB = (glVertexAttrib4ubvARBPROC) getGLExtensionProcAddress("glVertexAttrib4ubvARB");
		glVertexAttrib4usvARB = (glVertexAttrib4usvARBPROC) getGLExtensionProcAddress("glVertexAttrib4usvARB");
		glVertexAttrib4uivARB = (glVertexAttrib4uivARBPROC) getGLExtensionProcAddress("glVertexAttrib4uivARB");
		glVertexAttrib4fvARB = (glVertexAttrib4fvARBPROC) getGLExtensionProcAddress("glVertexAttrib4fvARB");
		glVertexAttrib4dvARB = (glVertexAttrib4dvARBPROC) getGLExtensionProcAddress("glVertexAttrib4dvARB");
		glVertexAttrib4NbvARB = (glVertexAttrib4NbvARBPROC) getGLExtensionProcAddress("glVertexAttrib4NbvARB");
		glVertexAttrib4NsvARB = (glVertexAttrib4NsvARBPROC) getGLExtensionProcAddress("glVertexAttrib4NsvARB");
		glVertexAttrib4NivARB = (glVertexAttrib4NivARBPROC) getGLExtensionProcAddress("glVertexAttrib4NivARB");
		glVertexAttrib4NubvARB = (glVertexAttrib4NubvARBPROC) getGLExtensionProcAddress("glVertexAttrib4NubvARB");
		glVertexAttrib4NusvARB = (glVertexAttrib4NusvARBPROC) getGLExtensionProcAddress("glVertexAttrib4NusvARB");
		glVertexAttrib4NuivARB = (glVertexAttrib4NuivARBPROC) getGLExtensionProcAddress("glVertexAttrib4NuivARB");
		glVertexAttribPointerARB = (glVertexAttribPointerARBPROC) getGLExtensionProcAddress("glVertexAttribPointerARB");
		glEnableVertexAttribArrayARB = (glEnableVertexAttribArrayARBPROC) getGLExtensionProcAddress("glEnableVertexAttribArrayARB");
		glDisableVertexAttribArrayARB = (glDisableVertexAttribArrayARBPROC) getGLExtensionProcAddress("glDisableVertexAttribArrayARB");
		glProgramStringARB = (glProgramStringARBPROC) getGLExtensionProcAddress("glProgramStringARB");
		glBindProgramARB = (glBindProgramARBPROC) getGLExtensionProcAddress("glBindProgramARB");
		glDeleteProgramsARB = (glDeleteProgramsARBPROC) getGLExtensionProcAddress("glDeleteProgramsARB");
		glGenProgramsARB = (glGenProgramsARBPROC) getGLExtensionProcAddress("glGenProgramsARB");
		glProgramEnvParameter4dARB = (glProgramEnvParameter4dARBPROC) getGLExtensionProcAddress("glProgramEnvParameter4dARB");
		glProgramEnvParameter4dvARB = (glProgramEnvParameter4dvARBPROC) getGLExtensionProcAddress("glProgramEnvParameter4dvARB");
		glProgramEnvParameter4fARB = (glProgramEnvParameter4fARBPROC) getGLExtensionProcAddress("glProgramEnvParameter4fARB");
		glProgramEnvParameter4fvARB = (glProgramEnvParameter4fvARBPROC) getGLExtensionProcAddress("glProgramEnvParameter4fvARB");
		glProgramLocalParameter4dARB = (glProgramLocalParameter4dARBPROC) getGLExtensionProcAddress("glProgramLocalParameter4dARB");
		glProgramLocalParameter4dvARB = (glProgramLocalParameter4dvARBPROC) getGLExtensionProcAddress("glProgramLocalParameter4dvARB");
		glProgramLocalParameter4fARB = (glProgramLocalParameter4fARBPROC) getGLExtensionProcAddress("glProgramLocalParameter4fARB");
		glProgramLocalParameter4fvARB = (glProgramLocalParameter4fvARBPROC) getGLExtensionProcAddress("glProgramLocalParameter4fvARB");
		glGetProgramEnvParameterdvARB = (glGetProgramEnvParameterdvARBPROC) getGLExtensionProcAddress("glGetProgramEnvParameterdvARB");
		glGetProgramEnvParameterfvARB = (glGetProgramEnvParameterfvARBPROC) getGLExtensionProcAddress("glGetProgramEnvParameterfvARB");
		glGetProgramLocalParameterdvARB = (glGetProgramLocalParameterdvARBPROC) getGLExtensionProcAddress("glGetProgramLocalParameterdvARB");
		glGetProgramLocalParameterfvARB = (glGetProgramLocalParameterfvARBPROC) getGLExtensionProcAddress("glGetProgramLocalParameterfvARB");
		glGetProgramivARB = (glGetProgramivARBPROC) getGLExtensionProcAddress("glGetProgramivARB");
		glGetProgramStringARB = (glGetProgramStringARBPROC) getGLExtensionProcAddress("glGetProgramStringARB");
		glGetVertexAttribdvARB = (glGetVertexAttribdvARBPROC) getGLExtensionProcAddress("glGetVertexAttribdvARB");
		glGetVertexAttribfvARB = (glGetVertexAttribfvARBPROC) getGLExtensionProcAddress("glGetVertexAttribfvARB");
		glGetVertexAttribivARB = (glGetVertexAttribivARBPROC) getGLExtensionProcAddress("glGetVertexAttribivARB");
		glGetVertexAttribPointervARB = (glGetVertexAttribPointervARBPROC) getGLExtensionProcAddress("glGetVertexAttribPointervARB");
		glIsProgramARB = (glIsProgramARBPROC) getGLExtensionProcAddress("glIsProgramARB");
#endif
	}

	void OGLDevice::initializeARBTextureCompression()
	{
		if (!extensionSupported("GL_ARB_texture_compression")) {
			/*localLogger() << "WARN: No ARB texture compression support detected";*/ return;
		}

#ifdef OVIS_IMPLEMENT_GL_ARB_TEXTURE_COMPRESSION
		glCompressedTexImage3DARB = (glCompressedTexImage3DARBPROC) getGLExtensionProcAddress("glCompressedTexImage3DARB");
		glCompressedTexImage2DARB = (glCompressedTexImage2DARBPROC) getGLExtensionProcAddress("glCompressedTexImage2DARB");
		glCompressedTexImage1DARB = (glCompressedTexImage1DARBPROC) getGLExtensionProcAddress("glCompressedTexImage1DARB");
		glCompressedTexSubImage3DARB = (glCompressedTexSubImage3DARBPROC) getGLExtensionProcAddress("glCompressedTexSubImage3DARB");
		glCompressedTexSubImage2DARB = (glCompressedTexSubImage2DARBPROC) getGLExtensionProcAddress("glCompressedTexSubImage2DARB");
		glCompressedTexSubImage1DARB = (glCompressedTexSubImage1DARBPROC) getGLExtensionProcAddress("glCompressedTexSubImage1DARB");
		glGetCompressedTexImageARB = (glGetCompressedTexImageARBPROC) getGLExtensionProcAddress("glGetCompressedTexImageARB");
#endif
	}

	void OGLDevice::initializeVBO()
	{
		if (!extensionSupported("GL_ARB_vertex_buffer_object")) {
			//localLogger() << "WARN: No vertex buffer object support detected";
			return;
		}

#ifdef OVIS_IMPLEMENT_GL_ARB_VERTEX_BUFFER_OBJECT
		glBindBufferARB = (glBindBufferARBPROC) getGLExtensionProcAddress("glBindBufferARB");
		glDeleteBuffersARB = (glDeleteBuffersARBPROC) getGLExtensionProcAddress("glDeleteBuffersARB");
		glGenBuffersARB = (glGenBuffersARBPROC) getGLExtensionProcAddress("glGenBuffersARB");
		glIsBufferARB = (glIsBufferARBPROC) getGLExtensionProcAddress("glIsBufferARB");
		glBufferDataARB = (glBufferDataARBPROC) getGLExtensionProcAddress("glBufferDataARB");
		glBufferSubDataARB = (glBufferSubDataARBPROC) getGLExtensionProcAddress("glBufferSubDataARB");
		glGetBufferSubDataARB = (glGetBufferSubDataARBPROC) getGLExtensionProcAddress("glGetBufferSubDataARB");
		glMapBufferARB = (glMapBufferARBPROC) getGLExtensionProcAddress("glMapBufferARB");
		glUnmapBufferARB = (glUnmapBufferARBPROC) getGLExtensionProcAddress("glUnmapBufferARB");
		glGetBufferParameterivARB = (glGetBufferParameterivARBPROC) getGLExtensionProcAddress("glGetBufferParameterivARB");
		glGetBufferPointervARB = (glGetBufferPointervARBPROC) getGLExtensionProcAddress("glGetBufferPointervARB");
#endif /* GL_ARB_vertex_buffer_object */
	}

	void OGLDevice::initializeARBOcclusionQuery()
	{
		if (!extensionSupported("GL_ARB_occlusion_query")) {
			/*localLogger() << "WARN: No ARB occlusion query support detected";*/ return;
		}

#ifdef OVIS_IMPLEMENT_GL_ARB_OCCLUSION_QUERY
		glGenQueriesARB = (glGenQueriesARBPROC) getGLExtensionProcAddress("glGenQueriesARB");
		glDeleteQueriesARB = (glDeleteQueriesARBPROC) getGLExtensionProcAddress("glDeleteQueriesARB");
		glIsQueryARB = (glIsQueryARBPROC) getGLExtensionProcAddress("glIsQueryARB");
		glBeginQueryARB = (glBeginQueryARBPROC) getGLExtensionProcAddress("glBeginQueryARB");
		glEndQueryARB = (glEndQueryARBPROC) getGLExtensionProcAddress("glEndQueryARB");
		glGetQueryivARB = (glGetQueryivARBPROC) getGLExtensionProcAddress("glGetQueryivARB");
		glGetQueryObjectivARB = (glGetQueryObjectivARBPROC) getGLExtensionProcAddress("glGetQueryObjectivARB");
		glGetQueryObjectuivARB = (glGetQueryObjectuivARBPROC) getGLExtensionProcAddress("glGetQueryObjectuivARB");
#endif
	}

	void OGLDevice::initializeNVOcclusionQuery()
	{
		if (!extensionSupported("GL_NV_occlusion_query")) {
			/*localLogger() << "WARN: No NV occlusion query support detected";*/ return;
		}

#ifdef OVIS_IMPLEMENT_GL_NV_OCCLUSION_QUERY
		glGenOcclusionQueriesNV = (glGenOcclusionQueriesNVPROC) getGLExtensionProcAddress("glGenOcclusionQueriesNV");
		glDeleteOcclusionQueriesNV = (glDeleteOcclusionQueriesNVPROC) getGLExtensionProcAddress("glDeleteOcclusionQueriesNV");
		glIsOcclusionQueryNV = (glIsOcclusionQueryNVPROC) getGLExtensionProcAddress("glIsOcclusionQueryNV");
		glBeginOcclusionQueryNV = (glBeginOcclusionQueryNVPROC) getGLExtensionProcAddress("glBeginOcclusionQueryNV");
		glEndOcclusionQueryNV = (glEndOcclusionQueryNVPROC) getGLExtensionProcAddress("glEndOcclusionQueryNV");
		glGetOcclusionQueryivNV = (glGetOcclusionQueryivNVPROC) getGLExtensionProcAddress("glGetOcclusionQueryivNV");
		glGetOcclusionQueryuivNV = (glGetOcclusionQueryuivNVPROC) getGLExtensionProcAddress("glGetOcclusionQueryuivNV");
#endif
	}





	OGLDevice::OGLDevice():m_IsValid(false),m_pSupportedExtensions(0),m_pOGLInternaldata(0)
	{
		m_pSupportedExtensions=new std::set <std::string>;
	}

	OGLDevice::~OGLDevice()
	{
		shutdownGL();
		if (m_pSupportedExtensions) delete m_pSupportedExtensions;
	}

	bool OGLDevice::isValid() const
	{
		return m_IsValid;
	}

	//*********** INITIALIZATION / ENUMERATION

	const video::Caps& OGLDevice::caps() const
	{
		return m_Caps;
	}

	ovis_uint32 OGLDevice::numVideoadapters() const
	{
		return 0;
	}

	ovis_uint32 OGLDevice::numVideomodes(const ovis_uint32 adapterindex,const video::Pixelformat pixelformat) const
	{
		return 0;
	}

	bool OGLDevice::fetchVideomode(const ovis_uint32 adapter,const ovis_uint32 modeindex,
		video::Videomode& mode,const video::Pixelformat pixelformat) const
	{
		return false;
	}

	void OGLDevice::init(base::Window* pWindow,const ovis_uint32 adapter,
		const video::Pixelformat colorbufferformat,
		const video::Pixelformat depthstencilformat,const ovis_uint32 Hz)
	{
		initGL(pWindow,adapter,colorbufferformat,depthstencilformat,Hz);

		parseExtensionString();
		initializeExtensions();

		glViewport(0,0,(GLint)(pWindow->width()),(GLint)(pWindow->height()));
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glFrontFace(GL_CCW);


		if (extensionSupported("GL_EXT_texture_filter_anisotropic")) {
			GLint maxanisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&maxanisotropy);
			m_Caps.m_MaxAnisotropy=static_cast<ovis_uint32>(maxanisotropy);
		}

		m_Caps.m_NPOTTextures=m_Caps.m_RectangularTextures=extensionSupported("GL_ARB_texture_non_power_of_two");

		m_Caps.m_Cubemaps=extensionSupported("GL_ARB_texture_cube_map");
		m_Caps.m_CompressedTextures=extensionSupported("GL_ARB_texture_compression");
		m_Caps.m_HWOcclusionQueries=extensionSupported("GL_ARB_occlusion_query") ||
			extensionSupported("GL_NV_occlusion_query");

		m_Caps.m_MaxActiveFFLights=fixedMaxLights();
		m_Caps.m_MaxTextureBlendStages=fixedMaxTextureStages();


		m_Displaywidth=pWindow->width();
		m_Displayheight=pWindow->height();
		m_ColorbufferFormat=colorbufferformat;
		m_DepthstencilbufferFormat=depthstencilformat;

	}

	bool OGLDevice::testVertexProgram(const base::String& program,const base::String& format)
	{
		return false;
	}

	bool OGLDevice::testFragmentProgram(const base::String& program,const base::String& format)
	{
		return false;
	}

	ovis_uint32 OGLDevice::displaywidth() const
	{
		return m_Displaywidth;
	}

	ovis_uint32 OGLDevice::displayheight() const
	{
		return m_Displayheight;
	}

	video::Pixelformat OGLDevice::colorbufferFormat() const
	{
		return m_ColorbufferFormat;
	}

	video::Pixelformat OGLDevice::depthstencilbufferFormat() const
	{
		return m_DepthstencilbufferFormat;
	}





		//*********** FRAME START / END / UPDATE
	void OGLDevice::clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue,
		const ovis_uint32 stencilvalue)
	{
		GLbitfield mask=0;

		if (buffers & video::Clear_Colorbuffer) {
			glClearColor(color[1],color[2],color[3],color[0]);
			mask|=GL_COLOR_BUFFER_BIT;
		}
		if (buffers & video::Clear_Depthbuffer) {
			glClearDepth(zvalue);
			mask|=GL_DEPTH_BUFFER_BIT;
		}
		if (buffers & video::Clear_Stencilbuffer) {
			glClearStencil((GLint)stencilvalue);
			mask|=GL_STENCIL_BUFFER_BIT;
		}

		glClear(mask);
	}

	void OGLDevice::begin()
	{
	}

	void OGLDevice::end()
	{
	}

	void OGLDevice::updateScreen()
	{
		updateGL();
	}

	bool OGLDevice::resizeViewport(const ovis_uint32 newx,const ovis_uint32 newy,
		const ovis_uint32 newwidth,const ovis_uint32 newheight,video::ViewportResizingCallback* callback)
	{
		return false;
	}





	//*********** RESOURCE GENERATION

	video::Vertexstream* OGLDevice::createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,
		const ovis_uint32 flags,const video::Memorypool pool)
	{
		return new OGLVertexstream(*this,numVertices,format,flags);
	}

	video::Indexstream* OGLDevice::createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,
		const ovis_uint32 flags,const video::Memorypool pool)
	{
		return new OGLIndexstream(*this,numIndices,format,flags);
	}

	video::Texture2D* OGLDevice::create2DTexture(const base::String& identifier,const base::String& texturesource,
		const ovis_uint32 width,const ovis_uint32 height,const ovis_uint32 levels,
		const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return new OGL2DTexture(*this,identifier,texturesource,width,height,levels,flags,format,pool);
	}

	video::Cubemap* OGLDevice::createCubemap(const base::String& identifier,const base::String& texturesource,
		const ovis_uint32 edgelength,const ovis_uint32 levels,
		const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool)
	{
		return 0;
	}

	video::Occlusionquery* OGLDevice::createOcclusionquery()
	{
		return 0;
	}

	video::Program* OGLDevice::createVertexProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
		return 0;
	}

	video::Program* OGLDevice::createFragmentProgram(const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format)
	{
		return 0;
	}





	//*********** PRIMITIVES DRAWING

	void OGLDevice::drawPrimitives(const video::PrimitivesType type,
		video::Vertexstream& vertexstream,const ovis_uint32 firstElement,
		const ovis_uint32 numElements)
	{
		GLenum ptype;
		ovis_uint32 vtxfactor;
		switch (type) {
			case video::Primitives_Points: vtxfactor=1; ptype=GL_POINTS; break;
			case video::Primitives_Linestrip: vtxfactor=1; ptype=GL_LINE_STRIP; break;
			case video::Primitives_Lines: vtxfactor=1; ptype=GL_LINES; break;
			case video::Primitives_Trianglestrip: vtxfactor=1; ptype=GL_TRIANGLE_STRIP; break;
			case video::Primitives_Trianglefan: vtxfactor=1; ptype=GL_TRIANGLE_FAN; break;
			case video::Primitives_Triangles: vtxfactor=3; ptype=GL_TRIANGLES; break;
		}

		vertexstream.bind();

		glDrawArrays(ptype,firstElement,numElements*vtxfactor);
	}

	void OGLDevice::drawIndexedPrimitives(
		const video::PrimitivesType type,
		video::Vertexstream& vertexstream,
		video::Indexstream& indexstream,
		const ovis_uint32 indexOffset,
		const ovis_uint32 numElements)
	{
		GLenum ptype;
		ovis_uint32 vtxfactor;
		switch (type) {
			case video::Primitives_Points: vtxfactor=1; ptype=GL_POINTS; break;
			case video::Primitives_Linestrip: vtxfactor=1; ptype=GL_LINE_STRIP; break;
			case video::Primitives_Lines: vtxfactor=1; ptype=GL_LINES; break;
			case video::Primitives_Trianglestrip: vtxfactor=1; ptype=GL_TRIANGLE_STRIP; break;
			case video::Primitives_Trianglefan: vtxfactor=1; ptype=GL_TRIANGLE_FAN; break;
			case video::Primitives_Triangles: vtxfactor=3; ptype=GL_TRIANGLES; break;
		}

		vertexstream.bind();
		indexstream.bind();

		glDrawElements(ptype,numElements*vtxfactor,
			(indexstream.indexformat()==video::Indexformat_16bit) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
#ifdef _MSC_VER
#pragma warning(disable:4312)
#endif
			(const GLvoid*)(indexformatSizeLookup(indexstream.indexformat())*indexOffset));
#ifdef _MSC_VER
#pragma warning(default:4312)
#endif
	}




	//*********** STATE SETTING

	void OGLDevice::enableCap(const GLenum cap,const bool state)
	{
		if (state) glEnable(cap); else glDisable(cap);
	}

	// hw blending. default: false
	void OGLDevice::blending(const bool state)
	{
		enableCap(GL_BLEND,state);
	}

	// handles zbuffer (both test & writing). default: true
	void OGLDevice::zbuffer(const bool state)
	{
		enableCap(GL_DEPTH_TEST,state);
	}

	// z-writing. default: true
	void OGLDevice::zwrite(const bool state)
	{
		glDepthMask(state ? GL_TRUE : GL_FALSE);
	}

	// handles stencil buffer (both test & writing). default: false
	void OGLDevice::stencilbuffer(const bool state)
	{
		enableCap(GL_STENCIL_TEST,state);
	}

	// stencil writing. default: 0xFFFFFFFF
	void OGLDevice::stencilwrite(const ovis_uint32 stencilmask)
	{
		glStencilMask(stencilmask);
	}

	// backface culling. default: true
	void OGLDevice::culling(const bool state)
	{
		enableCap(GL_CULL_FACE,state);
	}

	void OGLDevice::cullmode(const video::Cullingmode mode)
	{
		switch (mode) {
			case video::Cullingmode_Clockwise:glCullFace(GL_BACK); break;
			case video::Cullingmode_Counterclockwise:glCullFace(GL_FRONT); break;
		}
	}

	// blending function. default: one,zero
	void OGLDevice::blendfunc(const video::BlendFunc srcblend,const video::BlendFunc destblend)
	{
		ovis_uint32 blendfactors[2]={srcblend,destblend};
		GLenum factor[2];

		for (int i=0;i<2;++i) {
			switch (blendfactors[i]) {
				case video::BlendFunc_Zero:factor[i]=GL_ZERO; break;
				case video::BlendFunc_One:factor[i]=GL_ONE; break;
				case video::BlendFunc_Src_Color:factor[i]=GL_SRC_COLOR; break;
				case video::BlendFunc_One_Minus_Src_Color:factor[i]=GL_ONE_MINUS_SRC_COLOR; break;
				case video::BlendFunc_Dest_Color:factor[i]=GL_DST_COLOR; break;
				case video::BlendFunc_One_Minus_Dest_Color:factor[i]=GL_ONE_MINUS_DST_COLOR; break;
				case video::BlendFunc_Src_Alpha:factor[i]=GL_SRC_ALPHA; break;
				case video::BlendFunc_One_Minus_Src_Alpha:factor[i]=GL_ONE_MINUS_SRC_ALPHA; break;
				case video::BlendFunc_Dest_Alpha:factor[i]=GL_DST_ALPHA; break;
				case video::BlendFunc_One_Minus_Dest_Alpha:factor[i]=GL_ONE_MINUS_DST_ALPHA; break;
			}
		}

		glBlendFunc(factor[0],factor[1]);
	}

	// z test function. default: lessequal
	void OGLDevice::zfunc(const video::CompareFunc func)
	{
		GLenum glfunc;

		switch (func) {
			case video::CompareFunc_Never:glfunc=GL_NEVER; break;
			case video::CompareFunc_Less:glfunc=GL_LESS; break;
			case video::CompareFunc_Lessequal:glfunc=GL_LEQUAL; break;
			case video::CompareFunc_Equal:glfunc=GL_EQUAL; break;
			case video::CompareFunc_Greaterequal:glfunc=GL_GEQUAL; break;
			case video::CompareFunc_Greater:glfunc=GL_GREATER; break;
			case video::CompareFunc_Notequal:glfunc=GL_NOTEQUAL; break;
			case video::CompareFunc_Always:glfunc=GL_ALWAYS; break;
		}

		glDepthFunc(glfunc);
	}

	// z test function. default: lessequal,0,0
	void OGLDevice::stencilfunc(const video::CompareFunc func,const ovis_uint32 ref,const ovis_uint32 writemask)
	{
		GLenum glfunc;

		switch (func) {
			case video::CompareFunc_Never:glfunc=GL_NEVER; break;
			case video::CompareFunc_Less:glfunc=GL_LESS; break;
			case video::CompareFunc_Lessequal:glfunc=GL_LEQUAL; break;
			case video::CompareFunc_Equal:glfunc=GL_EQUAL; break;
			case video::CompareFunc_Greaterequal:glfunc=GL_GEQUAL; break;
			case video::CompareFunc_Greater:glfunc=GL_GREATER; break;
			case video::CompareFunc_Notequal:glfunc=GL_NOTEQUAL; break;
			case video::CompareFunc_Always:glfunc=GL_ALWAYS; break;
		}

		glStencilFunc(glfunc,ref,writemask);
	}

	// stencil operations. default: zero,zero,zero
	void OGLDevice::stencilop(const video::StencilOp opfail,const video::StencilOp opzfail,const video::StencilOp oppass)
	{
		GLenum glfunc[3];
		ovis_uint32 funcs[3]={opfail,opzfail,oppass};

		for (int i=0;i<3;++i) {
			switch (funcs[i]) {
				case video::StencilOp_Keep:glfunc[i]=GL_KEEP; break;
				case video::StencilOp_Zero:glfunc[i]=GL_ZERO; break;
				case video::StencilOp_Set_To_Reference:glfunc[i]=GL_REPLACE; break;
				case video::StencilOp_Incr:glfunc[i]=GL_INCR; break;
				case video::StencilOp_Decr:glfunc[i]=GL_DECR; break;
				case video::StencilOp_Invert:glfunc[i]=GL_INVERT; break;
			}
		}

		glStencilOp(glfunc[0],glfunc[1],glfunc[2]);
	}

	// geometry drawing mode. default: faces
	void OGLDevice::drawingmode(const video::Drawingmode mode)
	{
		switch (mode) {
			case video::Drawingmode_Points:glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); break;
			case video::Drawingmode_Lines:glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
			case video::Drawingmode_Faces:glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
			default:break;
		}
	}

	// shading of filled triangles. default: smooth
	void OGLDevice::solidshading(const video::Solidshading type)
	{
		switch (type) {
			case video::Solidshading_Flat:glShadeModel(GL_FLAT); break;
			case video::Solidshading_Smooth:glShadeModel(GL_SMOOTH); break;
		default:break;
		}
	}

	// depth bias. default: 0,0 (disables depth bias)
	void OGLDevice::depthbias(const float scaledbias,const float uniformbias)
	{
		if ((scaledbias==0) && (uniformbias==0)) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glDisable(GL_POLYGON_OFFSET_LINE);
			glDisable(GL_POLYGON_OFFSET_POINT);
		} else {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glEnable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_POLYGON_OFFSET_POINT);

			glPolygonOffset(scaledbias,uniformbias);
		}
	}

	// ************* Sample stage parameters

	unsigned long OGLDevice::samplerstageParameterInt(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam) const
	{
		return 0;
	}

	float OGLDevice::samplerstageParameterFloat(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam) const
	{
		return 0;
	}

	void OGLDevice::samplerstageParameterInt(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam,const unsigned long value)
	{
	}

	void OGLDevice::samplerstageParameterFloat(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam,const float value)
	{
	}

	// ************* Texture stage parameters

	unsigned long OGLDevice::texstageParameterInt(const ovis_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return 0;
	}

	float OGLDevice::texstageParameterFloat(const ovis_uint32 texstage,const video::Texstageparameter texparam) const
	{
		return 0;
	}

	void OGLDevice::texstageParameterInt(const ovis_uint32 texstage,const video::Texstageparameter texparam,const unsigned long value)
	{
	}

	void OGLDevice::texstageParameterFloat(const ovis_uint32 texstage,const video::Texstageparameter texparam,const float value)
	{
	}

	// ************* Vertex/fragment program parameters

	void OGLDevice::vprogramSetFloat2(const base::String& varname,const base::Float2&)
	{
	}

	void OGLDevice::vprogramSetFloat3(const base::String& varname,const base::Float3&)
	{
	}

	void OGLDevice::vprogramSetFloat4(const base::String& varname,const base::Float4&)
	{
	}

	void OGLDevice::vprogramSetFloat3x3(const base::String& varname,const base::Float3x3&)
	{
	}

	void OGLDevice::vprogramSetFloat4x3(const base::String& varname,const base::Float4x3&)
	{
	}

	void OGLDevice::vprogramSetFloat3x4(const base::String& varname,const base::Float3x4&)
	{
	}

	void OGLDevice::vprogramSetFloat4x4(const base::String& varname,const base::Float4x4&)
	{
	}

	void OGLDevice::fprogramSetFloat2(const base::String& varname,const base::Float2&)
	{
	}

	void OGLDevice::fprogramSetFloat3(const base::String& varname,const base::Float3&)
	{
	}

	void OGLDevice::fprogramSetFloat4(const base::String& varname,const base::Float4&)
	{
	}

	void OGLDevice::fprogramSetFloat3x3(const base::String& varname,const base::Float3x3&)
	{
	}

	void OGLDevice::fprogramSetFloat4x3(const base::String& varname,const base::Float4x3&)
	{
	}

	void OGLDevice::fprogramSetFloat3x4(const base::String& varname,const base::Float3x4&)
	{
	}

	void OGLDevice::fprogramSetFloat4x4(const base::String& varname,const base::Float4x4&)
	{
	}

	void OGLDevice::fprogramBind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
	}

	void OGLDevice::fprogramBindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
	}

	video::Program* OGLDevice::vertexprogram()
	{
		return 0;
	}

	video::Program* OGLDevice::fragmentprogram()
	{
		return 0;
	}

	void OGLDevice::vertexprogram(video::Program* pVertexprogram)
	{
	}

	void OGLDevice::fragmentprogram(video::Program* pFragmentprogram)
	{
	}

	// ************* Fixed function pipeline support

	void OGLDevice::activateFFPipeline(const video::Fixedfunctionactivationtype pipeline)
	{
//		if (pipeline==video::FixedfunctionVertex)	m_pCGSystem->disableVPProfile();
//		else	m_pCGSystem->disableFPProfile();
	}

	void OGLDevice::fixedSetMaterialAmbientColor(const base::Float4& color)
	{
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,static_cast<const float*>(color));
	}

	void OGLDevice::fixedSetMaterialDiffuseColor(const base::Float4& color)
	{
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,static_cast<const float*>(color));
	}

	void OGLDevice::fixedSetMaterialSpecularColor(const base::Float4& color)
	{
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,static_cast<const float*>(color));
	}

	void OGLDevice::fixedSetMaterialEmissiveColor(const base::Float4& color)
	{
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,static_cast<const float*>(color));
	}

	void OGLDevice::fixedSetMaterialShininess(const float shininess)
	{
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
	}

	void OGLDevice::fixedSet2DTexture(const ovis_uint32 texstage,const video::Texture2D* texVar)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB+texstage);
		glClientActiveTextureARB(GL_TEXTURE0_ARB+texstage);

		if (!texVar) { glDisable(GL_TEXTURE_2D); return; }
		else {
			glEnable(GL_TEXTURE_2D);
			((OGL2DTexture*)texVar)->glbind();
		}
	}

	ovis_uint32 OGLDevice::fixedMaxTextures()
	{
		return 0;
	}

	ovis_uint32 OGLDevice::fixedMaxTextureStages()
	{
		GLint maxtexturestages;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxtexturestages);
		return static_cast<ovis_uint32>(maxtexturestages);
	}

	void OGLDevice::fixedLighting(const bool state)
	{
		enableCap(GL_LIGHTING,state);
	}

	void OGLDevice::fixedSetLightEnabled(const ovis_uint32 lightNr,const bool state)
	{
		enableCap(GL_LIGHT0+lightNr,state);
	}

	void OGLDevice::fixedSetLightProperties(const ovis_uint32 lightNr,const video::FFLightProperties& properties)
	{
		{
			float f[4]={properties.m_Position.x(),properties.m_Position.y(),properties.m_Position.z(),1};
			glLightfv(GL_LIGHT0+lightNr,GL_POSITION,f);
		}

		{
			float f[4]={properties.m_Direction.x(),properties.m_Direction.y(),properties.m_Direction.z(),1};
			glLightfv(GL_LIGHT0+lightNr,GL_SPOT_DIRECTION,f);
		}

		{
			float f[4]={properties.m_Color[1],properties.m_Color[2],properties.m_Color[3],properties.m_Color[0]};
			glLightfv(GL_LIGHT0+lightNr,GL_DIFFUSE,f);
		}

		glLightf(GL_LIGHT0+lightNr,GL_CONSTANT_ATTENUATION,properties.m_ConstantAttenuation);
		glLightf(GL_LIGHT0+lightNr,GL_LINEAR_ATTENUATION,properties.m_LinearAttenuation);
		glLightf(GL_LIGHT0+lightNr,GL_QUADRATIC_ATTENUATION,properties.m_QuadraticAttenuation);
	}

	void OGLDevice::fixedSetProjectionMatrix(const base::Float4x4& matrix)
	{
		math::Matrix4f m;
		m=matrix;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(static_cast<const float*>(m));
	}

	void OGLDevice::fixedSetWorldMatrix(const base::Float4x4& matrix)
	{
		m_World=matrix;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(static_cast<const float*>((m_World*m_View)));
	}

	void OGLDevice::fixedSetViewMatrix(const base::Float4x4& matrix)
	{
		m_View=matrix;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(static_cast<const float*>((m_World*m_View)));
	}

	ovis_uint32 OGLDevice::fixedMaxLights() const
	{
		GLint maxlights;
		glGetIntegerv(GL_MAX_LIGHTS,&maxlights);
		return static_cast<ovis_uint32>(maxlights);
	}

}
}

extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
ovis::video::Videodevice* createVideodeviceInstance()
{
	return new ovis::opengldrv::OGLDevice;
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* videodeviceDescription()
{
	return "OpenGL videodevice v0.5 build 2005-08-28 19:48";
}
