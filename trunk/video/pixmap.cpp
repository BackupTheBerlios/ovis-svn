#include "../math/vectors.hh"
#include <math.h>
#include <memory.h>
#include "pixmap.hh"

namespace ovis {
namespace video {

	float add(const float a,const float b) { return a+b; }

	ovis_uint8 add(const ovis_uint8 a,const ovis_uint8 b)
	{
		int val=((int)a)+((int)b);
		if (val<0) val=0; if (val>255) val=255;
		return static_cast<ovis_uint8>( val );
	}

	float mul(const float a,const float b) { return a*b; }

	ovis_uint8 mul(const ovis_uint8 a,const ovis_uint8 b)
	{
		int val=((int)a)*((int)b)/255;
		if (val<0) val=0; if (val>255) val=255;
		return static_cast<ovis_uint8>( val );
	}

	void convert(ovis_uint8& b,const float f)
	{
		convert(b, static_cast<int>( f*255.0f ) );
	}

	void convert(float &f,const ovis_uint8 b)
	{
		f=((float)b)/255.0f;
	}

	void convert(ovis_uint8& b,const int i)
	{
		b=static_cast<ovis_uint8>( (i<0) ? 0 : ((i>255) ? 255 : i) );
	}

	Weighttable::Weighttable(const math::FilterBase &rFilter,const ovis_uint32 destSize,
												const ovis_uint32 srcSize,const double scale)
	{
		ovis_uint32 u;
		double _width;
		double fscale=1.0;
		double filterWidth=rFilter.width();

		if(scale<1.0) {
			// minification
			_width=filterWidth/scale; 
			fscale=scale; 
		} else {
			// magnification
			_width=filterWidth; 
		}

		// allocate a new line contributions structure
		//
		// window size is the number of sampled pixels
		m_WindowSize=2*(int)ceil(_width)+1; 
		m_LineLength=destSize; 
		// allocate list of contributions 
		m_WeightTable=new Contribution[m_LineLength];
		for(u=0;u<m_LineLength;++u) {
			// allocate contributions for every pixel
			m_WeightTable[u].m_pWeights=new double[m_WindowSize];
		}

		int rightmax=((int)srcSize)-1;

		for(u=0;u<m_LineLength;++u) {
			// scan through line of contributions
			double center=(double)u/scale;   // reverse mapping
			// find the significant edge points that affect the pixel
			int left=(int)floor(center-_width); if (left<0) left=0;
			int right=(int)ceil(center+_width); if (right>rightmax) right=rightmax; 

			// cut edge points to fit in filter window in case of spill-off
			if((right-left+1)>int(m_WindowSize)) {
				if(left<(int(srcSize-1)/2)) {
					++left;
				} else {
					--right; 
				}
			}

			m_WeightTable[u].m_Left=left; 
			m_WeightTable[u].m_Right=right;

			int src=0;
			double totalWeight=0.0;  // zero sum of weights
			for(src=left;src<=right;++src) {
				// calculate weights
				double weight=fscale*rFilter.filter(fscale*(center-(double)src));
				m_WeightTable[u].m_pWeights[src-left]=weight;
				totalWeight+=weight;
			}
			if((totalWeight>0) && (totalWeight!=1)) {
				// normalize weight of neighbouring points
				for(src=left;src<=right;++src) {
					// normalize point
					m_WeightTable[u].m_pWeights[src-left]/=totalWeight; 
				}
			}
	} 
	}

	Weighttable::~Weighttable() {
		for(ovis_uint32 u=0;u<m_LineLength;++u) {
			// free contributions for every pixel
			delete [] m_WeightTable[u].m_pWeights;
		}
		// free list of pixels contributions
		delete [] m_WeightTable;
	}

}
}
