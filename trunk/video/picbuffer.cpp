#include "picbuffer.hh"

namespace ovis {
namespace video {

	void convert(const ovis_int8 src,ovis_uint8& dest) { dest=(ovis_uint8)(((int)src)+128); }
	void convert(const ovis_int8 src,ovis_int8& dest) { dest=src; }
	void convert(const ovis_int8 src,ovis_int16& dest) { dest=((ovis_int16)src)<<8; }
	void convert(const ovis_int8 src,ovis_uint16& dest) { dest=(((ovis_int16)src)+128)<<8; }
	void convert(const ovis_int8 src,float& dest) { ovis_int16 i=src; i+=128; dest=((float)i)/255.0f; }

	void convert(const ovis_uint8 src,ovis_int8& dest) { dest=(ovis_int8)(((int)src)-128); }
	void convert(const ovis_uint8 src,ovis_uint8& dest) { dest=src; }
	void convert(const ovis_uint8 src,ovis_int16& dest) { dest=(((ovis_int32)src)<<8)-32768; }
	void convert(const ovis_uint8 src,ovis_uint16& dest) { dest=((ovis_uint16)src)<<8; }
	void convert(const ovis_uint8 src,float& dest) { dest=((float)src)/255.0f; }

	void convert(const ovis_int16 src,ovis_uint8& dest) { ovis_int32 i=src; dest=(ovis_int8)((i+32768)>>8); }
	void convert(const ovis_int16 src,ovis_int8& dest) { dest=(ovis_int8)(src>>8); }
	void convert(const ovis_int16 src,ovis_int16& dest) { dest=src; }
	void convert(const ovis_int16 src,ovis_uint16& dest) { dest=(ovis_uint16) (((ovis_int32)src)+32768); }
	void convert(const ovis_int16 src,float& dest) { ovis_int32 i=src; i+=32768; dest=((float)i)/65535.0f; }

	void convert(const ovis_uint16 src,ovis_uint8& dest) { dest=(ovis_uint8)(src>>8); }
	void convert(const ovis_uint16 src,ovis_int8& dest) { dest=(src>>8)-128; }
	void convert(const ovis_uint16 src,ovis_int16& dest) { dest=(ovis_int16)(((ovis_int32)src)-32768); }
	void convert(const ovis_uint16 src,ovis_uint16& dest) { dest=src; }
	void convert(const ovis_uint16 src,float& dest) { dest=((float)src)/65535.0f; }

	void convert(const float src,ovis_uint8& dest) {
		float f=src*255.0f;
		f=(f<0) ? 0 : ((f>255.0f) ? 255.0f : f);
		dest=(ovis_uint8)f;
	}

	void convert(const float src,ovis_int8& dest) {
		float f=(src)*255.0f-128.0f;
		f=(f<-128.0f) ? -128.0f : ((f>127.0f) ? 127.0f : f);
		dest=(ovis_int8)f;
	}

	void convert(const float src,ovis_uint16& dest) {
		float f=src*65535.0f;
		f=(f<0) ? 0 : ((f>65535.0f) ? 65535.0f : f);
		dest=(ovis_uint16)f;
	}

	void convert(const float src,ovis_int16& dest) {
		float f=src*65535.0f-32768.0f;
		f=(f<-32768.0f) ? -32768.0f : ((f>32767.0f) ? 32767.0f : f);
		dest=(ovis_int16)f;
	}

	void convert(const float src,float& dest) { dest=src; }


	template <typename srctype,typename desttype>
	void lerp2dint(const srctype srcpixels[4][4],desttype *destpixels,const float xf,const float yf)
	{
		ovis_int32 src1[4]={srcpixels[0][0],srcpixels[0][1],srcpixels[0][2],srcpixels[0][3]};
		ovis_int32 src2[4]={srcpixels[1][0],srcpixels[1][1],srcpixels[1][2],srcpixels[1][3]};
		ovis_int32 src3[4]={srcpixels[2][0],srcpixels[2][1],srcpixels[2][2],srcpixels[2][3]};
		ovis_int32 src4[4]={srcpixels[3][0],srcpixels[3][1],srcpixels[3][2],srcpixels[3][3]};

		ovis_int32 temp[2][4];

		temp[0][0]=src1[0]+(ovis_int32)((float)(src2[0]-src1[0])*xf);
		temp[0][1]=src1[1]+(ovis_int32)((float)(src2[1]-src1[1])*xf);
		temp[0][2]=src1[2]+(ovis_int32)((float)(src2[2]-src1[2])*xf);
		temp[0][3]=src1[3]+(ovis_int32)((float)(src2[3]-src1[3])*xf);

		temp[1][0]=src3[0]+(ovis_int32)((float)(src4[0]-src3[0])*xf);
		temp[1][1]=src3[1]+(ovis_int32)((float)(src4[1]-src3[1])*xf);
		temp[1][2]=src3[2]+(ovis_int32)((float)(src4[2]-src3[2])*xf);
		temp[1][3]=src3[3]+(ovis_int32)((float)(src4[3]-src3[3])*xf);

		ovis_int32 t;

		t=temp[0][0]+(ovis_int32)((float)(temp[1][0]-temp[0][0])*yf);
		convert((srctype)t , destpixels[0]);
		t=temp[0][1]+(ovis_int32)((float)(temp[1][1]-temp[0][1])*yf);
		convert((srctype)t , destpixels[1]);
		t=temp[0][2]+(ovis_int32)((float)(temp[1][2]-temp[0][2])*yf);
		convert((srctype)t , destpixels[2]);
		t=temp[0][3]+(ovis_int32)((float)(temp[1][3]-temp[0][3])*yf);
		convert((srctype)t , destpixels[3]);

//		(*destpixels)[0]=temp[0][0]+(desttype)((float)(temp[1][0]-temp[0][0])*yf);
//		(*destpixels)[1]=temp[0][1]+(desttype)((float)(temp[1][1]-temp[0][1])*yf);
//		(*destpixels)[2]=temp[0][2]+(desttype)((float)(temp[1][2]-temp[0][2])*yf);
//		(*destpixels)[3]=temp[0][3]+(desttype)((float)(temp[1][3]-temp[0][3])*yf);
	}

	template <typename srctype,typename desttype>
	void lerp2dfloat(const srctype srcpixels[4][4],desttype *destpixels,const float xf,const float yf)
	{
		float src1[4]={srcpixels[0][0],srcpixels[0][1],srcpixels[0][2],srcpixels[0][3]};
		float src2[4]={srcpixels[1][0],srcpixels[1][1],srcpixels[1][2],srcpixels[1][3]};
		float src3[4]={srcpixels[2][0],srcpixels[2][1],srcpixels[2][2],srcpixels[2][3]};
		float src4[4]={srcpixels[3][0],srcpixels[3][1],srcpixels[3][2],srcpixels[3][3]};

		float temp[2][4];

		temp[0][0]=(float)(src1[0])+((float)(src2[0]-src1[0])*xf);
		temp[0][1]=(float)(src1[1])+((float)(src2[1]-src1[1])*xf);
		temp[0][2]=(float)(src1[2])+((float)(src2[2]-src1[2])*xf);
		temp[0][3]=(float)(src1[3])+((float)(src2[3]-src1[3])*xf);

		temp[1][0]=(float)(src3[0])+((float)(src4[0]-src3[0])*xf);
		temp[1][1]=(float)(src3[1])+((float)(src4[1]-src3[1])*xf);
		temp[1][2]=(float)(src3[2])+((float)(src4[2]-src3[2])*xf);
		temp[1][3]=(float)(src3[3])+((float)(src4[3]-src3[3])*xf);

		convert((srctype)(temp[0][0]+((float)(temp[1][0]-temp[0][0])*yf)) , destpixels[0]);
		convert((srctype)(temp[0][1]+((float)(temp[1][1]-temp[0][1])*yf)) , destpixels[1]);
		convert((srctype)(temp[0][2]+((float)(temp[1][2]-temp[0][2])*yf)) , destpixels[2]);
		convert((srctype)(temp[0][3]+((float)(temp[1][3]-temp[0][3])*yf)) , destpixels[3]);

		//		(*destpixels)[0]=temp[0][0]+(desttype)((float)(temp[1][0]-temp[0][0])*yf);
		//		(*destpixels)[1]=temp[0][1]+(desttype)((float)(temp[1][1]-temp[0][1])*yf);
		//		(*destpixels)[2]=temp[0][2]+(desttype)((float)(temp[1][2]-temp[0][2])*yf);
		//		(*destpixels)[3]=temp[0][3]+(desttype)((float)(temp[1][3]-temp[0][3])*yf);
	}

	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }

	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }

	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }

	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf) { lerp2dint(srcpixels,destpixels,xf,yf); }

	void lerp2d(const float srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }
	void lerp2d(const float srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }
	void lerp2d(const float srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }
	void lerp2d(const float srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }

	void lerp2d(const ovis_uint8 srcpixels[4][4],float *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int8 srcpixels[4][4],float *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_uint16 srcpixels[4][4],float *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }
	void lerp2d(const ovis_int16 srcpixels[4][4],float *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }

	void lerp2d(const float srcpixels[4][4],float *destpixels,const float xf,const float yf) { lerp2dfloat(srcpixels,destpixels,xf,yf); }

/*	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint8 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf);

	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int8 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf);

	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint16 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf);

	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int16 srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf);

	void lerp2d(const float srcpixels[4][4],ovis_uint8 *destpixels,const float xf,const float yf);
	void lerp2d(const float srcpixels[4][4],ovis_int8 *destpixels,const float xf,const float yf);
	void lerp2d(const float srcpixels[4][4],ovis_uint16 *destpixels,const float xf,const float yf);
	void lerp2d(const float srcpixels[4][4],ovis_int16 *destpixels,const float xf,const float yf);

	void lerp2d(const ovis_uint8 srcpixels[4][4],float *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int8 srcpixels[4][4],float *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_uint16 srcpixels[4][4],float *destpixels,const float xf,const float yf);
	void lerp2d(const ovis_int16 srcpixels[4][4],float *destpixels,const float xf,const float yf);*/

	template <typename srctype,typename desttype> bool internalTransfer(const Picbuffer& src,Picbuffer& dest)
	{
		bool sameChtype=(src.channeldatatype()==dest.channeldatatype());
		bool sameWidth=(src.width()==dest.width());
		bool sameHeight=(src.height()==dest.height());
		bool sameRGBALayout=(src.rgbalayout()==dest.rgbalayout());

		if (sameChtype && sameWidth && sameHeight && sameRGBALayout) { dest.clone(src); return true; }
		else {

			int srcrofs=0,srcgofs=1,srcbofs=2,srcaofs=3,srcnumchannels=0;
			int dstrofs=0,dstgofs=1,dstbofs=2,dstaofs=3,dstnumchannels=0;
			switch (src.rgbalayout()) {
				case RGBALayout_RGB:srcrofs=0; srcgofs=1; srcbofs=2; srcaofs=-1; srcnumchannels=3; break;
				case RGBALayout_BGR:srcrofs=2; srcgofs=1; srcbofs=0; srcaofs=-1; srcnumchannels=3; break;
				case RGBALayout_RGBA:srcrofs=0; srcgofs=1; srcbofs=2; srcaofs=3; srcnumchannels=4; break;
				case RGBALayout_BGRA:srcrofs=2; srcgofs=1; srcbofs=0; srcaofs=3; srcnumchannels=4; break;
				case RGBALayout_ARGB:srcrofs=1; srcgofs=2; srcbofs=3; srcaofs=0; srcnumchannels=4; break;
				case RGBALayout_ABGR:srcrofs=3; srcgofs=2; srcbofs=1; srcaofs=0; srcnumchannels=4; break;
				default:break;
			}
			switch (dest.rgbalayout()) {
				case RGBALayout_RGB:dstrofs=0; dstgofs=1; dstbofs=2; dstaofs=-1; dstnumchannels=3; break;
				case RGBALayout_BGR:dstrofs=2; dstgofs=1; dstbofs=0; dstaofs=-1; dstnumchannels=3; break;
				case RGBALayout_RGBA:dstrofs=0; dstgofs=1; dstbofs=2; dstaofs=3; dstnumchannels=4; break;
				case RGBALayout_BGRA:dstrofs=2; dstgofs=1; dstbofs=0; dstaofs=3; dstnumchannels=4; break;
				case RGBALayout_ARGB:dstrofs=1; dstgofs=2; dstbofs=3; dstaofs=0; dstnumchannels=4; break;
				case RGBALayout_ABGR:dstrofs=3; dstgofs=2; dstbofs=1; dstaofs=0; dstnumchannels=4; break;
				default:break;
			}

			ovis_uint8 *destrows[4]={
				(dest.data())+dstrofs*sizeof(desttype),
				(dest.data())+dstgofs*sizeof(desttype),
				(dest.data())+dstbofs*sizeof(desttype),
				(dstaofs<0) ? 0 : ((dest.data())+dstaofs*sizeof(desttype))
			};

			unsigned int csrc=(srcaofs<0) ? 3 : 4;
			unsigned int cdest=(dstaofs<0) ? 3 : 4;

			for (unsigned long dy=0;dy<dest.height();++dy) {

				unsigned long sy=dy*(src.height()-1)/(dest.height()-1);

				unsigned long sy2=(sy+1);
				if (sy2>=src.height()) sy2=0;

				const ovis_uint8 *srcrows[4]={
					(src.data())+srcrofs*sizeof(srctype)+sy*src.stride(),
					(src.data())+srcgofs*sizeof(srctype)+sy*src.stride(),
					(src.data())+srcbofs*sizeof(srctype)+sy*src.stride(),
					(srcaofs<0) ? 0 : ((src.data())+srcaofs*sizeof(srctype)+sy*src.stride())
				};
				const ovis_uint8 *srcrows2[4]={
					(src.data())+srcrofs*sizeof(srctype)+sy2*src.stride(),
					(src.data())+srcgofs*sizeof(srctype)+sy2*src.stride(),
					(src.data())+srcbofs*sizeof(srctype)+sy2*src.stride(),
					(srcaofs<0) ? 0 : ((src.data())+srcaofs*sizeof(srctype)+sy2*src.stride())
				};

				/*if (dy==(dest.height()-1)) {
					srcrows2[0]=(src.data())+srcrofs*sizeof(srctype);
					srcrows2[1]=(src.data())+srcgofs*sizeof(srctype);
					srcrows2[2]=(src.data())+srcgofs*sizeof(srctype);
					srcrows2[3]=(srcaofs<0) ? 0 : ((src.data())+srcaofs*sizeof(srctype));
				}*/

				float yf=dy*(float)(src.height()-1)/(float)(dest.height()-1);
				yf-=(float)((int)yf);

				for (unsigned long dx=0;dx<dest.width();++dx) {

					unsigned long sx=dx*(src.width()-1)/(dest.width()-1);

					unsigned long sx2=(sx+1);
					if (sx2>=src.width()) sx2=0;

					float xf=dx*(float)(src.width()-1)/(float)(dest.width()-1);
					xf-=(float)((int)xf);

					// First array param: pixel nr.
					// 0 - upper left  1 - upper right  2 - lower left  3 - lower right
					// Second param: RGBA param  0 - R  1 - G  2 - B  3 - A
					srctype srcpixels[4][4];

					srcpixels[0][0]=((srctype*)srcrows[0])[sx*csrc];
					srcpixels[0][1]=((srctype*)srcrows[1])[sx*csrc];
					srcpixels[0][2]=((srctype*)srcrows[2])[sx*csrc];

					srcpixels[1][0]=((srctype*)srcrows[0])[sx2*csrc];
					srcpixels[1][1]=((srctype*)srcrows[1])[sx2*csrc];
					srcpixels[1][2]=((srctype*)srcrows[2])[sx2*csrc];

					srcpixels[2][0]=((srctype*)srcrows2[0])[sx*csrc];
					srcpixels[2][1]=((srctype*)srcrows2[1])[sx*csrc];
					srcpixels[2][2]=((srctype*)srcrows2[2])[sx*csrc];

					srcpixels[3][0]=((srctype*)srcrows2[0])[sx2*csrc];
					srcpixels[3][1]=((srctype*)srcrows2[1])[sx2*csrc];
					srcpixels[3][2]=((srctype*)srcrows2[2])[sx2*csrc];

					if (srcaofs>=0) {
						srcpixels[0][3]=((srctype*)srcrows[3])[sx*csrc];
						srcpixels[1][3]=((srctype*)srcrows[3])[sx2*csrc];
						srcpixels[2][3]=((srctype*)srcrows2[3])[sx*csrc];
						srcpixels[3][3]=((srctype*)srcrows2[3])[sx2*csrc];
					}

					desttype dest[4];

					lerp2d(srcpixels,dest,xf,yf);

					((desttype*)destrows[0])[dx*cdest]=dest[0];
					((desttype*)destrows[1])[dx*cdest]=dest[1];
					((desttype*)destrows[2])[dx*cdest]=dest[2];
					if (dstaofs>=0) ((desttype*)destrows[3])[dx*cdest]=dest[3];
				}

				/*srcrows[0]+=src.stride();
				srcrows[1]+=src.stride();
				srcrows[2]+=src.stride();
				srcrows[3]+=src.stride();

				srcrows2[0]+=src.stride();
				srcrows2[1]+=src.stride();
				srcrows2[2]+=src.stride();
				srcrows2[3]+=src.stride();*/

				destrows[0]+=dest.stride();
				destrows[1]+=dest.stride();
				destrows[2]+=dest.stride();
				if (dstaofs>=0) destrows[3]+=dest.stride();
			}

		}

		return true;
	}



	bool transfer(const Picbuffer& src,Picbuffer& dest)
	{
		switch (src.channeldatatype()) {
			case video::Channeldatatype_Uint8:
				switch (dest.channeldatatype()) {
					case video::Channeldatatype_Uint8:return internalTransfer<ovis_uint8,ovis_uint8>(src,dest);
					case video::Channeldatatype_Int8:return internalTransfer<ovis_uint8,ovis_int8>(src,dest);
					case video::Channeldatatype_Uint16:return internalTransfer<ovis_uint8,ovis_uint16>(src,dest);
					case video::Channeldatatype_Int16:return internalTransfer<ovis_uint8,ovis_int16>(src,dest);
					case video::Channeldatatype_Float32:return internalTransfer<ovis_uint8,float>(src,dest);
					case video::Channeldatatype_Float16:return false; // TODO: solve this
				}
				break;
			case video::Channeldatatype_Int8:
				switch (dest.channeldatatype()) {
					case video::Channeldatatype_Uint8:return internalTransfer<ovis_int8,ovis_uint8>(src,dest);
					case video::Channeldatatype_Int8:return internalTransfer<ovis_int8,ovis_int8>(src,dest);
					case video::Channeldatatype_Uint16:return internalTransfer<ovis_int8,ovis_uint16>(src,dest);
					case video::Channeldatatype_Int16:return internalTransfer<ovis_int8,ovis_int16>(src,dest);
					case video::Channeldatatype_Float32:return internalTransfer<ovis_int8,float>(src,dest);
					case video::Channeldatatype_Float16:return false; // TODO: solve this
				}
				break;
			case video::Channeldatatype_Uint16:
				switch (dest.channeldatatype()) {
					case video::Channeldatatype_Uint8:return internalTransfer<ovis_uint16,ovis_uint8>(src,dest);
					case video::Channeldatatype_Int8:return internalTransfer<ovis_uint16,ovis_int8>(src,dest);
					case video::Channeldatatype_Uint16:return internalTransfer<ovis_uint16,ovis_uint16>(src,dest);
					case video::Channeldatatype_Int16:return internalTransfer<ovis_uint16,ovis_int16>(src,dest);
					case video::Channeldatatype_Float32:return internalTransfer<ovis_uint16,float>(src,dest);
					case video::Channeldatatype_Float16:return false; // TODO: solve this
				}
				break;
			case video::Channeldatatype_Int16:
				switch (dest.channeldatatype()) {
					case video::Channeldatatype_Uint8:return internalTransfer<ovis_int16,ovis_uint8>(src,dest);
					case video::Channeldatatype_Int8:return internalTransfer<ovis_int16,ovis_int8>(src,dest);
					case video::Channeldatatype_Uint16:return internalTransfer<ovis_int16,ovis_uint16>(src,dest);
					case video::Channeldatatype_Int16:return internalTransfer<ovis_int16,ovis_int16>(src,dest);
					case video::Channeldatatype_Float32:return internalTransfer<ovis_int16,float>(src,dest);
					case video::Channeldatatype_Float16:return false; // TODO: solve this
				}
				break;
			case video::Channeldatatype_Float32:
				switch (dest.channeldatatype()) {
					case video::Channeldatatype_Uint8:return internalTransfer<float,ovis_uint8>(src,dest);
					case video::Channeldatatype_Int8:return internalTransfer<float,ovis_int8>(src,dest);
					case video::Channeldatatype_Uint16:return internalTransfer<float,ovis_uint16>(src,dest);
					case video::Channeldatatype_Int16:return internalTransfer<float,ovis_int16>(src,dest);
					case video::Channeldatatype_Float32:return internalTransfer<float,float>(src,dest);
					case video::Channeldatatype_Float16:return false; // TODO: solve this
				}
				break;
			default:return false;
		}

		return false;
	}

	bool calculateNormalmap(const Picbuffer& src,Picbuffer& dest)
	{
		return false;
	}

}
}
