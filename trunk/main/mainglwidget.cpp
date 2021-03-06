#include <QMouseEvent>
#include <QWheelEvent>

#include "mainglwidget.hh"
#include "../base/dll.hh"
#include "../math/quaternion.hh"
#include "../video/coreenums.hh"
#include "../video/loadscenefromfile.hh"

namespace ovis {

namespace video {
	OVIS_API Scene* generateTestscene(Renderer &rRenderer,Colorscale *pColorscale);
};

	MainGLWidget::MainGLWidget(QWidget *pParent):QGLWidget(pParent),m_pOGLRenderer(0),m_pScene(0),m_pTestscene(0)
	{
		m_Colorscale.addColor(0.00f,0x00FF0000); // red
		m_Colorscale.addColor(0.33f,0x0000FF00); // green
		m_Colorscale.addColor(0.66f,0x0000FFFF); // yellow
		m_Colorscale.addColor(1.00f,0x000000FF); // red
	}

	MainGLWidget::~MainGLWidget()
	{
		if ((m_pTestscene!=0) && (m_pScene!=m_pTestscene)) delete m_pTestscene;
		if (m_pScene) delete m_pScene;
		if (m_pOGLRenderer) delete m_pOGLRenderer;
	}

	void MainGLWidget::perspective(const int w,const int h)
	{
		m_Camera.projmatrix().perspective(
			3.1415926535f*0.5f,
			((float)w/(float)h),
			1,100);
	}

	void MainGLWidget::orthogonal(const int w,const int h)
	{
		m_Camera.projmatrix().orthogonal(
			((float)w/(float)h)*4,4,
			1,100);
	}

	void MainGLWidget::initializeGL()
	{
		m_pOGLRenderer=new opengldrv::OGLRenderer(width(),height());
		//m_pTestscene=generateTestscene(*m_pOGLRenderer,&m_Colorscale);
		//m_pScene=m_pTestscene;
		m_pScene=video::loadSceneFromFile("../binaryDataFile.dat",*m_pOGLRenderer,&m_Colorscale);

		m_Camera.viewmatrix().m_43=m_Position.z()=3;

		m_pScene->camera(m_Camera);

		m_FPS.update();
	}

	void MainGLWidget::resizeGL(int w,int h)
	{
		glViewport(0,0,w,h);
		perspective(w,h);
		//orthogonal(w,h);
	}

	void MainGLWidget::paintGL()
	{
		m_pOGLRenderer->clear(video::Clear_Colorbuffer|video::Clear_Depthbuffer,base::Float4(0,0.4f,0.4f,0.4f));

		unsigned int w=width(),h=height();
		unsigned int rside=((w<h) ? w : h)/20;

		glViewport(0,0,w,h);
		if (m_pScene) m_pScene->render();


		if (rside>10) {
			m_pOGLRenderer->clear(video::Clear_Depthbuffer,base::Float4(0,0.4f,0.4f,0.4f));

			glViewport(w-rside-10,h-rside-10,rside,rside);
			m_RotCube.render(m_Camera);
		}

		glDisable(GL_LIGHTING);
		/*QString fpstext; fpstext.setNum(m_FPS.fps()); fpstext+=" fps";
		renderText(5+x(),5+y(),fpstext);*/
		m_FPS.update();
	}

	void MainGLWidget::mousePressEvent(QMouseEvent *e)
	{
		float xx,yy;
		xx=((float)(e->x()))/((float)(width())*0.5f)-1.0f;
		yy=((float)(e->y()))/((float)(height())*0.5f)-1.0f;

		m_Arcball.click(xx,-yy);

//		m_OldViewmatrix=m_Camera.viewmatrix();
//		m_OldViewmatrix.m_43=0;
		m_OldRotation=m_Rotation;

		calculateViewmatrix();
		updateGL();
	}

	void MainGLWidget::mouseMoveEvent(QMouseEvent *e)
	{
		float xx,yy;
		xx=((float)(e->x()))/((float)(width())*0.5f)-1.0f;
		yy=((float)(e->y()))/((float)(height())*0.5f)-1.0f;

		math::Quaternion quat;
		//math::Matrix4f m;
		
		m_Arcball.drag(xx,-yy,&quat);
		//m.fromQuaternion(quat);

		m_Rotation=(m_OldRotation*quat).normalized();

		//m_Camera.viewmatrix()=m_OldViewmatrix*m;
		//m_Camera.viewmatrix().m_43=m_ViewDistance;

		calculateViewmatrix();
		updateGL();
	}

	void MainGLWidget::wheelEvent(QWheelEvent *e)
	{
		m_Position.z()+=0.05f*((float)e->delta())/120.0f; 
		if (m_Position.z()<1.0f) m_Position.z()=1.0f;
		//m_Camera.viewmatrix().m_43=m_ViewDistance;

		calculateViewmatrix();
		updateGL();
	}

	void MainGLWidget::calculateViewmatrix()
	{
		m_Camera.viewmatrix().loadIdentity();
		m_Camera.viewmatrix().fromQuaternion(m_Rotation);
		m_Camera.viewmatrix().m_41=m_Position.x();
		m_Camera.viewmatrix().m_42=m_Position.y();
		m_Camera.viewmatrix().m_43=m_Position.z();
	}

}
