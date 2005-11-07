#ifndef OVIS_VIDEO_RENDERER_HH_INCLUDED
#define OVIS_VIDEO_RENDERER_HH_INCLUDED

#include "../base/basememobj.hh"
#include "camera.hh"
#include "cubemap.hh"
#include "pixelformat.hh"
#include "scenenode.hh"
#include "indexstream.hh"
#include "texture2d.hh"
#include "vertexstream.hh"

namespace ovis {
namespace video {

	class Lightserver;
	class Vertexformat;

	class OVIS_API Renderer:public base::BaseMemObj
	{
	public:
		virtual ~Renderer();
		
		/// Initializes the renderer with the given attributes		
		virtual void init(const ovis_uint32 width,const ovis_uint32 height,
			const Pixelformat colorbufferformat,const Pixelformat depthstencilformat,const ovis_uint32 Hz)=0;
			
		virtual void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ovis_uint32 stencilvalue=0)=0;
			
		virtual Vertexstream* createVertexstream(const ovis_uint32 numVertices,const Vertexformat& format,
			const ovis_uint32 flags)=0;
		virtual Indexstream* createIndexstream(const ovis_uint32 numIndices,const Indexformat format,
			const ovis_uint32 flags)=0;

		virtual Texture2D* create2DTexture(const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 width,const ovis_uint32 height,const ovis_uint32 levels,
			const ovis_uint32 flags,const Pixelformat format)=0;

		virtual Cubemap* createCubemap(const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 edgelength,const ovis_uint32 levels,
			const ovis_uint32 flags,const Pixelformat format)=0;

		/// Renders the scene with "scenenode" as root node
		/**
		* Renders the scene from the current camera position & orientation.
		* "scenenode" is the root node. This node gets rendered, along with its children
		* etc. The "Rootnode" type wasn't used since it should be allowed to render
		* subsets of a scene, which isn't possible with a "Rootnode" type.
		* NOTE: It only renders enabled nodes. If "scenenode" is disabled, neither of its
		* children will be rendered either.
		* @param scenenode The root node of the scene (or the subset of it) to be rendered.
		*/
		virtual void renderScene(const Camera& camera,Scenenode& scenenode)=0;

		/// Tests whether the video device supports features needed by this renderer or not
		virtual bool supported() const=0;

		/// Specifies the light server the renderer shall use.
		void lightserver(Lightserver& lserver);

		/// Adjusts the vertexformat to be suitable for this renderer
		virtual void adjustVertexformat(Vertexformat& format) const {}

	protected:
		Renderer();

		Lightserver* m_pLightserver;
	};

	typedef Renderer* (*CreateRendererInstanceFunc)();
	typedef const char* (*RendererDescriptionFunc)();

}
}

#endif
