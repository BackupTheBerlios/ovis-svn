#include "oglrenderer.hh"

namespace ovis {
namespace opengldrv {

	OGLRenderer::OGLRenderer(const ovis_uint32 width,const ovis_uint32 height):Renderer(width,height)
	{
	}

	void OGLRenderer::render(video::Scene& scene)
	{
	}
	
	void OGLRenderer::resize(const ovis_uint32 width,const ovis_uint32 height)
	{
	}
	
	void OGLRenderer::clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue,const ovis_uint32 stencilvalue)
	{
	}
	
	video::Vertexstream* OGLRenderer::createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,const ovis_uint32 flags)
	{
		return 0;
	}
	
	video::Indexstream* OGLRenderer::createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,const ovis_uint32 flags)
	{
		return 0;
	}
	

	bool OGLRenderer::supported() const
	{
		return true;
	}

}
}
