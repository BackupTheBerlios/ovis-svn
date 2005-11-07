#ifndef RTEST_TESTWINDOW_HH_INCLUDED
#define RTEST_TESTWINDOW_HH_INCLUDED

#include <qapplication.h>
#include <qmainwindow.h>
#include <qgl.h>

#include "../ogl/oglrenderer.hh"
#include "../video/testtorus.hh"

class TestGL:public QGLWidget
{
public:
	TestGL(QWidget *pParent,int w,int h);
	~TestGL();
	
protected:
	void initializeGL();
	void resizeGL(int w,int h);
	void paintGL();

	ovis::opengldrv::OGLRenderer *m_pOGLRenderer;
	ovis::video::Testtorus *m_pTesttorus;
	ovis::video::Camera m_Camera;
};

class Testwindow:public QMainWindow
{
	Q_OBJECT

public:
	Testwindow(QApplication *pApplication);
	~Testwindow();

protected:

	QApplication *m_pApplication;
	TestGL *m_pTestGL;
};

#endif
