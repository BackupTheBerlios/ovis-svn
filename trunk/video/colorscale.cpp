#include <vector>
//#include <iostream>
#include "colorscale.hh"

namespace ovis {
namespace video {

	struct Colorscale::Colorlist {
		std::vector < std::pair<float,ovis_uint32> > m_Colors;
	};



	Colorscale::Colorscale():m_pColors(new Colorlist)
	{
	}

	Colorscale::~Colorscale()
	{
		delete m_pColors;
	}

	void Colorscale::addColor(const float position,const ovis_uint8 a,const ovis_uint8 b,const ovis_uint8 g,const ovis_uint8 r)
	{
		addColor(position,(((ovis_uint32)a)<<24)|(((ovis_uint32)b)<<16)|(((ovis_uint32)g)<<8)|((ovis_uint32)r));
	}

	void Colorscale::addColor(const float position,const ovis_uint32 argb)
	{
		m_pColors->m_Colors.push_back(std::pair<float,ovis_uint32>(position,argb));
	}

	ovis_uint32 Colorscale::lerpColor(const float position) const
	{
		ovis_uint8 a,b,g,r;
		lerpColor(position,a,b,g,r);

		return (((ovis_uint32)a)<<24)|(((ovis_uint32)b)<<16)|(((ovis_uint32)g)<<8)|((ovis_uint32)r);
	}

	void Colorscale::lerpColor(const float position,ovis_uint8 &a,ovis_uint8 &b,ovis_uint8 &g,ovis_uint8 &r) const
	{
		std::vector < std::pair<float,ovis_uint32> > ::iterator itlast,it=m_pColors->m_Colors.begin();
		itlast=it;

		while (it!=m_pColors->m_Colors.end()) {

			if ((it+1)==m_pColors->m_Colors.end()) {
				ovis_uint32 col=(*it).second;
				a=(col>>24)&0xff;
				b=(col>>16)&0xff;
				g=(col>>8)&0xff;
				r=col&0xff;
				return;
			} else {
				if ((*(it+1)).first>position) {
					std::vector < std::pair<float,ovis_uint32> > ::iterator it2=it+1;
					float relpos=position-(*it).first;
					float range=(*it2).first-(*it).first;
	
					//std::cout << "position " << position << " (*it).first " << (*it).first << " range " << range << "\n";
	
					relpos=(range==0) ? 0 : relpos/range;
					if (relpos<0) relpos=0;
	
					ovis_uint32 col1=(*it).second;
					ovis_uint32 col2=(*it2).second;
	
					//std::cout << std::hex << "col1 " << col1 << " col2 " << col2 << "\n";
					
					int a1=(col1>>24)&0xff,a2=(col2>>24)&0xff;
					int b1=(col1>>16)&0xff,b2=(col2>>16)&0xff;
					int g1=(col1>>8)&0xff,g2=(col2>>8)&0xff;
					int r1=col1&0xff,r2=col2&0xff;
	
					//std::cout << std::hex << "r1 " << r1 << " r2 " << r2 << " relpos " << relpos << "\n";
	
					a1+=(int)( (float)(a2-a1)*relpos );
					b1+=(int)( (float)(b2-b1)*relpos );
					g1+=(int)( (float)(g2-g1)*relpos );
					r1+=(int)( (float)(r2-r1)*relpos );
	
					a=(ovis_uint8)(a1&0xff);
					b=(ovis_uint8)(b1&0xff);
					g=(ovis_uint8)(g1&0xff);
					r=(ovis_uint8)(r1&0xff);
					return;
				}
			}

			itlast=it;
			++it;
		}

		if (itlast!=m_pColors->m_Colors.end()) {
			ovis_uint32 col=(*itlast).second;
			a=(col>>24)&0xff;
			b=(col>>16)&0xff;
			g=(col>>8)&0xff;
			r=col&0xff;
		} else {
			a=b=g=r=0;
		}
	}

}
}
