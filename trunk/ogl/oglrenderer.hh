#ifndef OVIS_OPENGLDRV_OGLRENDERER_HH_INCLUDED
#define OVIS_OPENGLDRV_OGLRENDERER_HH_INCLUDED

#include "ovisgl.hh"
#include "../video/renderer.hh"

namespace ovis {
namespace opengldrv {

	class OGLRenderer:public video::Renderer
	{
	public:
		OGLRenderer(const ovis_uint32 width,const ovis_uint32 height);
	
		void render(video::Scene& scene);
		void resize(const ovis_uint32 width,const ovis_uint32 height);
		void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,const ovis_uint32 stencilvalue=0);
		video::Vertexstream* createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,const ovis_uint32 flags);
		video::Indexstream* createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,const ovis_uint32 flags);

		bool supported() const;

	protected:		
	};

}
}

#endif
