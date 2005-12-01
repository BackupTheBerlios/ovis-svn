#ifndef OVIS_MAINGLWIDGET_HH_INCLUDED
#define OVIS_MAINGLWIDGET_HH_INCLUDED

#include <QGLWidget>
#include "../base/fpscalc.hh"
#include "../math/arcball.hh"
#include "../math/quaternion.hh"
#include "../ogl/oglrenderer.hh"
#include "../video/camera.hh"
#include "../video/colorscale.hh"
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

		void calculateViewmatrix();

		void mousePressEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void wheelEvent(QWheelEvent *e);

		opengldrv::OGLRenderer *m_pOGLRenderer;
		base::Fpscalc m_FPS;
		video::Scene *m_pScene,*m_pTestscene;
		math::Arcball m_Arcball;
		math::Quaternion m_Rotation,m_OldRotation;
		math::Vector3f m_Position;
		video::Camera m_Camera;
		video::Colorscale m_Colorscale;
		RotationCube m_RotCube;
	};

}

#endif
