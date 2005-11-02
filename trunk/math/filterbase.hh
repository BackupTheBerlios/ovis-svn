#ifndef OVIS_BASE_FILTERBASE_HH_INCLUDED
#define OVIS_BASE_FILTERBASE_HH_INCLUDED

namespace ovis {
namespace math {

	class FilterBase
	{
	protected:
		double m_Width;

		FilterBase(double newwidth):m_Width(newwidth) {}
	public:
		virtual ~FilterBase() {}

		double width() const			{ return m_Width; }
		void   width(double newwidth)	{ m_Width=newwidth; }

		virtual double filter(const double value) const=0;
	};

}
}

#endif
