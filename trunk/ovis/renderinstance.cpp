#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include "../ogl/oglrenderer.hh"
#include "../video/loadscenefromfile.hh"
#include "renderinstance.hh"

#ifdef COMPILE_WITH_SDL_SUPPORT
#include <SDL.h>
#endif

#define mutexScopedLock() boost::recursive_try_mutex::scoped_try_lock mutexLock(*m_pMutex)

namespace ovis {

namespace video {
	OVIS_API Scene* generateTestscene(Renderer *pRenderer,Colorscale *pColorscale);
}




	// Constructor/Destructor

	Renderinstance::Renderinstance():m_pRenderer(0),m_pScene(0),m_pTestscene(0),m_Interactive(false),m_pThread(0)
	{
		m_InternalFunctor.m_pRenderinstance=this;
	
		m_pMutex=new boost::recursive_try_mutex();
	
		m_Camera.viewmatrix().m_43=m_Position.z()=3;
	
		m_Colorscale.addColor(0.00f,0x00FF0000); // red
		m_Colorscale.addColor(0.33f,0x0000FF00); // green
		m_Colorscale.addColor(0.66f,0x0000FFFF); // yellow
		m_Colorscale.addColor(1.00f,0x000000FF); // red
	}
	
	Renderinstance::~Renderinstance()
	{
		shutdownRenderer();
		delete m_pMutex;
	}



	// Renderer handling

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
		
		m_Usedrenderer=usedrenderer;
		m_Windowmode=((usedrenderer==UsedRenderer_Opengl) && (windowmode==Window_None)) ? Window_Normal : windowmode;
		m_Windowwidth=windowwidth;
		m_Windowheight=windowheight;
		m_Interactive=interactive;
		
		if (usedrenderer==UsedRenderer_Opengl) {
			startThread();
		} else {
			createRenderer(usedrenderer,windowwidth,windowheight);
			if (windowmode!=Window_None)
				startThread();
		}
	}
	
	void Renderinstance::startThread()
	{
		stopThread();
		m_ThreadQuit=false;
		m_pThread=new boost::thread(m_InternalFunctor);
	}

	void Renderinstance::stopThread()
	{
		if (m_pThread==0) return;
		signalThreadEnd();
		waitUntilWindowClosed();
		delete m_pThread;
		m_pThread=0;
	}
	
	void Renderinstance::createRenderer(const UsedRenderer usedrenderer,const int windowwidth,const int windowheight)
	{
		mutexScopedLock();

		switch (usedrenderer) {
			case UsedRenderer_Opengl:
				m_pRenderer=new opengldrv::OGLRenderer(windowwidth,windowheight);
			default:break;
		}
		
		if (m_pScene) m_pScene->renderer(*m_pRenderer);
		
		m_pRenderer->resize(0,0,windowwidth,windowheight);
		perspective(windowwidth,windowheight);
	}
	
	void Renderinstance::shutdownRenderer()
	{
		stopThread();
		if (m_pRenderer!=0) delete m_pRenderer; m_pRenderer=0;
		if ((m_pScene!=0) && (m_pScene!=m_pTestscene)) delete m_pScene; m_pScene=0;
		if (m_pTestscene!=0) delete m_pTestscene; m_pTestscene=0;
	}
	
	Renderinstance::UsedRenderer Renderinstance::usedRendererType() const
	{
		return m_Usedrenderer;
	}
	
	video::Renderer* Renderinstance::usedRenderer()
	{
		return m_pRenderer;
	}
	
	
	
	
	// Window handling

	void Renderinstance::windowmode(const Windowmode windowmode)
	{
	}




	// Getters/setters

	const math::Quaternion& Renderinstance::getRotation() const
	{
		return m_Rotation;
	}
	
	const math::Vector3f& Renderinstance::getPosition() const
	{
		return m_Position;
	}
	
	bool Renderinstance::isInteractive() const
	{
		return m_Interactive;
	}
	
	void Renderinstance::setInteractive(const bool mode)
	{
		mutexScopedLock();
		m_Interactive=mode;
	}

	void Renderinstance::setRotation(const math::Quaternion& newrot)
	{
		mutexScopedLock();
		m_Rotation=newrot;
	}
	
	void Renderinstance::setPosition(const math::Vector3f& newpos)
	{
		mutexScopedLock();
		m_Position=newpos;
	}




	// Thread handling

	void Renderinstance::InternalFunctor::operator()()
	{
#ifdef COMPILE_WITH_SDL_SUPPORT

		SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER );
		m_pRenderinstance->m_Screen=SDL_SetVideoMode(m_pRenderinstance->m_Windowwidth,m_pRenderinstance->m_Windowheight,0,
			((m_pRenderinstance->m_Usedrenderer==UsedRenderer_Opengl) ? SDL_OPENGL : 0)
			| ((m_pRenderinstance->m_Windowmode==Window_Normal) ? SDL_RESIZABLE : 0)
			| ((m_pRenderinstance->m_Windowmode==Window_Fullscreen) ? SDL_FULLSCREEN : 0)
			);
			
		if (m_pRenderinstance->m_Usedrenderer==UsedRenderer_Opengl) {
			m_pRenderinstance->createRenderer(m_pRenderinstance->m_Usedrenderer,m_pRenderinstance->m_Windowwidth,m_pRenderinstance->m_Windowheight);
		}

		SDL_Event event;
		while (!m_pRenderinstance->m_ThreadQuit) {
			SDL_WaitEvent(&event);
	
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN: {
					if (!m_pRenderinstance->m_Interactive) break;
					if (!(event.button.button&SDL_BUTTON_LEFT) || !(event.button.state&SDL_PRESSED))
						break;
				
					float xx,yy;
					xx=((float)(event.button.x))/((float)(m_pRenderinstance->m_Windowwidth)*0.5f)-1.0f;
					yy=((float)(event.button.y))/((float)(m_pRenderinstance->m_Windowheight)*0.5f)-1.0f;
			
					m_pRenderinstance->m_Arcball.click(xx,-yy);
			
					m_pRenderinstance->m_OldRotation=m_pRenderinstance->getRotation();
			
					m_pRenderinstance->calculateViewmatrix();
				} break;

				case SDL_MOUSEMOTION: {
					if (!m_pRenderinstance->m_Interactive) break;
					if (!(event.motion.state&SDL_BUTTON_LEFT)) break;

					float xx,yy;
					xx=((float)(event.motion.x))/((float)(m_pRenderinstance->m_Windowwidth)*0.5f)-1.0f;
					yy=((float)(event.motion.y))/((float)(m_pRenderinstance->m_Windowheight)*0.5f)-1.0f;
			
					math::Quaternion quat;					
					m_pRenderinstance->m_Arcball.drag(xx,-yy,&quat);
					m_pRenderinstance->setRotation((m_pRenderinstance->m_OldRotation*quat).normalized());
					m_pRenderinstance->calculateViewmatrix();
				} break;

				case SDL_VIDEORESIZE: {
					int w=event.resize.w;
					int h=event.resize.h;
					m_pRenderinstance->m_Windowwidth=w;
					m_pRenderinstance->m_Windowheight=h;
					m_pRenderinstance->m_pRenderer->resize(0,0,w,h);
					m_pRenderinstance->perspective(w,h);
				} break;
				case SDL_QUIT:m_pRenderinstance->signalThreadEnd(); break;
				default:break;
			}
			
			if (m_pRenderinstance->m_pRenderer) {
				m_pRenderinstance->m_pRenderer->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,0.4f,0.4f,0.4f));
				if (m_pRenderinstance->m_pScene) {
					m_pRenderinstance->m_pScene->render();
				}
			}

			if (m_pRenderinstance->usedRendererType()==UsedRenderer_Opengl)
				SDL_GL_SwapBuffers();
			else
				SDL_Flip(m_pRenderinstance->m_Screen);
			}
		
		if (m_pRenderinstance->m_Usedrenderer==m_pRenderinstance->UsedRenderer_Opengl)
			if (m_pRenderinstance->m_pRenderer!=0) delete m_pRenderinstance->m_pRenderer; m_pRenderinstance->m_pRenderer=0;
		
		SDL_Quit();
		
#endif
	}

	boost::recursive_try_mutex& Renderinstance::mutex()
	{
		return *m_pMutex;
	}
	
	void Renderinstance::signalThreadEnd()
	{
		mutexScopedLock();
		m_ThreadQuit=true;
	}

	void Renderinstance::waitUntilWindowClosed()
	{
	}
	
	
	

	// Scene handling
	
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
	
	void Renderinstance::scene(video::Scene &rScene)
	{
		if (m_pScene==(&rScene)) return;

		mutexScopedLock();
		
		if ((m_pScene!=0) && (m_pScene!=m_pTestscene)) {
			delete m_pScene; m_pScene=0;
		}

		rScene.camera(m_Camera);
		if (m_pRenderer) rScene.renderer(*m_pRenderer);
		rScene.colorscale(m_Colorscale);
		
		m_pScene=&rScene;
	}
	
	
	
	
	// Camera handling
	
	void Renderinstance::calculateViewmatrix()
	{
		mutexScopedLock();

		m_Camera.viewmatrix().loadIdentity();
		m_Camera.viewmatrix().fromQuaternion(m_Rotation);
		m_Camera.viewmatrix().m_41=m_Position.x();
		m_Camera.viewmatrix().m_42=m_Position.y();
		m_Camera.viewmatrix().m_43=m_Position.z();
	}
	
	void Renderinstance::perspective(const int w,const int h)
	{
		mutexScopedLock();

		m_Camera.projmatrix().perspective(
			3.1415926535f*0.5f,
			((float)w/(float)h),
			1,100);
	}
	
}
