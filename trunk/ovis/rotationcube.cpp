#include <GL/gl.h>
#include "../math/matrix.hh"
#include "../math/quaternion.hh"
#include "rotationcube.hh"

namespace ovis {

	RotationCube::RotationCube()
	{
	}

	RotationCube::~RotationCube()
	{
	}

	void RotationCube::render(const video::Camera &camera)
	{

		const float *pF;
		math::Quaternion quat;
		quat.fromMatrix(camera.viewmatrix());

		math::Matrix4f modelview;
		modelview.fromQuaternion(quat);
		{
			math::Matrix4f m;
			m.translate(0,0,3);
			modelview*=m;
		}



		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);



		// identity matrix to disable light transformation
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		float lpos[3]={-30,-3,-30};
		glLightfv(0,GL_POSITION,lpos);
		float ldif[3]={1,1,1};
		glLightfv(0,GL_DIFFUSE,ldif);
		glLightf(0,GL_CONSTANT_ATTENUATION,1);



		glMatrixMode(GL_PROJECTION);
		math::Matrix4f p;
		p.orthogonal(4,4,1,100);
		pF=p;
		glLoadMatrixf(pF);

		glMatrixMode(GL_MODELVIEW);
		pF=modelview;
		glLoadMatrixf(pF);



		const float sidelength=1;
		const float nrmlength=-0.57735026918962576451f;

		glBegin(GL_TRIANGLES);


		glColor3f(0,0,1);

		// Back

		glNormal3f(-nrmlength,-nrmlength,-nrmlength);	glVertex3f(-sidelength,-sidelength,-sidelength);
		glNormal3f(-nrmlength, nrmlength,-nrmlength);	glVertex3f(-sidelength, sidelength,-sidelength);
		glNormal3f( nrmlength, nrmlength,-nrmlength);	glVertex3f( sidelength, sidelength,-sidelength);

		glNormal3f( nrmlength, nrmlength,-nrmlength);	glVertex3f( sidelength, sidelength,-sidelength);
		glNormal3f( nrmlength,-nrmlength,-nrmlength);	glVertex3f( sidelength,-sidelength,-sidelength);
		glNormal3f(-nrmlength,-nrmlength,-nrmlength);	glVertex3f(-sidelength,-sidelength,-sidelength);


		// Front

		glNormal3f(-nrmlength,-nrmlength, nrmlength);	glVertex3f(-sidelength,-sidelength, sidelength);
		glNormal3f( nrmlength,-nrmlength, nrmlength);	glVertex3f( sidelength,-sidelength, sidelength);
		glNormal3f( nrmlength, nrmlength, nrmlength);	glVertex3f( sidelength, sidelength, sidelength);

		glNormal3f( nrmlength, nrmlength, nrmlength);	glVertex3f( sidelength, sidelength, sidelength);
		glNormal3f(-nrmlength, nrmlength, nrmlength);	glVertex3f(-sidelength, sidelength, sidelength);
		glNormal3f(-nrmlength,-nrmlength, nrmlength);	glVertex3f(-sidelength,-sidelength, sidelength);


		glColor3f(0,1,0);

		// Left

		glNormal3f(-nrmlength,-nrmlength,-nrmlength);	glVertex3f(-sidelength,-sidelength,-sidelength);
		glNormal3f(-nrmlength,-nrmlength, nrmlength);	glVertex3f(-sidelength,-sidelength, sidelength);
		glNormal3f(-nrmlength, nrmlength, nrmlength);	glVertex3f(-sidelength, sidelength, sidelength);

		glNormal3f(-nrmlength, nrmlength, nrmlength);	glVertex3f(-sidelength, sidelength, sidelength);
		glNormal3f(-nrmlength, nrmlength,-nrmlength);	glVertex3f(-sidelength, sidelength,-sidelength);
		glNormal3f(-nrmlength,-nrmlength,-nrmlength);	glVertex3f(-sidelength,-sidelength,-sidelength);


		// Right

		glNormal3f( nrmlength,-nrmlength,-nrmlength);	glVertex3f( sidelength,-sidelength,-sidelength);
		glNormal3f( nrmlength, nrmlength,-nrmlength);	glVertex3f( sidelength, sidelength,-sidelength);
		glNormal3f( nrmlength, nrmlength, nrmlength);	glVertex3f( sidelength, sidelength, sidelength);

		glNormal3f( nrmlength, nrmlength, nrmlength);	glVertex3f( sidelength, sidelength, sidelength);
		glNormal3f( nrmlength,-nrmlength, nrmlength);	glVertex3f( sidelength,-sidelength, sidelength);
		glNormal3f( nrmlength,-nrmlength,-nrmlength);	glVertex3f( sidelength,-sidelength,-sidelength);


		glColor3f(1,0,0);

		// Bottom

		glNormal3f(-nrmlength,-nrmlength,-nrmlength);	glVertex3f(-sidelength,-sidelength,-sidelength);
		glNormal3f( nrmlength,-nrmlength,-nrmlength);	glVertex3f( sidelength,-sidelength,-sidelength);
		glNormal3f( nrmlength,-nrmlength, nrmlength);	glVertex3f( sidelength,-sidelength, sidelength);

		glNormal3f( nrmlength,-nrmlength, nrmlength);	glVertex3f( sidelength,-sidelength, sidelength);
		glNormal3f(-nrmlength,-nrmlength, nrmlength);	glVertex3f(-sidelength,-sidelength, sidelength);
		glNormal3f(-nrmlength,-nrmlength,-nrmlength);	glVertex3f(-sidelength,-sidelength,-sidelength);


		// Top

		glNormal3f(-nrmlength, nrmlength,-nrmlength);	glVertex3f(-sidelength, sidelength,-sidelength);
		glNormal3f( nrmlength, nrmlength, nrmlength);	glVertex3f( sidelength, sidelength, sidelength);
		glNormal3f( nrmlength, nrmlength,-nrmlength);	glVertex3f( sidelength, sidelength,-sidelength);

		glNormal3f(-nrmlength, nrmlength, nrmlength);	glVertex3f(-sidelength, sidelength, sidelength);
		glNormal3f( nrmlength, nrmlength, nrmlength);	glVertex3f( sidelength, sidelength, sidelength);
		glNormal3f(-nrmlength, nrmlength,-nrmlength);	glVertex3f(-sidelength, sidelength,-sidelength);




		glEnd();

		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

	}

}
