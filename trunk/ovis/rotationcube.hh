#ifndef OVIS_ROTATION_CUBE_HH_INCLUDED
#define OVIS_ROTATION_CUBE_HH_INCLUDED

#include "../video/camera.hh"

namespace ovis {

	class RotationCube
	{
	public:
		RotationCube();
		~RotationCube();

		void render(const video::Camera &camera);
	protected:
	};

}

#endif
