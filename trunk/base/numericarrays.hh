#ifndef OVIS_BASE_NUMERIC_ARRAYS_HH_INCLUDED
#define OVIS_BASE_NUMERIC_ARRAYS_HH_INCLUDED

#include "dll.hh"

namespace ovis {
namespace base {

class Float3;
class Float4;
class Float4x3;
class Float3x4;
class Float4x4;

class OVIS_API Float2
{
public:

	Float2() {}
	Float2(const float f1);
	Float2(const float f1,const float f2);
	Float2(const float *f);
	Float2(const Float2& src);
	Float2(const Float3& src);
	Float2(const Float4& src);
	
	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
protected:
	float m_Floats[2];
};

class OVIS_API Float3
{
public:
	Float3() {}
	Float3(const float f1);
	Float3(const float f1,const float f2);
	Float3(const float f1,const float f2,const float f3);
	Float3(const float *f);
	Float3(const Float2& float2);
	Float3(const Float3& src);
	Float3(const Float4& src);

	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
protected:
	float m_Floats[3];
};

class OVIS_API Float4
{
public:
	Float4() {}
	Float4(const float f1);
	Float4(const float f1,const float f2);
	Float4(const float f1,const float f2,const float f3);
	Float4(const float f1,const float f2,const float f3,const float f4);
	Float4(const float *f);
	Float4(const Float2& float2);
	Float4(const Float3& float3);
	Float4(const Float4& src);

	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
protected:
	float m_Floats[4];
};

class OVIS_API Float3x3
{
public:
	Float3x3() {}
	Float3x3(const float* f);
	Float3x3( float n11 , float n12 , float n13,
		float n21 , float n22 , float n23,
		float n31 , float n32 , float n33 )
		: m_11( n11 ) , m_12( n12 ) , m_13( n13 ) ,
		m_21( n21 ) , m_22( n22 ) , m_23( n23 ) ,
		m_31( n31 ) , m_32( n32 ) , m_33( n33 ) 
	{}
	Float3x3(const Float3x3& src);
	Float3x3(const Float3x4& src);
	Float3x3(const Float4x3& src);
	Float3x3(const Float4x4& src);
	
	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
	inline float value(const int column,const int row) const { return m_Floats[column+row*3]; }
	inline float& value(const int column,const int row) { return m_Floats[column+row*3]; }

protected:
	union
	{
		struct
		{
			float m_Floats[3*3];
		};
		struct
		{
			float m_11 , m_12 , m_13;
			float m_21 , m_22 , m_23;
			float m_31 , m_32 , m_33;
		};
	};
};

class OVIS_API Float3x4
{
public:
	Float3x4() {}
	Float3x4(const float* f);
	Float3x4( float n11 , float n12 , float n13,
		float n21 , float n22 , float n23,
		float n31 , float n32 , float n33,
		float n41 , float n42 , float n43 )
		: m_11( n11 ) , m_12( n12 ) , m_13( n13 ) ,
		m_21( n21 ) , m_22( n22 ) , m_23( n23 ) ,
		m_31( n31 ) , m_32( n32 ) , m_33( n33 ) ,
		m_41( n41 ) , m_42( n42 ) , m_43( n43 )
	{}
	Float3x4(const Float3x3& src);
	Float3x4(const Float3x4& src);
	Float3x4(const Float4x3& src);
	Float3x4(const Float4x4& src);
	
	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
	inline float value(const int column,const int row) const { return m_Floats[column+row*3]; }
	inline float& value(const int column,const int row) { return m_Floats[column+row*3]; }

protected:
	union
	{
		struct
		{
			float m_Floats[3*4];
		};
		struct
		{
			float m_11 , m_12 , m_13;
			float m_21 , m_22 , m_23;
			float m_31 , m_32 , m_33;
			float m_41 , m_42 , m_43;
		};
	};
};

class OVIS_API Float4x3
{
public:
	Float4x3() {}
	Float4x3(const float* f);
	Float4x3( float n11 , float n12 , float n13 , float n14,
		float n21 , float n22 , float n23 , float n24,
		float n31 , float n32 , float n33 , float n34)
		: m_11( n11 ) , m_12( n12 ) , m_13( n13 ) , m_14( n14 ) ,
		m_21( n21 ) , m_22( n22 ) , m_23( n23 ) , m_24( n24 ) ,
		m_31( n31 ) , m_32( n32 ) , m_33( n33 ) , m_34( n34 )
	{}
	Float4x3(const Float3x3& src);
	Float4x3(const Float3x4& src);
	Float4x3(const Float4x3& src);
	Float4x3(const Float4x4& src);
	
	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
	inline float value(const int column,const int row) const { return m_Floats[column+row*4]; }
	inline float& value(const int column,const int row) { return m_Floats[column+row*4]; }

protected:
	union
	{
		struct
		{
			float m_Floats[4*3];
		};
		struct
		{
			float m_11 , m_12 , m_13 , m_14;
			float m_21 , m_22 , m_23 , m_24;
			float m_31 , m_32 , m_33 , m_34;
		};
	};
};

class OVIS_API Float4x4
{
public:
	Float4x4() {}
	Float4x4(const float* f);
	Float4x4( float n11 , float n12 , float n13 , float n14,
		float n21 , float n22 , float n23 , float n24,
		float n31 , float n32 , float n33 , float n34,
		float n41 , float n42 , float n43 , float n44 )
		: m_11( n11 ) , m_12( n12 ) , m_13( n13 ) , m_14( n14 ),
		m_21( n21 ) , m_22( n22 ) , m_23( n23 ) , m_24( n24 ),
		m_31( n31 ) , m_32( n32 ) , m_33( n33 ) , m_34( n34 ),
		m_41( n41 ) , m_42( n42 ) , m_43( n43 ) , m_44( n44 )
	{}
	Float4x4(const Float3x3& src);
	Float4x4(const Float3x4& src);
	Float4x4(const Float4x3& src);
	Float4x4(const Float4x4& src);

	Float4x4& operator =(const Float4x4& src);
	
	inline operator const float*() const { return m_Floats; }
	inline operator float*() { return m_Floats; }
	inline float operator[](int i) const { return m_Floats[i]; }
	inline float& operator[](int i) { return m_Floats[i]; }
	inline float value(const int column,const int row) const { return m_Floats[column+row*4]; }
	inline float& value(const int column,const int row) { return m_Floats[column+row*4]; }

protected:
	union
	{
		struct
		{
			float m_Floats[4*4];
		};
		struct
		{
			float m_11 , m_12 , m_13 , m_14;
			float m_21 , m_22 , m_23 , m_24;
			float m_31 , m_32 , m_33 , m_34;
			float m_41 , m_42 , m_43 , m_44;
		};
	};
};

}
}

#endif
