#ifndef OVIS_OPENGLDRV_OGLRENDERER_HH_INCLUDED
#define OVIS_OPENGLDRV_OGLRENDERER_HH_INCLUDED

#include <set>
#include <vector>
#include "ovisgl.hh"
#include "../base/dll.hh"
#include "../math/matrix.hh"
#include "../video/coreenums.hh"
#include "../video/renderer.hh"

namespace ovis {
namespace opengldrv {

	class OGLRenderer:public video::Renderer
	{
	public:
		OGLRenderer(const ovis_uint32 width,const ovis_uint32 height);
		~OGLRenderer();
	
		bool isValid() const;

		void render(video::Scene& scene);
		void resize(const ovis_uint32 width,const ovis_uint32 height);
		void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,const ovis_uint32 stencilvalue=0);
		video::Vertexstream* createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,const ovis_uint32 flags);
		video::Indexstream* createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,const ovis_uint32 flags);

		bool supported() const;

		void drawingmode(const video::Drawingmode mode);
		video::Drawingmode drawingmode() const;

		void drawPrimitives(const video::PrimitivesType type,
			video::Vertexstream& vertexstream,const ovis_uint32 firstElement,
			const ovis_uint32 numElements);
		void drawIndexedPrimitives(
			const video::PrimitivesType type,
			video::Vertexstream& vertexstream,
			video::Indexstream& indexstream,
			const ovis_uint32 indexOffset,
			const ovis_uint32 numElements);

		bool extensionSupported(const std::string& extensionName) const;

	protected:

		void *getGLExtensionProcAddress(const char *name) const;
		
		void parseExtensionString();
		void initializeExtensions();
		void initializeVBO();
		void initializeOpenGL12();
		void initializeARBImaging();
		void initializeARBMultitexture();
		void initializeARBVertexProgram();
		void initializeARBTextureCompression();

		bool m_IsValid;
		math::Matrix4f m_World,m_View;
		video::Drawingmode m_Drawingmode;

		ovis_uint32 m_Displaywidth,m_Displayheight;

		std::set <std::string> *m_pSupportedExtensions;
	};

}
}

#endif
