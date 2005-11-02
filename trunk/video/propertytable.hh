#ifndef OVIS_VIDEO_PROPERTYTABLE_HH_INCLUDED
#define OVIS_VIDEO_PROPERTYTABLE_HH_INCLUDED

#include "../base/basememobj.hh"
#include "../base/itypes.hh"
#include "../base/numericarrays.hh"
#include "../base/string.hh"
#include "cubemap.hh"
#include "texture2d.hh"

#include <map>
#include <string>

namespace ovis {
namespace video {

	class Propertytable:public base::BaseMemObj
	{
	public:
		OVIS_API Propertytable();
		OVIS_API Propertytable(const Propertytable& src) { clone(src); }
		OVIS_API virtual ~Propertytable();

		OVIS_API inline Propertytable& operator = (const Propertytable& src) { clone(src); return *this; }
		
		OVIS_API virtual void clone(const Propertytable& src);
		OVIS_API void inject(const Propertytable& src);

		OVIS_API void addFloat(const base::String& name,const float f);
		OVIS_API void addFloat2(const base::String& name,const base::Float2& float2);
		OVIS_API void addFloat3(const base::String& name,const base::Float3& float3);
		OVIS_API void addFloat4(const base::String& name,const base::Float4& float4);
		OVIS_API void addFloat3x3(const base::String& name,const base::Float3x3& float3x3);
		OVIS_API void addFloat4x3(const base::String& name,const base::Float4x3& float4x3);
		OVIS_API void addFloat3x4(const base::String& name,const base::Float3x4& float3x4);
		OVIS_API void addFloat4x4(const base::String& name,const base::Float4x4& float4x4);
		OVIS_API void addInt(const base::String& name,const ovis_int32 i);
		OVIS_API void addBool(const base::String& name,const bool b);
		OVIS_API void add2DTexture(const base::String& name,Texture2D* tex);
		OVIS_API void addCubemap(const base::String& name,Cubemap* cubemap);

		OVIS_API bool fetchFloat(const base::String& name,float& f);
		OVIS_API bool fetchFloat2(const base::String& name,base::Float2& float2);
		OVIS_API bool fetchFloat3(const base::String& name,base::Float3& float3);
		OVIS_API bool fetchFloat4(const base::String& name,base::Float4& float4);
		OVIS_API bool fetchFloat3x3(const base::String& name,base::Float3x3& float3x3);
		OVIS_API bool fetchFloat4x3(const base::String& name,base::Float4x3& float4x3);
		OVIS_API bool fetchFloat3x4(const base::String& name,base::Float3x4& float3x4);
		OVIS_API bool fetchFloat4x4(const base::String& name,base::Float4x4& float4x4);
		OVIS_API bool fetchInt(const base::String& name,ovis_int32& i);
		OVIS_API bool fetchBool(const base::String& name,bool& b);
		OVIS_API bool fetch2DTexture(const base::String& name,Texture2D*& tex);
		OVIS_API bool fetchCubemap(const base::String& name,Cubemap*& cubemap);
		
		const std::map<std::string,base::Float4>& mapFloat4() const;
		const std::map<std::string,base::Float4x4>& mapFloat4x4() const;
		const std::map<std::string,ovis_int32>& mapInt() const;
		const std::map<std::string,bool>& mapBool() const;
		const std::map<std::string,Texture2D*>& mapTex2D() const;
		const std::map<std::string,Cubemap*>& mapCubemap() const;

		OVIS_API bool isValid() const;

		inline void refcountTextures(const bool b) { m_RefcountTextures=b; }
		inline bool texturesRefcounted() const { return m_RefcountTextures; }
	protected:
		bool m_RefcountTextures;

		struct STLMaps;
		STLMaps *m_pSTLMaps;
	};

}
}

#endif
