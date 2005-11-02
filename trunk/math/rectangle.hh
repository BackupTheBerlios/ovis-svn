#ifndef OVIS_MATH_RECTANGLE_HH_INCLUDED
#define OVIS_MATH_RECTANGLE_HH_INCLUDED

#include "../base/dll.hh"

namespace ovis {
namespace math {

class OVIS_API Rectangle
{
public:
	Rectangle();
	Rectangle(const Rectangle& src);
	Rectangle(const float l,const float r,const float t,const float b);
	virtual ~Rectangle() {}

	bool includes(const Rectangle& rect);

	inline float left() const { return m_Left; }
	inline float& left() { return m_Left; }
	inline void left(const float f) { m_Left=f; }

	inline float right() const { return m_Right; }
	inline float& right() { return m_Right; }
	inline void right(const float f) { m_Right=f; }

	inline float top() const { return m_Top; }
	inline float& top() { return m_Top; }
	inline void top(const float f) { m_Top=f; }
	
	inline float bottom() const { return m_Bottom; }
	inline float& bottom() { return m_Bottom; }
	inline void bottom(const float f) { m_Bottom=f; }
protected:
	float m_Left,m_Right,m_Top,m_Bottom;
};

}
}

#endif
