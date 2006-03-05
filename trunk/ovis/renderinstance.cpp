#include <boost/thread/thread.hpp>
#include "../ogl/oglrenderer.hh"
#include "../video/loadscenefromfile.hh"
#include "renderinstance.hh"

#ifdef COMPILE_WITH_SDL_SUPPORT
#include <SDL.h>
#endif

namespace ovis {

namespace video {
	OVIS_API Scene* generateTestscene(Renderer *pRenderer,Colorscale *pColorscale);
}

	class Renderinstance::Window
	{
	public:
		Window(Renderinstance& rRenderinstance,const bool interactive,const Windowmode windowmode,const int windowwidth,
			const int windowheight);
		~Window();
		
		void startThread();
		void stopThread();
		
		void mode(const Windowmode newmode);
		void interactive(const bool mode);
		void scene(video::Scene *pScene);
		
		void freeze();
		void unfreeze();
		
		void updateScreen();
		
	protected:
		void messagepump();
	
		video::Scene *m_pScene;
		Renderinstance& m_rRenderinstance;
		bool m_Interactive;
		Windowmode m_Windowmode;
		int m_Width,m_Height;
		base::Fpscalc m_FPS;
		math::Arcball m_Arcball;
		math::Quaternion m_OldRotation;
	
#ifdef COMPILE_WITH_SDL_SUPPORT
		SDL_Surface *m_Screen;
#endif
	};
	
	Renderinstance::Window::Window(Renderinstance& rRenderinstance,const bool interactive,const Windowmode windowmode,
	const int windowwidth,const int windowheight):m_pScene(0),m_rRenderinstance(rRenderinstance),
	m_Interactive(interactive),m_Windowmode(Window_None),m_Width(windowwidth),m_Height(windowheight)
	{
		mode(windowmode);
	}
	
	Renderinstance::Window::~Window()
	{
		stopThread();
	
#ifdef COMPILE_WITH_SDL_SUPPORT
		if (m_Windowmode!=Window_None)
			SDL_Quit();
#endif
	}
	
	void Renderinstance::Window::startThread()
	{
	}
	
	void Renderinstance::Window::stopThread()
	{
	}
	
	void Renderinstance::Window::mode(const Windowmode newmode)
	{
		if (newmode==m_Windowmode) return;
		if ((m_rRenderinstance.usedRendererType()==UsedRenderer_Opengl) && (newmode==Window_None)) return;
		
	
#ifdef COMPILE_WITH_SDL_SUPPORT
		switch (newmode) {
			case Window_Normal:
			case Window_Fullscreen:
				if (m_Windowmode==Window_None)
					SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER );
						m_Screen=SDL_SetVideoMode(m_Width,m_Height,0,
						((m_rRenderinstance.usedRendererType()==UsedRenderer_Opengl) ? SDL_OPENGL : 0)
						| ((newmode==Window_Normal) ? SDL_RESIZABLE : 0)
						| ((newmode==Window_Fullscreen) ? SDL_FULLSCREEN : 0)
						);
				break;
			default:break;
		}
		
#endif
		m_Windowmode=newmode;
	}
	
	void Renderinstance::Window::interactive(const bool mode)
	{
		freeze();
		m_Interactive=mode;
		unfreeze();
	}
	
	void Renderinstance::Window::scene(video::Scene *pScene)
	{
		freeze();
		m_pScene=pScene;
		unfreeze();
	}
	
	void Renderinstance::Window::freeze()
	{
	}
	
	void Renderinstance::Window::unfreeze()
	{
	}
	
	void Renderinstance::Window::updateScreen()
	{
#ifdef COMPILE_WITH_SDL_SUPPORT
		if (m_rRenderinstance.usedRendererType()==UsedRenderer_Opengl)
			SDL_GL_SwapBuffers();
		else
			SDL_Flip(m_Screen);
#endif
	}
	
	void Renderinstance::Window::messagepump()
	{
#ifdef COMPILE_WITH_SDL_SUPPORT
		if (m_Windowmode==Window_None) return;

		SDL_Event event;
		bool quit=false;
		while (!quit) {
			SDL_WaitEvent(&event);
	
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN: {
					if (!m_Interactive) break;
					if (!(event.button.button&SDL_BUTTON_LEFT) || !(event.button.state&SDL_PRESSED))
						break;
				
					float xx,yy;
					xx=((float)(event.button.x))/((float)(m_Width)*0.5f)-1.0f;
					yy=((float)(event.button.y))/((float)(m_Height)*0.5f)-1.0f;
			
					m_Arcball.click(xx,-yy);
			
					m_OldRotation=m_rRenderinstance.rotation();
			
					m_rRenderinstance.calculateViewmatrix();
				} break;

				case SDL_MOUSEMOTION: {
					if (!m_Interactive) break;
					if (!(event.motion.state&SDL_BUTTON_LEFT)) break;

					float xx,yy;
					xx=((float)(event.motion.x))/((float)(m_Width)*0.5f)-1.0f;
					yy=((float)(event.motion.y))/((float)(m_Height)*0.5f)-1.0f;
			
					math::Quaternion quat;					
					m_Arcball.drag(xx,-yy,&quat);
					m_rRenderinstance.rotation((m_OldRotation*quat).normalized());
			
					m_rRenderinstance.calculateViewmatrix();
				} break;

				case SDL_VIDEORESIZE: {
					int w=event.resize.w;
					int h=event.resize.h;
					m_Width=w;
					m_Height=h;
					m_rRenderinstance.usedRenderer()->resize(0,0,w,h);
					m_rRenderinstance.perspective(w,h);
				} break;
				case SDL_QUIT:quit=true; break;
				default:break;
			}

			if (m_rRenderinstance.usedRenderer())
				m_rRenderinstance.usedRenderer()->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,0.4f,0.4f,0.4f));
			if (m_pScene) m_pScene->render();

			updateScreen();
		}
#endif
	}








	void Renderinstance::testinitRenderer(
		const UsedRenderer usedrenderer,
		const Windowmode windowmode,
		const int windowwidth,
		const int windowheight,
		const bool interactive)
	{
		if (m_pRenderer!=0) return;
		
		if (m_pTestscene==0) m_pTestscene=video::generateTestscene(0,0);
		initRenderer(usedrenderer,windowmode,windowwidth,windowheight,interactive);
		scene(*m_pTestscene);
	}

	void Renderinstance::initRenderer(
		const UsedRenderer usedrenderer,
		const Windowmode windowmode,
		const int windowwidth,
		const int windowheight,
		const bool interactive)
	{
		if (m_pRenderer!=0) return;
		
		m_Interactive=interactive;

		Windowmode wm=((usedrenderer==UsedRenderer_Opengl) && (windowmode==Window_None)) ? Window_Normal : windowmode;
		m_Usedrenderer=usedrenderer;
	
		perspective(windowwidth,windowheight);
		
		m_pWindow=new Window(*this,interactive,wm,windowwidth,windowheight);

		m_pRenderer=0;
	
		switch (usedrenderer) {
	#ifdef COMPILE_WITH_SDL_SUPPORT
			case UsedRenderer_Opengl:
				opengldrv::OGLRenderer *pOGLRenderer=new opengldrv::OGLRenderer(windowwidth,windowheight);
				m_pRenderer=pOGLRenderer;
				break;
	#endif
			case UsedRenderer_SWRasterizer:
				break;
			case UsedRenderer_Raytracer:
				break;
			default:return; // no renderer -> bail out
		}

		m_pWindow->startThread();
	}
	
	void Renderinstance::shutdownRenderer()
	{
		if (m_pRenderer==0) return;

		m_pWindow->stopThread();
		delete m_pRenderer; m_pRenderer=0;
		if (m_pScene!=0) delete m_pScene; m_pScene=0;
		if (m_pWindow!=0) delete m_pWindow; m_pWindow=0;
	}
	
	Renderinstance::UsedRenderer Renderinstance::usedRendererType() const
	{
		return m_Usedrenderer;
	}
	
	video::Renderer* Renderinstance::usedRenderer()
	{
		return m_pRenderer;
	}
	
	const math::Quaternion& Renderinstance::rotation() const
	{
		return m_Rotation;
	}
	
	const math::Vector3f& Renderinstance::position() const
	{
		return m_Position;
	}
	
	bool Renderinstance::interactive() const
	{
		return m_Interactive;
	}

	void Renderinstance::windowmode(const Windowmode windowmode)
	{
		if (m_pWindow) {
			m_pWindow->freeze();
			m_pWindow->mode(windowmode);
			m_pWindow->unfreeze();
		}
	}
	
	void Renderinstance::interactive(const bool mode)
	{
		if (m_pWindow) {
			m_pWindow->freeze();
			m_Interactive=mode;
			m_pWindow->interactive(mode);
			m_pWindow->unfreeze();
		} else m_Interactive=mode;
	}

	void Renderinstance::rotation(const math::Quaternion& newrot)
	{
		if (m_pWindow) m_pWindow->freeze();
		m_Rotation=newrot;
		if (m_pWindow) m_pWindow->unfreeze();
	}
	
	void Renderinstance::position(const math::Vector3f& newpos)
	{
		if (m_pWindow) m_pWindow->freeze();
		m_Position=newpos;
		if (m_pWindow) m_pWindow->unfreeze();
	}

	void Renderinstance::waitUntilWindowClosed()
	{
	}
	
	void Renderinstance::fetchSceneFromFile(const char *filename)
	{
		if (m_pRenderer==0) return;

		video::Scene* pScene=video::loadSceneFromFile(filename,*m_pRenderer,&m_Colorscale);
		scene(*pScene);
	}
	
	video::Scene* Renderinstance::currentScene()
	{
		return m_pScene;
	}
	
	void Renderinstance::calculateViewmatrix()
	{
		if (m_pWindow) m_pWindow->freeze();
	
		m_Camera.viewmatrix().loadIdentity();
		m_Camera.viewmatrix().fromQuaternion(m_Rotation);
		m_Camera.viewmatrix().m_41=m_Position.x();
		m_Camera.viewmatrix().m_42=m_Position.y();
		m_Camera.viewmatrix().m_43=m_Position.z();

		if (m_pWindow) m_pWindow->unfreeze();
	}

	Renderinstance::Renderinstance():m_pRenderer(0),m_pScene(0),m_pTestscene(0),m_pWindow(0)
	{
		m_Camera.viewmatrix().m_43=m_Position.z()=3;
	
		m_Colorscale.addColor(0.00f,0x00FF0000); // red
		m_Colorscale.addColor(0.33f,0x0000FF00); // green
		m_Colorscale.addColor(0.66f,0x0000FFFF); // yellow
		m_Colorscale.addColor(1.00f,0x000000FF); // red
	}
	
	Renderinstance::~Renderinstance()
	{
		shutdownRenderer();
	}

	void Renderinstance::scene(video::Scene &rScene)
	{
		if (m_pScene==(&rScene)) return;

		if (m_pWindow) m_pWindow->freeze();

		if ((m_pScene!=0) && (m_pScene!=m_pTestscene)) {
			delete m_pScene; m_pScene=0;
		}

		rScene.camera(m_Camera);
		rScene.renderer(*m_pRenderer);
		rScene.colorscale(m_Colorscale);
		
		m_pScene=&rScene;

		if (m_pWindow) {
			m_pWindow->scene(m_pScene);
			m_pWindow->unfreeze();
		}
	}
	
	void Renderinstance::perspective(const int w,const int h)
	{
		m_Camera.projmatrix().perspective(
			3.1415926535f*0.5f,
			((float)w/(float)h),
			1,100);
	}
	
}
