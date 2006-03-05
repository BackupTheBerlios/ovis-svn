#ifndef OVIS_RENDERINSTANCE_HH_INCLUDED
#define OVIS_RENDERINSTANCE_HH_INCLUDED

#include "../base/fpscalc.hh"
#include "../math/arcball.hh"
#include "../math/quaternion.hh"
#include "../video/camera.hh"
#include "../video/colorscale.hh"
#include "../video/renderer.hh"
#include "../video/scene.hh"
#include "rotationcube.hh"

namespace ovis {

	class Renderinstance
	{
	public:
	
		enum Windowmode
		{
			Window_None=0,
			Window_Normal=1,
			Window_Fullscreen=2
		};
		
		enum UsedRenderer
		{
			UsedRenderer_Opengl=0,
			UsedRenderer_SWRasterizer=1,
			UsedRenderer_Raytracer=2
		};

		void testinitRenderer(
			const UsedRenderer usedrenderer,
			const Windowmode windowmode,
			const int windowwidth,
			const int windowheight,
			const bool interactive);

		void initRenderer(
			const UsedRenderer usedrenderer,
			const Windowmode windowmode,
			const int windowwidth,
			const int windowheight,
			const bool interactive);
		
		void shutdownRenderer();
		
		UsedRenderer usedRendererType() const;
		video::Renderer *usedRenderer();
		const math::Quaternion& rotation() const;
		const math::Vector3f& position() const;
		bool interactive() const;

		void windowmode(const Windowmode windowmode);
		void interactive(const bool mode);
		void rotation(const math::Quaternion& newrot);
		void position(const math::Vector3f& newpos);
		void waitUntilWindowClosed();
		
		void fetchSceneFromFile(const char *filename);
		
		video::Scene* currentScene();

		void calculateViewmatrix();
		void perspective(const int w,const int h);

		Renderinstance();
		~Renderinstance();
				
	protected:
		void scene(video::Scene &rScene);
	
		UsedRenderer m_Usedrenderer;

		math::Quaternion m_Rotation;
		math::Vector3f m_Position;
		
		video::Camera m_Camera;
		video::Colorscale m_Colorscale;
		video::Renderer *m_pRenderer;
		video::Scene* m_pScene,*m_pTestscene;
		
		bool m_Interactive;
		
		class Window;
		Window *m_pWindow;
	};

}

#endif
