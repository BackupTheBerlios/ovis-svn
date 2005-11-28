#ifndef OVIS_VIDEO_RENDERER_HH_INCLUDED
#define OVIS_VIDEO_RENDERER_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/basememobj.hh"
#include "../base/numericarrays.hh"

#include "indexstream.hh"
#include "vertexstream.hh"

namespace ovis {
namespace video {

	class Scene;

	class Renderer:public base::BaseMemObj
	{
	public:
		virtual ~Renderer() {}

		//! Renders a scene.
		/**
		* \note
		* Scene::render() calls this method internally. Look there for more information about this.
		*/
		virtual void render(Scene& scene)=0;
		//! Signals the renderer that the viewport has been resized.
		/**
		* @param width New width of the viewport, in pixels
		* @param height New height of the viewport, in pixels
		*/
		virtual void resize(const ovis_uint32 width,const ovis_uint32 height)=0;
		virtual void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ovis_uint32 stencilvalue=0)=0;
		virtual Vertexstream* createVertexstream(const ovis_uint32 numVertices,const Vertexformat& format,
			const ovis_uint32 flags)=0;
		virtual Indexstream* createIndexstream(const ovis_uint32 numIndices,const Indexformat format,
			const ovis_uint32 flags)=0;

		/// Tests whether the video device supports features needed by this renderer or not
		virtual bool supported() const=0;

		virtual void drawingmode(const Drawingmode mode)=0;
		virtual Drawingmode drawingmode() const=0;

	protected:
		//! Constructs a renderer.
		/**
		* @param width Width of the used viewport, in pixels
		* @param height Height of the used viewport, in pixels
		*/
		Renderer(const ovis_uint32 width,const ovis_uint32 height) {}
	};

	typedef Renderer* (*CreateRendererInstanceFunc)(const ovis_uint32 width,const ovis_uint32 height);
	typedef const char* (*RendererDescriptionFunc)();

}
}

#endif
