//#include <boost/python.hpp>
#include "../ogl/oglrenderer.hh"
#include "renderinstance.hh"

namespace ovis {

namespace video {
	OVIS_API Scene* generateTestscene(Renderer *pRenderer,Colorscale *pColorscale);
}

	void Renderinstance::perspectiveViewport(const int w,const int h)
	{
		m_pRenderer->resize(0,0,w,h);
		perspective(w,h);
		m_Width=w;
		m_Height=h;
	}

	void Renderinstance::perspective(const int w,const int h)
	{
		m_Camera.projmatrix().perspective(
			3.1415926535f*0.5f,
			((float)w/(float)h),
			1,100);
	}

	void Renderinstance::calculateViewmatrix()
	{
		m_Camera.viewmatrix().loadIdentity();
		m_Camera.viewmatrix().fromQuaternion(m_Rotation);
		m_Camera.viewmatrix().m_41=m_Position.x();
		m_Camera.viewmatrix().m_42=m_Position.y();
		m_Camera.viewmatrix().m_43=m_Position.z();
	}
	
	void Renderinstance::updateScreen()
	{
	#ifdef COMPILE_WITH_SDL_SUPPORT
		if (m_Usedrenderer==UsedRenderer_Opengl)
			SDL_GL_SwapBuffers();
		else
			SDL_Flip(m_Screen);
	#endif
	}
	
	void Renderinstance::testinitRenderer(
		const UsedRenderer usedrenderer,
		const Windowmode windowmode,
		const int windowwidth,
		const int windowheight)
	{
		if (m_pRenderer!=0) return;
		
		m_pTestscene=video::generateTestscene(0,0);
		initRenderer(m_pTestscene,usedrenderer,windowmode,windowwidth,windowheight);
	}

	void Renderinstance::initRenderer(
		video::Scene *pScene,
		const UsedRenderer usedrenderer,
		const Windowmode windowmode,
		const int windowwidth,
		const int windowheight)
	{
		if (m_pRenderer!=0) return;

		m_Windowmode=Window_None;

		Windowmode wm=((usedrenderer==UsedRenderer_Opengl) && (windowmode==Window_None)) ? Window_Normal : windowmode;
		m_Usedrenderer=usedrenderer;
	
		m_Width=windowwidth;
		m_Height=windowheight;

		this->windowmode(wm);

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
	
		m_Camera.viewmatrix().m_43=m_Position.z()=3;
		
		if (pScene) scene(*pScene);
	
		m_pRenderer->resize(0,0,windowwidth,windowheight);
		perspective(windowwidth,windowheight);

	}
	
	void Renderinstance::windowmode(const Windowmode windowmode)
	{
		if (m_Windowmode==windowmode) return;
		if ((m_Usedrenderer==UsedRenderer_Opengl) && (m_Windowmode!=Window_None)) return;
	
	#ifdef COMPILE_WITH_SDL_SUPPORT
		m_Screen=0;
		
		switch (windowmode) {
			case Window_Normal:
				SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER );
				m_Screen=SDL_SetVideoMode(m_Width,m_Height,0,
				((m_Usedrenderer==UsedRenderer_Opengl) ? SDL_OPENGL : 0)
				| ((windowmode==Window_Normal) ? SDL_RESIZABLE : 0)
				);
			default:break;
		}
	#endif
		m_Windowmode=windowmode;
	}
	
	void Renderinstance::scene(video::Scene &rScene)
	{
		rScene.camera(m_Camera);
		rScene.renderer(*m_pRenderer);
		rScene.colorscale(m_Colorscale);
		
		m_pScene=&rScene;
	}

	void Renderinstance::endRenderer()
	{
		if (m_pTestscene) delete m_pTestscene; m_pTestscene=0;
		if (m_pRenderer) delete m_pRenderer; m_pRenderer=0;

	#ifdef COMPILE_WITH_SDL_SUPPORT
		if (m_Windowmode!=Window_None)
			SDL_Quit();
			
		m_Windowmode=Window_None;
	#endif
		m_pScene=0;
	}
	
	void Renderinstance::interactiveLoop()
	{
	#ifdef COMPILE_WITH_SDL_SUPPORT
		if (!m_pScene && (m_Windowmode==Window_None)) return;

		SDL_Event event;
		bool quit=false;
		while (!quit) {
			SDL_WaitEvent(&event);
	
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN: {
					if (!(event.button.button&SDL_BUTTON_LEFT) || !(event.button.state&SDL_PRESSED))
						break;
				
					float xx,yy;
					xx=((float)(event.button.x))/((float)(m_Width)*0.5f)-1.0f;
					yy=((float)(event.button.y))/((float)(m_Height)*0.5f)-1.0f;
			
					m_Arcball.click(xx,-yy);
			
					m_OldRotation=m_Rotation;
			
					calculateViewmatrix();
					updateScreen();
				} break;

				case SDL_MOUSEMOTION: {
					if (!(event.motion.state&SDL_BUTTON_LEFT)) break;

					float xx,yy;
					xx=((float)(event.motion.x))/((float)(m_Width)*0.5f)-1.0f;
					yy=((float)(event.motion.y))/((float)(m_Height)*0.5f)-1.0f;
			
					math::Quaternion quat;					
					m_Arcball.drag(xx,-yy,&quat);
					m_Rotation=(m_OldRotation*quat).normalized();
			
					calculateViewmatrix();
					updateScreen();
				} break;

				case SDL_VIDEORESIZE: {
					int w=event.resize.w;
					int h=event.resize.h;
					m_Width=w;
					m_Height=h;
					m_pRenderer->resize(0,0,w,h);
					perspective(w,h);
				} break;
				case SDL_QUIT:quit=true; break;
				default:break;
			}

			m_pRenderer->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,0.4f,0.4f,0.4f));
			m_pScene->render();

			updateScreen();
		}
	#endif
	}

	Renderinstance::Renderinstance():m_pRenderer(0),m_pScene(0),m_pTestscene(0)
	{
		m_Colorscale.addColor(0.00f,0x00FF0000); // red
		m_Colorscale.addColor(0.33f,0x0000FF00); // green
		m_Colorscale.addColor(0.66f,0x0000FFFF); // yellow
		m_Colorscale.addColor(1.00f,0x000000FF); // red

		m_Windowmode=Window_None;
	}

	Renderinstance::~Renderinstance()
	{
		endRenderer();
	}

}
