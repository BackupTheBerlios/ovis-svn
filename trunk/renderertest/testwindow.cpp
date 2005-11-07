#include "testwindow.hh"
#include "../video/renderer.hh"



TestGL::TestGL(QWidget *pParent,int w,int h):QGLWidget(pParent),m_pOGLRenderer(0),m_pTesttorus(0),m_Camera("cam1")
{
	m_Camera.projmatrix().perspective(
			90,
			(float)(w)/(float)(h),
			1,100);
	m_Camera.position(0,0,-3);
}

TestGL::~TestGL()
{
	if (m_pTesttorus) delete m_pTesttorus;
	if (m_pOGLRenderer) delete m_pOGLRenderer;

}

void TestGL::initializeGL()
{
	m_pOGLRenderer=new ovis::opengldrv::OGLRenderer;
	m_pTesttorus=new ovis::video::Testtorus(*m_pOGLRenderer,"testtorus",1.4f,0.5f,30);
	m_pOGLRenderer->init(width(),height(),ovis::video::Pixelformat_ARGB8888,ovis::video::Pixelformat_D16,0);
}

void TestGL::resizeGL(int w,int h)
{
}

void TestGL::paintGL()
{
	m_pOGLRenderer->clear(ovis::video::Clear_Colorbuffer|ovis::video::Clear_Depthbuffer,
				ovis::base::Float4(0,0.5f,0.5f,0.5f));
	m_pOGLRenderer->renderScene(m_Camera,*m_pTesttorus);
}




Testwindow::Testwindow(QApplication *pApplication):m_pApplication(pApplication)
{

	unsigned int dw=m_pApplication->desktop()->width();
	unsigned int dh=m_pApplication->desktop()->height();

	unsigned int w=dw*7/10;
	unsigned int h=dh*7/10;

	move((dw-w)/2,(dh-h)/2);
	resize(w,h);
	
	m_pTestGL=new TestGL(this,w,h);
	setCentralWidget(m_pTestGL);
}

Testwindow::~Testwindow()
{
	delete m_pTestGL;
}
