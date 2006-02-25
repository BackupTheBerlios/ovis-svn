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

#ifdef COMPILE_WITH_SDL_SUPPORT
#include <SDL.h>
#endif

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

		void perspectiveViewport(const int w,const int h);
		
		void testinitRenderer(
			const UsedRenderer usedrenderer,
			const Windowmode windowmode,
			const int windowwidth,
			const int windowheight);

		void initRenderer(
			video::Scene *pScene,
			const UsedRenderer usedrenderer,
			const Windowmode windowmode,
			const int windowwidth,
			const int windowheight);
		
		void windowmode(const Windowmode windowmode);
		void scene(video::Scene &rScene);

		void endRenderer();

		void interactiveLoop();

		Renderinstance();
		~Renderinstance();

	protected:

		void perspective(const int w,const int h);

		void updateScreen();
		void calculateViewmatrix();

		Windowmode m_Windowmode;
		UsedRenderer m_Usedrenderer;
#ifdef COMPILE_WITH_SDL_SUPPORT
		SDL_Surface *m_Screen;
#endif
		base::Fpscalc m_FPS;
		math::Arcball m_Arcball;
		math::Quaternion m_Rotation,m_OldRotation;
		math::Vector3f m_Position;
		video::Camera m_Camera;
		video::Colorscale m_Colorscale;
		video::Renderer *m_pRenderer;
		RotationCube m_RotCube;
		video::Scene* m_pScene,*m_pTestscene;
		int m_Width,m_Height;
	};

}

#endif
