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
	QDesktopWidget *desktop = QApplication::desktop();
     	int dwidth  = desktop->width();
     	int dheight = desktop->height();
     	int width   = dwidth  * 7/10;
     	int height  = dheight * 7/10;

	QSettings settings("OpenVisualizer", "AppExpl");
	QPoint pos = settings.value("pos",  QPoint((dwidth-width)/2,(dheight-height)/2)).toPoint();
	QSize size = settings.value("size", QSize(width, height)).toSize();
	resize(size);
	move(pos);

	m_pTestGL=new TestGL(this,800,600);
	setCentralWidget(m_pTestGL);
}

Testwindow::~Testwindow()
{
	delete m_pTestGL;
}
