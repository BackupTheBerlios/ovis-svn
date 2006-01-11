#include "../base/log.hh"
#include "../video/coreenums.hh"
#include "../video/scene.hh"
#include "oglindexstream.hh"
#include "oglrenderer.hh"
#include "oglvertexstream.hh"

#include <iostream>

namespace ovis {
namespace opengldrv {

	OGLRenderer::OGLRenderer(const ovis_uint32 width,const ovis_uint32 height):
	Renderer(width,height),m_IsValid(false),m_pSupportedExtensions(0)
	{
		m_pSupportedExtensions=new std::set <std::string>;
		
		parseExtensionString();
		initializeExtensions();

		glViewport(0,0,(GLint)width,(GLint)height);
		glDepthFunc(GL_LESS);

		glFrontFace(GL_CW);

		m_Displaywidth=width;
		m_Displayheight=height;

		drawingmode(video::Drawingmode_SmoothShadedAndWireframe);




		m_IsValid=true;
	}
	
	OGLRenderer::~OGLRenderer()
	{
		if (m_pSupportedExtensions) delete m_pSupportedExtensions;
	}

	bool OGLRenderer::isValid() const
	{
		return m_IsValid;
	}

	void OGLRenderer::render(video::Scene& scene)
	{
		const float *pF;

		glMatrixMode(GL_PROJECTION);
		pF=scene.camera()->projmatrix();
		glLoadMatrixf(pF);

		glMatrixMode(GL_MODELVIEW);
		pF=scene.camera()->viewmatrix();
		glLoadMatrixf(pF);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glPolygonOffset(0,0);

		for (ovis_uint32 entrynr=0;entrynr<scene.attributetable().numEntries();++entrynr) {
			const video::Attributetableentry *pEntry=scene.attributetable().entry(entrynr);
			if (pEntry==0) continue;

			if (pEntry->m_Faceattribute!=video::MATERIAL_VERTEXCOLOR) {
				// TODO:
			}

			drawIndexedPrimitives(
				video::Primitives_Triangles,
				*(scene.vertexstream()),
				*(scene.indexstream()),
				pEntry->m_Startface*3,
				pEntry->m_Faceamount);
		}


		if ((m_Drawingmode==video::Drawingmode_FlatShadedAndWireframe) || (m_Drawingmode==video::Drawingmode_SmoothShadedAndWireframe)) {
			video::Drawingmode d=m_Drawingmode;
			drawingmode(video::Drawingmode_Wireframe);

			glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonOffset(-1,-1);
			glColor3f(0,0,0);

			drawIndexedPrimitives(video::Primitives_Triangles,*(scene.vertexstream()),*(scene.indexstream()),
				0,scene.indexstream()->capacity()/3);

			drawingmode(d);
		}
	}
	
	void OGLRenderer::resize(const ovis_uint32 width,const ovis_uint32 height)
	{
		glViewport(0,0,(GLint)width,(GLint)height);
	}

	void OGLRenderer::clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue,const ovis_uint32 stencilvalue)
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
	
	video::Vertexstream* OGLRenderer::createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,const ovis_uint32 flags)
	{
		return new OGLVertexstream(numVertices,format,flags);
	}
	
	video::Indexstream* OGLRenderer::createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,const ovis_uint32 flags)
	{
		return new OGLIndexstream(numIndices,format,flags);
	}
	

	bool OGLRenderer::supported() const
	{
		return true;
	}

	void OGLRenderer::drawingmode(const video::Drawingmode mode)
	{
		switch (mode) {
			case video::Drawingmode_Points:
				glShadeModel(GL_SMOOTH);
				glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
				break;
			case video::Drawingmode_Wireframe:
				glShadeModel(GL_SMOOTH);
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case video::Drawingmode_FlatShaded:
			case video::Drawingmode_FlatShadedAndWireframe:
				glShadeModel(GL_FLAT);
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case video::Drawingmode_SmoothShaded:
			case video::Drawingmode_SmoothShadedAndWireframe:
				glShadeModel(GL_SMOOTH);
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
		};

		m_Drawingmode=mode;
	}

	video::Drawingmode OGLRenderer::drawingmode() const
	{
		return m_Drawingmode;
	}

	void OGLRenderer::drawPrimitives(const video::PrimitivesType type,
		video::Vertexstream& vertexstream,const ovis_uint32 firstElement,
		const ovis_uint32 numElements)
	{
		GLenum ptype=GL_POINTS;
		ovis_uint32 vtxfactor=1;
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

	void OGLRenderer::drawIndexedPrimitives(
		const video::PrimitivesType type,
		video::Vertexstream& vertexstream,
		video::Indexstream& indexstream,
		const ovis_uint32 indexOffset,
		const ovis_uint32 numElements)
	{
		GLenum ptype=GL_POINTS;
		ovis_uint32 vtxfactor=1;
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

		if (m_Drawingmode==video::Drawingmode_Wireframe)
			glDisableClientState(GL_COLOR_ARRAY);

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


//#define IONGL_HAVE_GLXGETPROCADDRESS


	void *OGLRenderer::getGLExtensionProcAddress(const char *name) const
	{
		void *pProc;
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
		pProc=(void*)wglGetProcAddress(name);
#else
#ifdef IONGL_HAVE_GLXGETPROCADDRESS
		pProc=(void*)glXGetProcAddress((const GLubyte*)name);
#else
		pProc=(void*)glXGetProcAddressARB((const GLubyte*)name);
#endif

#endif
		if (!pProc) base::log("OGLDevice::getGLExtensionProcAddress()",base::Warning) << "WARN: Extension proc " << name << " not found\n";
		return pProc;
	}

	void OGLRenderer::parseExtensionString()
	{
		const char *ext=(const char*)glGetString(GL_EXTENSIONS);
		
		if (ext==0) {
			base::log("OGLDevice::parseExtensionString()",base::Error) << "No extension string returned!\n";
			return;
		}
	
		std::string extensions(ext);

		std::string token;
		std::string::size_type pos=0,lastpos=0;
		
		base::log("OGLDevice::parseExtensionString()",base::Message) << "Supported extensions:\n";

		while ((pos=extensions.find_first_of(' ',lastpos))!=std::string::npos) {
			token=extensions.substr(lastpos,pos-lastpos);
			lastpos=pos+1;
			m_pSupportedExtensions->insert(token);
		
			//base::log("OGLDevice::parseExtensionString()",base::Message) << token << " supported\n";
			base::logstream() << token << " ";
		}
		base::logstream() << "\n";
	}

	bool OGLRenderer::extensionSupported(const std::string& extensionName) const
	{
		return (m_pSupportedExtensions->find(extensionName)!=m_pSupportedExtensions->end());
	}

	void OGLRenderer::initializeExtensions()
	{
		initializeOpenGL12();
		initializeARBVertexProgram();
		initializeARBImaging();
		initializeARBMultitexture();
		initializeARBTextureCompression();
		initializeVBO();
	}

	void OGLRenderer::initializeOpenGL12()
	{
#ifdef ION_IMPLEMENT_GL_VERSION_1_2
		glTexImage3D = (glTexImage3DPROC) getGLExtensionProcAddress("glTexImage3D");
		glTexSubImage3D = (glTexSubImage3DPROC) getGLExtensionProcAddress("glTexSubImage3D");
		glCopyTexSubImage3D = (glCopyTexSubImage3DPROC) getGLExtensionProcAddress("glCopyTexSubImage3D");
		glDrawRangeElements = (glDrawRangeElementsPROC) getGLExtensionProcAddress("glDrawRangeElements");
#endif /* GL_VERSION_1_2 */
	}

	void OGLRenderer::initializeARBImaging()
	{
		if (!extensionSupported("GL_ARB_imaging")) {
			/*localLogger() << "WARN: No ARB_imaging support detected";*/ return;
		}
	
#ifdef ION_IMPLEMENT_GL_ARB_IMAGING
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

	void OGLRenderer::initializeARBMultitexture()
	{
		if (!extensionSupported("GL_ARB_multitexture")) {
			//localLogger() << "WARN: No ARB multitexture support detected";
			return;
		}
	
#ifdef ION_IMPLEMENT_GL_ARB_MULTITEXTURE
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

	void OGLRenderer::initializeARBVertexProgram()
	{
		if (!extensionSupported("GL_ARB_vertex_program")) {
			/*localLogger() << "WARN: No ARB vertex program support detected";*/ return;
		}

#ifdef ION_IMPLEMENT_GL_ARB_VERTEX_PROGRAM
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

	void OGLRenderer::initializeARBTextureCompression()
	{
		if (!extensionSupported("GL_ARB_texture_compression")) {
			/*localLogger() << "WARN: No ARB texture compression support detected";*/ return;
		}

#ifdef ION_IMPLEMENT_GL_ARB_TEXTURE_COMPRESSION
		glCompressedTexImage3DARB = (glCompressedTexImage3DARBPROC) getGLExtensionProcAddress("glCompressedTexImage3DARB");
		glCompressedTexImage2DARB = (glCompressedTexImage2DARBPROC) getGLExtensionProcAddress("glCompressedTexImage2DARB");
		glCompressedTexImage1DARB = (glCompressedTexImage1DARBPROC) getGLExtensionProcAddress("glCompressedTexImage1DARB");
		glCompressedTexSubImage3DARB = (glCompressedTexSubImage3DARBPROC) getGLExtensionProcAddress("glCompressedTexSubImage3DARB");
		glCompressedTexSubImage2DARB = (glCompressedTexSubImage2DARBPROC) getGLExtensionProcAddress("glCompressedTexSubImage2DARB");
		glCompressedTexSubImage1DARB = (glCompressedTexSubImage1DARBPROC) getGLExtensionProcAddress("glCompressedTexSubImage1DARB");
		glGetCompressedTexImageARB = (glGetCompressedTexImageARBPROC) getGLExtensionProcAddress("glGetCompressedTexImageARB");
#endif
	}

	void OGLRenderer::initializeVBO()
	{
		if (!extensionSupported("GL_ARB_vertex_buffer_object")) {
			//localLogger() << "WARN: No vertex buffer object support detected";
			return;
		}

#ifdef ION_IMPLEMENT_GL_ARB_VERTEX_BUFFER_OBJECT
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

}
}

extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
ovis::video::Renderer* createRendererInstance(const ovis_uint32 width,const ovis_uint32 height)
{
	return new ovis::opengldrv::OGLRenderer(width,height);
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* rendererDescription()
{
	return "OpenGL renderer v0.5 build 2005-11-07 19:48";
}

