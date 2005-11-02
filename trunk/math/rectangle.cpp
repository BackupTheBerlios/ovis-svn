#include "rectangle.hh"

namespace ovis {
namespace math {

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(const Rectangle& src):m_Left(src.left()),m_Right(src.right()),m_Top(src.top()),m_Bottom(src.bottom())
{
}

Rectangle::Rectangle(const float l,const float r,const float t,const float b):m_Left(l),m_Right(r),m_Top(t),m_Bottom(b)
{
}

bool Rectangle::includes(const Rectangle& rect)
{
	return (rect.left()>=m_Left)&&(rect.right()<=m_Right)&&(rect.top()>=m_Top)&&(rect.bottom()<=m_Bottom);
}

}
}
