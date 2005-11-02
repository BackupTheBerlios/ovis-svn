#include "numericarrays.hh"

namespace ovis {
namespace base {

	Float2::Float2(const Float2& src) { m_Floats[0]=src[0]; m_Floats[1]=src[1];}
	Float2::Float2(const Float3& src) { m_Floats[0]=src[0]; m_Floats[1]=src[1];}
	Float2::Float2(const Float4& src) { m_Floats[0]=src[0]; m_Floats[1]=src[1];}
	Float2::Float2(const float f1) { m_Floats[0]=f1; }
	Float2::Float2(const float f1,const float f2) { m_Floats[0]=f1; m_Floats[1]=f2; }
	Float2::Float2(const float *f) { for (int i=0;i<2;++i) m_Floats[i]=f[i]; }



	Float3::Float3(const Float2& float2)
	{
		m_Floats[0]=float2[0];
		m_Floats[1]=float2[1];
		m_Floats[2]=0;
	}

	Float3::Float3(const Float3& src)
	{
		m_Floats[0]=src[0];
		m_Floats[1]=src[1];
		m_Floats[2]=src[2];
	}

	Float3::Float3(const Float4& src)
	{
		m_Floats[0]=src[0];
		m_Floats[1]=src[1];
		m_Floats[2]=src[2];
	}

	Float3::Float3(const float f1)
	{
		m_Floats[0]=f1;
	}

	Float3::Float3(const float f1,const float f2)
	{
		m_Floats[0]=f1;
		m_Floats[1]=f2;
	}

	Float3::Float3(const float f1,const float f2,const float f3)
	{
		m_Floats[0]=f1;
		m_Floats[1]=f2;
		m_Floats[2]=f3;
	}

	Float3::Float3(const float *f) { for (int i=0;i<3;++i) m_Floats[i]=f[i]; }







	Float4::Float4(const Float2& float2)
	{
		m_Floats[0]=float2[0];
		m_Floats[1]=float2[1];
		m_Floats[2]=0;
		m_Floats[3]=0;
	}

	Float4::Float4(const Float3& float3)
	{
		m_Floats[0]=float3[0];
		m_Floats[1]=float3[1];
		m_Floats[2]=float3[2];
		m_Floats[3]=0;
	}

	Float4::Float4(const Float4& src)
	{
		m_Floats[0]=src[0];
		m_Floats[1]=src[1];
		m_Floats[2]=src[2];
		m_Floats[3]=src[3];
	}

	Float4::Float4(const float f1)
	{
		m_Floats[0]=f1;
	}

	Float4::Float4(const float f1,const float f2)
	{
		m_Floats[0]=f1;
		m_Floats[1]=f2;
	}

	Float4::Float4(const float f1,const float f2,const float f3)
	{
		m_Floats[0]=f1;
		m_Floats[1]=f2;
		m_Floats[2]=f3;
	}

	Float4::Float4(const float f1,const float f2,const float f3,const float f4)
	{
		m_Floats[0]=f1;
		m_Floats[1]=f2;
		m_Floats[2]=f3;
		m_Floats[3]=f4;
	}

	Float4::Float4(const float *f) { for (int i=0;i<4;++i) m_Floats[i]=f[i]; }







	Float3x3::Float3x3(const float *f) { for (int i=0;i<3*3;++i) m_Floats[i]=f[i]; }

	Float3x3::Float3x3(const Float3x3& src) { for (int i=0;i<3*3;++i) m_Floats[i]=src[i]; }

	Float3x3::Float3x3(const Float3x4& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);
		}
	}

	Float3x3::Float3x3(const Float4x3& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);
		}
	}

	Float3x3::Float3x3(const Float4x4& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);
		}
	}


	Float3x4::Float3x4(const float *f) { for (int i=0;i<3*4;++i) m_Floats[i]=f[i]; }

	Float3x4::Float3x4(const Float3x3& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);
			value(j,3)=0;
		}

	}

	Float3x4::Float3x4(const Float3x4& src)
	{
		for (int i=0;i<3*4;++i) m_Floats[i]=src[i];
	}

	Float3x4::Float3x4(const Float4x3& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);
			
			value(j,3)=0;
		}
	}

	Float3x4::Float3x4(const Float4x4& src)
	{
		for (int j=0;j<4;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);
		}
	}




	Float4x3::Float4x3(const float *f) { for (int i=0;i<4*3;++i) m_Floats[i]=f[i]; }

	Float4x3::Float4x3(const Float3x3& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);

			value(3,j)=0;
		}
	}

	Float4x3::Float4x3(const Float3x4& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<3;++i)
				value(i,j)=src.value(i,j);

			value(3,j)=0;
		}
	}

	Float4x3::Float4x3(const Float4x3& src)
	{
		for (int i=0;i<4*3;++i) m_Floats[i]=src[i];
	}

	Float4x3::Float4x3(const Float4x4& src)
	{
		for (int j=0;j<3;++j) {
			for (int i=0;i<4;++i)
				value(i,j)=src.value(i,j);
		}
	}





	Float4x4::Float4x4(const float *f) { for (int i=0;i<4*4;++i) m_Floats[i]=f[i]; }

	Float4x4::Float4x4(const Float3x3& src)
	{
		int i,j;
		for (j=0;j<3;++j) {
			for (i=0;i<3;++i) {
				m_Floats[i+j*4]=src[i+j*3];
			}
			m_Floats[3+j*4]=0;
		}
		for (i=0;i<4;++i) m_Floats[i+4*3]=0;
	}

	Float4x4::Float4x4(const Float3x4& src)
	{
		int i,j;
		for (j=0;j<4;++j) {
			for (i=0;i<3;++i) {
				m_Floats[i+j*4]=src[i+j*3];
			}
			m_Floats[3+j*4]=0;
		}
	}

	Float4x4::Float4x4(const Float4x3& src)
	{
		int i;
		for (i=0;i<4*3;++i) m_Floats[i]=src[i];
		for (i=0;i<4;++i) m_Floats[i+4*3]=0;
	}

	Float4x4::Float4x4(const Float4x4& src)
	{
		for (int i=0;i<4*4;++i) m_Floats[i]=src[i];
	}

	Float4x4& Float4x4::operator =(const Float4x4& src)
	{
		for (int i=0;i<4*4;++i) m_Floats[i]=src[i];
		return *this;
	}

}
}
