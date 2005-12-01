#ifndef OVIS_MAINGLWIDGET_HH_INCLUDED
#define OVIS_MAINGLWIDGET_HH_INCLUDED

#include <QGLWidget>
#include "../base/fpscalc.hh"
#include "../math/arcball.hh"
#include "../ogl/oglrenderer.hh"
#include "../video/camera.hh"
#include "../video/scene.hh"
#include "rotationcube.hh"

namespace ovis {

	class MainGLWidget:public QGLWidget
	{
	public:
		MainGLWidget(QWidget *pParent);
		~MainGLWidget();

	protected:
		void initializeGL();
		void resizeGL(int w,int h);
		void paintGL();

		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void wheelEvent(QWheelEvent *e);

		opengldrv::OGLRenderer *m_pOGLRenderer;
		base::Fpscalc m_FPS;
		video::Scene *m_pScene,*m_pTestscene;
		math::Arcball m_Arcball;
		math::Matrix4f m_OldViewmatrix;
		float m_ViewDistance;
		video::Camera m_Camera;
		RotationCube m_RotCube;
	};

}

#endif
