#include "arcball.hh"

namespace ovis {
namespace math {

//Arcball sphere constants:
//Diameter is       2.0f
//Radius is         1.0f
//Radius squared is 1.0f

# define Epsilon 1.0e-5

void Arcball::mapToSphere(const float x,const float y,Vector3f& newvec) const
{
    float length=x*x+y*y;

    //If the point is mapped outside of the sphere... (length > radius squared)
    if (length > 1.0f)
    {
        float norm;

        //Compute a normalizing factor (radius / sqrt(length))
        norm    = 1.0f / sqrtf(length);

        //Return the "normalized" vector, a point on the sphere
        newvec.x() = x*norm;
        newvec.y() = y*norm;
        newvec.z() = 0.0f;
    }
    else    //Else it's on the inside
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        newvec.x() = x;
        newvec.y() = y;
        newvec.z() = sqrtf(1.0f - length);
    }
}

//Create/Destroy
Arcball::Arcball():m_StVec(0,0,0),m_EnVec(0,0,0)
{
}

void Arcball::click(const float x,const float y)
{
	mapToSphere(x,y, m_StVec);
}

void Arcball::drag(const float x,const float y, Quaternion* newrot)
{
	mapToSphere(x,y, m_EnVec);

	if (newrot) {
		Vector3f perp=m_StVec^m_EnVec;

		if (perp.length() > Epsilon) {
			newrot->x() = perp.x();
			newrot->y() = perp.y();
			newrot->z() = perp.z();
			newrot->w()= m_StVec*m_EnVec;
		} else {
			newrot->x() = 
			newrot->y() = 
			newrot->z() = 
			newrot->w() = 0.0f;
		}
	}
}

}
}
