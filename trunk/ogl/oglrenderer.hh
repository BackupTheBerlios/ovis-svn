#ifndef OVIS_OGL_OGLRENDERER_HH_INCLUDED
#define OVIS_OGL_OGLRENDERER_HH_INCLUDED

#include <set>

#include "../video/renderer.hh"
#include "../video/scenenode.hh"
#include "ogldevice.hh"

namespace ovis {
namespace opengldrv {

	class OVIS_API OGLRenderer:public video::Renderer
	{
	public:
		OGLRenderer();
		~OGLRenderer();

		bool supported() const;
		void renderScene(const video::Camera& camera,video::Scenenode& scenenode);
		
		void init(const ovis_uint32 width,const ovis_uint32 height,
			const video::Pixelformat colorbufferformat,const video::Pixelformat depthstencilformat,const ovis_uint32 Hz);
			
		void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ovis_uint32 stencilvalue=0);
			
		video::Vertexstream* createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,
			const ovis_uint32 flags);
		video::Indexstream* createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,
			const ovis_uint32 flags);

		video::Texture2D* create2DTexture(const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 width,const ovis_uint32 height,const ovis_uint32 levels,
			const ovis_uint32 flags,const video::Pixelformat format);

		video::Cubemap* createCubemap(const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 edgelength,const ovis_uint32 levels,
			const ovis_uint32 flags,const video::Pixelformat format);

		bool isValid() const;
	protected:
		void traverseScene(std::set<video::Scenenode*> &donenodes,video::Visitcontext& vctx,video::Scenenode& scenenode);
		
		OGLDevice *m_pOGLDevice;
	};

}
}

#endif
