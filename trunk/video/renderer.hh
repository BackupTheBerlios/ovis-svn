#ifndef OVIS_VIDEO_RENDERER_HH_INCLUDED
#define OVIS_VIDEO_RENDERER_HH_INCLUDED

#include "camera.hh"
#include "scenenode.hh"

namespace ovis {
namespace video {

	class Lightserver;
	class Vertexformat;
	class Videodevice;

	class OVIS_API Renderer
	{
	public:
		virtual ~Renderer();

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

		/// Returns the video device the renderer uses
		inline Videodevice& videodevice() const { return m_rVideodevice; }

		/// Tests whether the video device supports features needed by this renderer or not
		virtual bool supported() const=0;

		/// Specifies the light server the renderer shall use.
		void lightserver(Lightserver& lserver);

		/// Adjusts the vertexformat to be suitable for this renderer
		virtual void adjustVertexformat(Vertexformat& format) const {}

		/// This should be called when the video device changed its viewport
		/**
		* After the viewport resize, the device may have deallocated all resources that are in
		* the default pool. Calling this method gives the renderer the chance to reset internal
		* resources.
		*/
		virtual void checkResources()=0;
	protected:
		Renderer(Videodevice& rVideodevice);

		Videodevice& m_rVideodevice;
		Lightserver* m_pLightserver;
	};

}
}

#endif
