#ifndef OVIS_D3D9DRV_D3D9FRAGMENTPROGRAM_HH_INCLUDED
#define OVIS_D3D9DRV_D3D9FRAGMENTPROGRAM_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "../base/dll.hh"
#include "d3d9device.hh"

namespace ovis {
namespace d3d9drv {

	class OVIS_API D3D9Fragmentprogram:public video::Program
	{
	public:
		D3D9Fragmentprogram(D3D9Device& d3d9device,const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);
		~D3D9Fragmentprogram();

		LPDIRECT3DPIXELSHADER9 pixelshader();
		void set();
		bool isValid() const;
		LPD3DXCONSTANTTABLE constantTable();

		void setFloat(const base::String& varname,const float);
		void setFloat2(const base::String& varname,const base::Float2&);
		void setFloat3(const base::String& varname,const base::Float3&);
		void setFloat4(const base::String& varname,const base::Float4&);
		void setFloat3x3(const base::String& varname,const base::Float3x3&);
		void setFloat4x3(const base::String& varname,const base::Float4x3&);
		void setFloat3x4(const base::String& varname,const base::Float3x4&);
		void setFloat4x4(const base::String& varname,const base::Float4x4&);
		void bind2DTexture(const base::String& varname,const video::Texture2D* texVar);
		void bindCubemap(const base::String& varname,const video::Cubemap* texVar);


	protected:
		D3D9Device& m_rD3D9Device;

		LPDIRECT3DPIXELSHADER9 m_pPS;
		LPD3DXCONSTANTTABLE m_pConstantTable;
		bool m_IsValid;
	};

}
}

#endif
