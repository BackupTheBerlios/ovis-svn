#include "vectors.hh"
#include "quaternion.hh"
#include "matrix.hh"

namespace ovis {
namespace math {

	Vector2f& Vector2f::normalize()
	{
		float l=length();
		
		if (l==0.0f) { m_Floats[0]=0.0f; m_Floats[1]=0.0f; }
		else { l=1.0f/l; m_Floats[0]*=l; m_Floats[1]*=l; }
		
		return *this;
	}

	Vector2f Vector2f::normalizedVector() const
	{
		float l=length();

		if (l==0.0f) return Vector2f(0,0);
		else {
			l=1.0f/l;
			return Vector2f(m_Floats[0]*l,m_Floats[1]*l);
		}

		return *this;
	}

	Vector3f& Vector3f::normalize()
	{
		float l=length();
		
		if (l==0.0f) { m_Floats[0]=0.0f; m_Floats[1]=0.0f; m_Floats[2]=0.0f; }
		else { l=1.0f/l; m_Floats[0]*=l; m_Floats[1]*=l; m_Floats[2]*=l; }
		
		return *this;
	}

	Vector3f Vector3f::normalizedVector() const
	{
		float l=length();

		if (l==0.0f) return Vector3f(0,0,0);
		else {
			l=1.0f/l;
			return Vector3f(m_Floats[0]*l,m_Floats[1]*l,m_Floats[2]*l);
		}

		return *this;
	}

	Vector4f& Vector4f::normalize()
	{
		float l=length();
		
		if (l==0.0f) { m_Floats[0]=0.0f; m_Floats[1]=0.0f; m_Floats[2]=0.0f; m_Floats[3]=0.0f; }
		else { l=1.0f/l; m_Floats[0]*=l; m_Floats[1]*=l; m_Floats[2]*=l; m_Floats[3]*=l; }
		
		return *this;
	}

	Vector4f Vector4f::normalizedVector() const
	{
		float l=length();

		if (l==0.0f) return Vector4f(0,0,0,0);
		else {
			l=1.0f/l;
			return Vector4f(m_Floats[0]*l,m_Floats[1]*l,m_Floats[2]*l,m_Floats[3]*l);
		}

		return *this;
	}

	Vector3f Vector3f::rotatedX(const float angle) const
	{
		if (angle==0) return *this;

		float sinAngle=(float)sin(angle);
		float cosAngle=(float)cos(angle);

		return Vector3f(x(),cosAngle*y()-sinAngle*z(),sinAngle*y()+cosAngle*z());
	}

	Vector3f Vector3f::rotatedY(const float angle) const
	{
		if (angle==0) return *this;

		float sinAngle=(float)sin(angle);
		float cosAngle=(float)cos(angle);

		return Vector3f(cosAngle*x()+sinAngle*z(),y(),-sinAngle*x()+cosAngle*z());
	}

	Vector3f Vector3f::rotatedZ(const float angle) const
	{
		if (angle==0) return *this;

		float sinAngle=(float)sin(angle);
		float cosAngle=(float)cos(angle);

		return Vector3f(cosAngle*x()-sinAngle*y(),sinAngle*x()+cosAngle*y(),z());
	}

}
}
