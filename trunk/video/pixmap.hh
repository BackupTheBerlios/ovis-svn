#ifndef OVIS_VIDEO_PIXMAP_HH_INCLUDED
#define OVIS_VIDEO_PIXMAP_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/itypes.hh"
#include "../math/filterbase.hh"
#include "../math/vectors.hh"
#include "picbuffer.hh"

namespace ovis {
namespace video {

	class OVIS_API Weighttable
	{
	protected:
		struct Contribution {
			double *m_pWeights;
			int m_Left,m_Right;   
		};

		Contribution *m_WeightTable;
		ovis_uint32 m_WindowSize;
		ovis_uint32 m_LineLength;
	public:
		Weighttable(const math::FilterBase &rFilter,const ovis_uint32 destSize,const ovis_uint32 srcSize,const double scale);
		~Weighttable();

		inline double weight(const int destPos,const int srcPos) const {
			return m_WeightTable[destPos].m_pWeights[srcPos];
		}

		inline int leftBoundary(const int destPos) const {
			return m_WeightTable[destPos].m_Left;
		}

		inline int rightBoundary(const int destPos) const {
			return m_WeightTable[destPos].m_Right;
		}
	};

	OVIS_API float add(const float a,const float b);
	OVIS_API ovis_uint8 add(const ovis_uint8 a,const ovis_uint8 b);

	OVIS_API float mul(const float a,const float b);
	OVIS_API ovis_uint8 mul(const ovis_uint8 a,const ovis_uint8 b);

	inline void convert(float& f,const float _f) { f=_f; }

	OVIS_API void convert(ovis_uint8& b,const float f);
	OVIS_API void convert(ovis_uint8& b,const int i);
	OVIS_API void convert(float &f,const ovis_uint8 b);

	enum Channels {
		Color_redChannel=0,
		Color_greenChannel=1,
		Color_blueChannel=2,
		Color_alphaChannel=3
	};

	template <typename T>
	struct Color
	{
		T m_Channels[4];

		Color() {}
		Color(const T r,const T g,const T b,const T a) {
			m_Channels[Color_redChannel]=r;
			m_Channels[Color_greenChannel]=g;
			m_Channels[Color_blueChannel]=b;
			m_Channels[Color_alphaChannel]=a;
		}
		void copy(const Color& src) { for (int i=0;i<4;++i) m_Channels[i]=src.m_Channels[i]; }
	};

	class Weighttable;

	template <typename T>
	class Pixmap:public Picbuffer
	{
	public:
		Pixmap(const ovis_uint32 width,const ovis_uint32 height);
		Pixmap(const Picbuffer& picbuffer);
		Pixmap(const Pixmap<T>& src);
		virtual ~Pixmap();
		
		inline Pixmap<T>& operator =(const Pixmap<T>& src) { clone(src); return *this; }

		inline void clone(const base::Datablock& datablock) { (static_cast<base::Datablock*>(this))->clone(datablock); }
		void clone(const Picbuffer& picbuffer);
		void clone(const Pixmap<T>& src);

		void create(const ovis_uint32 width,const ovis_uint32 height);
		void clear(const T r,const T g,const T b,const T a);
		void clear(const Color<T>& color);

		inline const Color<T>* pixels() const { return m_pPixel; }
		inline Color<T>* pixels() { return m_pPixel; }
		inline ovis_uint32 width() const { return m_Width; }
		inline ovis_uint32 height() const { return m_Height; }
		inline ovis_uint32 bytesPerPixel() const { return numRGBALayoutChannels(rgbalayout())*sizeof(T); }
		inline ovis_uint32 stride() const { return bytesPerPixel()*width(); }
		inline RGBALayouts rgbalayout() const { return RGBALayout_RGBA; }

		void flip();

		void pixel(const ovis_uint32 x,const ovis_uint32 y,const Color<T>& color);
		Color<T>& pixel(const ovis_uint32 x,const ovis_uint32 y) const;
		Color<T> pixel(const ovis_uint32 x,const ovis_uint32 y);

		void filteredRescale(const Pixmap<T>& src,const math::FilterBase& resamplingFilter);
		void filteredRescale(const ovis_uint32 newwidth,const ovis_uint32 newheight,const math::FilterBase& resamplingFilter);

		//! Generates a normal map out of a grayscale bumpmap.
		/*!
		@param bumpmap The grayscale bumpmap.
		@param intensity Intensity of the normal map to be created. The higher the intensity,
		the stronger the "bumps". */
		void generateNormalMap(const Pixmap<T> &bumpmap,const float intensity=1.0f,const bool heightInAlpha=false);
	protected:
		void estimateChanneldatatype();
		static void horizontalFilter(const math::FilterBase &filter,const Pixmap<T> &src,Pixmap &dest);
		static void verticalFilter(const math::FilterBase &filter,const Pixmap<T> &src,Pixmap &dest);

		ovis_uint32 m_Width,m_Height;
		Color<T> *m_pPixel;
	};

	template <typename T>
	void Pixmap<T>::estimateChanneldatatype()
	{
		unsigned int chsize=sizeof(T);
		bool chsigned=T(-1)<T(0);
		bool chfloat=(float(T(0.5f))) != 0.5f;

		if (chfloat) {
			if (chsize==4) m_Channeldatatype=Channeldatatype_Float32;
		} else {
			if (chsigned) {
				if (chsize==1) m_Channeldatatype=Channeldatatype_Int8;
				else if (chsize==2) m_Channeldatatype=Channeldatatype_Int16;
			} else {
				if (chsize==1) m_Channeldatatype=Channeldatatype_Uint8;
				else if (chsize==2) m_Channeldatatype=Channeldatatype_Uint16;
			}
		}
	}

	template <typename T>
	Pixmap<T>::Pixmap(const ovis_uint32 width,const ovis_uint32 height):m_pPixel(0)
	{
		estimateChanneldatatype();
		create(width,height);
	}

	template <typename T>
	Pixmap<T>::Pixmap(const Pixmap<T>& src):m_pPixel(0)
	{
		estimateChanneldatatype();
		clone(src);
	}

	template <typename T>
	Pixmap<T>::Pixmap(const Picbuffer& src):m_pPixel(0)
	{
		estimateChanneldatatype();
		clone(src);
	}

	template <typename T>
	void Pixmap<T>::clone(const Picbuffer& picbuffer)
	{
		create(picbuffer.width(),picbuffer.height());

		if ((picbuffer.rgbalayout()==RGBALayout_RGBA) && (picbuffer.channeldatatype()==channeldatatype()))
			Datablock::clone(picbuffer);
		else {
			int rofs=0,gofs=1,bofs=2,aofs=3,numchannels=0;
			switch (picbuffer.rgbalayout()) {
				case RGBALayout_RGB:rofs=0; gofs=1; bofs=2; aofs=-1; numchannels=3; break;
				case RGBALayout_BGR:rofs=2; gofs=1; bofs=0; aofs=-1; numchannels=3; break;
				case RGBALayout_BGRA:rofs=2; gofs=1; bofs=0; aofs=3; numchannels=4; break;
				case RGBALayout_ARGB:rofs=1; gofs=2; bofs=3; aofs=0; numchannels=4; break;
				case RGBALayout_ABGR:rofs=3; gofs=2; bofs=1; aofs=0; numchannels=4; break;
				default:break;
			}

			if (picbuffer.channeldatatype()==channeldatatype()) {
				const T* pSrc=(const T*)(picbuffer.data());
				const T* pSrcEnd=(const T*)(picbuffer.data()+picbuffer.stride()*picbuffer.height());
				T* pDest=(T*)data();

				for (;pSrc<pSrcEnd;pSrc+=numchannels,pDest+=4) {
					pDest[Color_redChannel]=pSrc[rofs];
					pDest[Color_greenChannel]=pSrc[gofs];
					pDest[Color_blueChannel]=pSrc[bofs];
					pDest[Color_alphaChannel]=(aofs!=-1) ? 0xff : pSrc[aofs];
				}
			}
			// TODO: Convert channel datatypes
		}
	}

	template <typename T>
	void Pixmap<T>::clone(const Pixmap<T>& src)
	{
		create(src.width(),src.height());
		Datablock::clone(src);
		//memcpy(m_pPixel,src.data(),sizeof(Color<T>)*m_Width*m_Height);
	}

	template <typename T>
	Pixmap<T>::~Pixmap()
	{
	//	if (m_pPixel) delete [] m_pPixel; m_pPixel=0;
	}

	template <typename T>
	void Pixmap<T>::create(const ovis_uint32 width,const ovis_uint32 height)
	{
		//if (m_pPixel) delete [] m_pPixel; m_pPixel=0;

		resize(width*height*sizeof(Color<T>));

		m_pPixel=/*new Color<T>[width*height];*/(Color<T>*)(data());
		m_Width=width;
		m_Height=height;
	}

	template <typename T>
	void Pixmap<T>::clear(const T r,const T g,const T b,const T a)
	{
		Color<T> col(r,g,b,a);
		Color<T>* pPixel=m_pPixel,*pPixelEnd=m_pPixel+m_Width*m_Height;
		for (;pPixel<pPixelEnd;++pPixel) pPixel->copy(col);
	}

	template <typename T>
	void Pixmap<T>::clear(const Color<T>& color)
	{
		Color<T>* pPixel=m_pPixel,*pPixelEnd=m_pPixel+m_Width*m_Height;
		for (;pPixel<pPixelEnd;++pPixel) pPixel->copy(color);
	}

	template <typename T>
	void Pixmap<T>::flip()
	{
		Color<T>* row=new Color<T>[m_Width];

		ovis_uint32 rowstride=stride();
		Color<T>* rows[2]={m_pPixel,m_pPixel+(m_Height-1)*m_Width};
		for (ovis_uint32 y=0;y<m_Height/2;++y) {
			memcpy(row,rows[0],rowstride);
			memcpy(rows[0],rows[1],rowstride);
			memcpy(rows[1],row,rowstride);
			rows[0]+=m_Width;
			rows[1]-=m_Width;
		}

		delete [] row;
	}

	template <typename T>
	void Pixmap<T>::pixel(const ovis_uint32 x,const ovis_uint32 y,const Color<T>& color)
	{
		m_pPixel[x+y*m_Width]=color;
	}

	template <typename T>
	Color<T>& Pixmap<T>::pixel(const ovis_uint32 x,const ovis_uint32 y) const
	{
		return m_pPixel[x+y*m_Width];
	}

	template <typename T>
	Color<T> Pixmap<T>::pixel(const ovis_uint32 x,const ovis_uint32 y)
	{
		return m_pPixel[x+y*m_Width];
	}

	template <typename T>
	void Pixmap<T>::horizontalFilter(const math::FilterBase &filter,const Pixmap<T> &src,Pixmap &dest)
	{
		if(dest.width()==src.height()) {
			dest.clone(src);
		} else {
			// allocate and calculate the contributions
			Weighttable weightsTable(filter,
				dest.width(), src.height(),
				double(dest.width())/double(src.width()));

			// step through rows            
			for(ovis_uint32 y=0;y<dest.height();++y) {
				// scale each row 
				const Color<T> *pSrc=src.pixels()+y*src.width();
				Color<T> *pDest=dest.pixels()+y*dest.width();

				for(ovis_uint32 x=0;x<dest.width();++x) {
					// loop through row
					double valueR=0,valueG=0,valueB=0,valueA=0;
					int left=weightsTable.leftBoundary(x);    // retrieve left boundary
					int right=weightsTable.rightBoundary(x);  // retrieve right boundary

					for(int i=left;i<=right;++i) {
						// scan between boundaries
						// accumulate weighted effect of each neighboring pixel
						double weight=weightsTable.weight(x,i-left);
						float ch;
						convert(ch,pSrc[i].m_Channels[Color_redChannel]); valueR+=(weight*ch); 
						convert(ch,pSrc[i].m_Channels[Color_greenChannel]); valueG+=(weight*ch); 
						convert(ch,pSrc[i].m_Channels[Color_blueChannel]); valueB+=(weight*ch); 
						convert(ch,pSrc[i].m_Channels[Color_alphaChannel]); valueA+=(weight*ch); 
					}

					// place result in destination pixel
					convert(pDest[x].m_Channels[Color_redChannel],(float)valueR);
					convert(pDest[x].m_Channels[Color_greenChannel],(float)valueG);
					convert(pDest[x].m_Channels[Color_blueChannel],(float)valueB);
					convert(pDest[x].m_Channels[Color_alphaChannel],(float)valueA);
				} 
			}
		}
	}

	template <typename T>
	void Pixmap<T>::verticalFilter(const math::FilterBase &filter,const Pixmap<T> &src,Pixmap &dest)
	{
		if(dest.height()==src.height()) {
			dest.clone(src);
		} else {
			// allocate and calculate the contributions
			Weighttable weightsTable(filter,
				dest.height(), src.height(),
				double(dest.height())/double(src.height()));

			// step through columns
			for(ovis_uint32 x=0;x<dest.width();++x) {
				// scale each column
				for(ovis_uint32 y=0;y<dest.height();++y) {
					double valueR=0,valueG=0,valueB=0,valueA=0;
					int left=weightsTable.leftBoundary(y);
					int right=weightsTable.rightBoundary(y);


					Color<T> *pDest=dest.pixels()+y*dest.width();

					for(int i=left;i<=right;++i) {
						const Color<T> *pSrc=src.pixels()+i*src.width();
						double weight=weightsTable.weight(y,i-left);
						float ch;
						convert(ch,pSrc[x].m_Channels[Color_redChannel]); valueR+=(weight*ch); 
						convert(ch,pSrc[x].m_Channels[Color_greenChannel]); valueG+=(weight*ch); 
						convert(ch,pSrc[x].m_Channels[Color_blueChannel]); valueB+=(weight*ch); 
						convert(ch,pSrc[x].m_Channels[Color_alphaChannel]); valueA+=(weight*ch); 
					}

					// place result in destination pixel
					convert(pDest[x].m_Channels[Color_redChannel],(float)valueR);
					convert(pDest[x].m_Channels[Color_greenChannel],(float)valueG);
					convert(pDest[x].m_Channels[Color_blueChannel],(float)valueB);
					convert(pDest[x].m_Channels[Color_alphaChannel],(float)valueA);
				}
			}			
		}
	}

	template <typename T>
	void Pixmap<T>::filteredRescale(const Pixmap& src,const math::FilterBase& resamplingFilter)
	{
		if(width()*src.height()<=height()*src.width()) {
			Pixmap tmp(width(),src.height());

			horizontalFilter(resamplingFilter,src,tmp);
			verticalFilter(resamplingFilter,tmp,*this);
		} else {
			Pixmap tmp(src.width(),height());

			verticalFilter(resamplingFilter,src,tmp);
			horizontalFilter(resamplingFilter,tmp,*this);
		}	
	}

	template <typename T>
	void Pixmap<T>::filteredRescale(const ovis_uint32 newwidth,const ovis_uint32 newheight,const math::FilterBase& resamplingFilter)
	{
		Pixmap src(*this);
		create(newwidth,newheight);
		filteredRescale(src,resamplingFilter);
	}

	template <typename T>
	void Pixmap<T>::generateNormalMap(const Pixmap<T> &bumpmap,const float intensity,const bool heightInAlpha)
	{
		float scale=((intensity>0.01f) ? intensity : 0.01f);

		if ((width()!=bumpmap.width()) ||
			(height()!=bumpmap.height())) create(bumpmap.width(),bumpmap.height());
		long w=((long)bumpmap.width()),h=((long)bumpmap.height());

		const Color<T> *pBump=bumpmap.pixels();
		Color<T> *pDest=pixels();

		for (long y=0;y<h;++y) {
			for (long x=0;x<w;++x,++pBump,++pDest) {
				math::Vector3f pxnorm(0,0,0),pdestnorm(0,0,0);
				math::Vector3f neighbours[2];


				/*float _height=(
					((int)pBump->m_Floats[0])+
					((int)pBump->m_Floats[1])+
					((int)pBump->m_Floats[2]))/3.0f;*/
				float f,_height=0;
				convert(f,pBump->m_Channels[Color_redChannel]); _height+=f;
				convert(f,pBump->m_Channels[Color_greenChannel]); _height+=f;
				convert(f,pBump->m_Channels[Color_blueChannel]); _height+=f;
				_height/=3.0f;

				pxnorm.x()=(float)(x);
				pxnorm.y()=(float)(y);
				pxnorm.z()=_height*scale;
				convert(pDest->m_Channels[Color_alphaChannel],heightInAlpha ? _height : 1.0f);

				/*
				*
				/|\
				/0|1\
				*--+--*
				\2|3/
				\|/
				*
				*/
				const int neighbourtab[][2][3]={
					{{0,-1,-1},{-1,0,-1}},
					{{0,-1,-1},{1,0,1}},
					{{0,1,1},{-1,0,-1}},
					{{0,1,1},{1,0,1}}
				};

				for (int i=0;i<4;++i) {
					for (int j=0;j<2;++j) {
						long xcoord=x+neighbourtab[i][j][0];
						long ycoord=y+neighbourtab[i][j][1];
						if (xcoord<0) xcoord=w+xcoord;
						if (ycoord<0) ycoord=h+ycoord;
						if (xcoord>=w) xcoord%=w;
						if (ycoord>=h) ycoord%=h;

						const Color<T> *pNeighbour=bumpmap.pixels()+w*ycoord+xcoord;
						_height=0;
						convert(f,pNeighbour->m_Channels[Color_redChannel]); _height+=f;
						convert(f,pNeighbour->m_Channels[Color_greenChannel]); _height+=f;
						convert(f,pNeighbour->m_Channels[Color_blueChannel]); _height+=f;
						_height/=3.0f;
						neighbours[j].x()=(float)(x+neighbourtab[i][j][0]);
						neighbours[j].y()=(float)(y+neighbourtab[i][j][1]);
						neighbours[j].z()=_height*scale;
					}
					math::Vector3f vecA=(neighbours[0]-pxnorm)*((float)(neighbourtab[i][0][2]));
					math::Vector3f vecB=(neighbours[1]-pxnorm)*((float)(neighbourtab[i][1][2]));
					math::Vector3f vec=vecB^vecA;
					pdestnorm+=vec;

				}
				pdestnorm.normalize();

				convert(pDest->m_Channels[Color_redChannel],pdestnorm.x()*0.5f+0.5f);
				convert(pDest->m_Channels[Color_greenChannel],pdestnorm.y()*0.5f+0.5f);
				convert(pDest->m_Channels[Color_blueChannel],pdestnorm.z()*0.5f+0.5f);
			}
		}
	}


}
}

#endif
