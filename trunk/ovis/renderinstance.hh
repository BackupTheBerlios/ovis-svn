#ifndef OVIS_RENDERINSTANCE_HH_INCLUDED
#define OVIS_RENDERINSTANCE_HH_INCLUDED

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>

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
		
		
		// Constructor/Destructor
		
		Renderinstance();
		~Renderinstance();
		
		// Renderer handling

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
		
		// Window handling

		void windowmode(const Windowmode windowmode);
		
		// Getters/setters
		
		const math::Quaternion& getRotation() const;
		const math::Vector3f& getPosition() const;
		bool isInteractive() const;

		void setInteractive(const bool mode);
		void setRotation(const math::Quaternion& newrot);
		void setPosition(const math::Vector3f& newpos);
		
		// Thread handling

		boost::recursive_try_mutex& mutex();
		void signalThreadEnd();
		void waitUntilWindowClosed();
		
		// Scene handling
		
		void fetchSceneFromFile(const char *filename);
		video::Scene* currentScene();
		
		// Camera handling

		void calculateViewmatrix();
		void perspective(const int w,const int h);

	protected:
		class InternalFunctor
		{
		public:
			void operator()();

			Renderinstance* m_pRenderinstance;
		};
		
		friend class InternalFunctor;
	
		Renderinstance(const Renderinstance& r) {}
		Renderinstance& operator =(const Renderinstance& r) { return *this; }
	
		void createRenderer(const UsedRenderer usedrenderer,const int windowwidth,const int windowheight);
		void scene(video::Scene &rScene);
		void startThread();
		void stopThread();
	
		UsedRenderer m_Usedrenderer;
		Windowmode m_Windowmode;

		math::Quaternion m_Rotation;
		math::Vector3f m_Position;
		
		video::Camera m_Camera;
		video::Colorscale m_Colorscale;
		video::Renderer *m_pRenderer;
		video::Scene* m_pScene,*m_pTestscene;
		
		int m_Windowwidth,m_Windowheight;
		
		bool m_Interactive,m_ThreadQuit;
		
		boost::thread *m_pThread;
		boost::recursive_try_mutex *m_pMutex;
		
		math::Arcball m_Arcball;
		math::Quaternion m_OldRotation;
		
		InternalFunctor m_InternalFunctor;
		
#ifdef COMPILE_WITH_SDL_SUPPORT
		SDL_Surface *m_Screen;
#endif
	
	};

}

#endif
