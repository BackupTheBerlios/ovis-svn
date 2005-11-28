#include <QMouseEvent>
#include <QWheelEvent>

#include "mainglwidget.hh"
#include "../math/quaternion.hh"
#include "../video/coreenums.hh"

namespace ovis {

namespace video {
	Scene* generateTestscene(Renderer &rRenderer);
};

	MainGLWidget::MainGLWidget(QWidget *pParent):QGLWidget(pParent),m_pOGLRenderer(0),m_pScene(0),m_ViewDistance(3)
	{
	}

	MainGLWidget::~MainGLWidget()
	{
		if ((m_pTestscene!=0) && (m_pScene!=m_pTestscene)) delete m_pTestscene;
		if (m_pScene) delete m_pScene;
		if (m_pOGLRenderer) delete m_pOGLRenderer;
	}


	void MainGLWidget::initializeGL()
	{
		m_pOGLRenderer=new opengldrv::OGLRenderer(width(),height());
		m_pTestscene=generateTestscene(*m_pOGLRenderer);
		m_pScene=m_pTestscene;

		//m_pScene->camera().viewmatrix().translate(0,0,3);
		//math::Quaternion quat;
		//quat.fromRotAxisAndAngle(math::Vector3f(1,0,0),1);
		//m_pScene->camera().viewmatrix().fromQuaternion(quat);
		m_pScene->camera().viewmatrix().m_43=m_ViewDistance;
		m_pScene->camera().projmatrix().perspective(
			3.1415926535f*0.5f,
			(float)(width())/(float)(height()),
			1,100);

		m_FPS.update();
	}

	void MainGLWidget::resizeGL(int w,int h)
	{
		glViewport(0,0,w,h);
		m_pScene->camera().projmatrix().perspective(
			3.1415926535f*0.5f,
			((float)w/(float)h),
			1,100);
	}

	void MainGLWidget::paintGL()
	{
		m_pOGLRenderer->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,0.4f,0.4f,0.4f));

		if (m_pScene) m_pScene->render();

		glDisable(GL_LIGHTING);
		QString fpstext; fpstext.setNum(m_FPS.fps()); fpstext+=" fps";
		renderText(5+x(),5+y(),fpstext);
		m_FPS.update();
	}

	void MainGLWidget::mousePressEvent(QMouseEvent *e)
	{
		float xx,yy;
		xx=((float)(e->x()))/((float)(width())*0.5f)-1.0f;
		yy=((float)(e->y()))/((float)(height())*0.5f)-1.0f;

		m_Arcball.click(xx,-yy);

		m_OldViewmatrix=m_pScene->camera().viewmatrix();
		m_OldViewmatrix.m_43=0;

		updateGL();
	}

	void MainGLWidget::mouseMoveEvent(QMouseEvent *e)
	{
		float xx,yy;
		xx=((float)(e->x()))/((float)(width())*0.5f)-1.0f;
		yy=((float)(e->y()))/((float)(height())*0.5f)-1.0f;

		math::Quaternion quat;
		math::Matrix4f m;
		
		m_Arcball.drag(xx,-yy,&quat);
		m.fromQuaternion(quat);

		m_pScene->camera().viewmatrix()=m_OldViewmatrix*m;
		m_pScene->camera().viewmatrix().m_43=m_ViewDistance;

		updateGL();
	}

	void MainGLWidget::wheelEvent(QWheelEvent *e)
	{
		m_ViewDistance+=0.05f*((float)e->delta())/120.0f;
		if (m_ViewDistance<1.0f) m_ViewDistance=1.0f;
		m_pScene->camera().viewmatrix().m_43=m_ViewDistance;

		updateGL();
	}

}
