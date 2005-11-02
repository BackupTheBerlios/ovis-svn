#include <d3dx9.h>

#include "../base/datablock.hh"
#include "../base/log.hh"
#include "d3d9cubemap.hh"
#include "d3d9fragmentprogram.hh"
#include "d3d9texture2d.hh"
#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	D3D9Fragmentprogram::D3D9Fragmentprogram(D3D9Device& d3d9device,const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format):
		Program(d3d9device,identifier,entrypoint,format),m_rD3D9Device(d3d9device),m_pPS(0),m_pConstantTable(0),
		m_IsValid(true)
	{

		if ((format=="d3d_ps11")
		|| (format=="d3d_ps12")
		|| (format=="d3d_ps13")
		|| (format=="d3d_ps14")
		|| (format=="d3d_ps20")
		|| (format=="d3d_ps30")) {
			// An assembly shader (no HLSL)

			LPD3DXBUFFER pBuffer,pErrors;

			HRESULT hr=D3DXAssembleShader(program.cstr(),program.length(),0,0,0,&pBuffer,&pErrors);

			if (FAILED(hr)) {
				m_IsValid=false;

				base::log("D3D9Vertexprogram()",base::Error) << "could not assemble vertex program \"" << identifier << "\"\n";
			}

			if (pErrors) {
				base::log("D3D9Vertexprogram()",base::Message) << "D3DXAssembleShader() returns:\n";
				base::Datablock dblock;
				dblock.resize(pErrors->GetBufferSize()+1);
				memcpy(dblock.data(),pErrors->GetBufferPointer(),pErrors->GetBufferSize());
				dblock.data()[dblock.size()-1]=0;
				base::logstream() << ((char*)(dblock.data())) << "\nD3DXAssembleShader() output end\n";

				pErrors->Release();
			}

			if (m_IsValid) {
				hr=d3d9device.d3d9deviceinterface()->CreatePixelShader(
					(CONST DWORD*)(pBuffer->GetBufferPointer()),&m_pPS);

				if (FAILED(hr)) {
					m_IsValid=false;

					base::log("D3D9Fragmentprogram()",base::Error) << "could not create fragment program \"" << format << "\n";
				}
			}

			if (pBuffer) pBuffer->Release();
		} else if ((format=="d3d_hlsl_ps11")
		|| (format=="d3d_hlsl_ps12")
		|| (format=="d3d_hlsl_ps13")
		|| (format=="d3d_hlsl_ps14")
		|| (format=="d3d_hlsl_ps20")
		|| (format=="d3d_hlsl_ps2a")
		|| (format=="d3d_hlsl_ps30")) {
			LPD3DXBUFFER pBuffer,pErrors;

			const char *profile=0;

			if (format=="d3d_hlsl_ps11") profile="ps_1_1";
			else if (format=="d3d_hlsl_ps12") profile="ps_1_2";
			else if (format=="d3d_hlsl_ps13") profile="ps_1_3";
			else if (format=="d3d_hlsl_ps14") profile="ps_1_4";
			else if (format=="d3d_hlsl_ps20") profile="ps_2_0";
			else if (format=="d3d_hlsl_ps2a") profile="ps_2_a";
			else if (format=="d3d_hlsl_ps30") profile="ps_3_0";

			if (profile==0) {
				m_IsValid=false;
				base::log("D3D9Fragmentprogram()",base::Error) << "invalid HLSL format \"" << format << "\"\n";

				return;
			}

			HRESULT hr=D3DXCompileShader(program.cstr(),program.length(),0,0,entrypoint.cstr(),profile,0,
				&pBuffer,&pErrors,&m_pConstantTable);

			if (FAILED(hr)) {
				m_IsValid=false;

				base::log("D3D9Fragmentprogram()",base::Error) << "could not compile fragment program \"" << identifier << "\"\n";
			}

			if (pErrors) {
				base::log("D3D9Fragmentprogram()",base::Message) << "D3DXCompileShader() returns:\n";
				base::Datablock dblock;
				dblock.resize(pErrors->GetBufferSize()+1);
				memcpy(dblock.data(),pErrors->GetBufferPointer(),pErrors->GetBufferSize());
				dblock.data()[dblock.size()-1]=0;
				base::logstream() << ((char*)(dblock.data())) << "\nD3DXCompileShader() output end\n";

				pErrors->Release();
			}

			if (m_IsValid) {

				// Dump the constant table
				{
					D3DXCONSTANTTABLE_DESC cttdesc;
					m_pConstantTable->GetDesc(&cttdesc);

					if (cttdesc.Constants==0) {
						base::logstream() << "Fragment program \"" << identifier << "\" has no constants\n";
					} else {
						base::logstream() << "Constant table of fragment program \"" << identifier << "\":\n";

						for (unsigned long l=0;l<cttdesc.Constants;++l) {
							D3DXHANDLE handle=m_pConstantTable->GetConstant(NULL,l);
							D3DXCONSTANT_DESC ctdesc;
							UINT u=1;
							m_pConstantTable->GetConstantDesc(handle,&ctdesc,&u);

							base::logstream() << "[" << ctdesc.Name << "]\n";
						}
					}

				}

				hr=d3d9device.d3d9deviceinterface()->CreatePixelShader(
					(CONST DWORD*)(pBuffer->GetBufferPointer()),&m_pPS);

				if (FAILED(hr)) {
					m_IsValid=false;

					base::log("D3D9Fragmentprogram()",base::Error) << "could not create fragment program \"" << format << "\n";
				}
			}

			if (pBuffer) pBuffer->Release();
		}
			
		
	}

	D3D9Fragmentprogram::~D3D9Fragmentprogram()
	{
		if (m_rD3D9Device.fragmentprogram()==this) {
			m_rD3D9Device.fragmentprogram(0);
			m_rD3D9Device.d3d9deviceinterface()->SetPixelShader(0);
		}

		if (m_pConstantTable) m_pConstantTable->Release();
		if (m_pPS) m_pPS->Release();
	}

	LPDIRECT3DPIXELSHADER9 D3D9Fragmentprogram::pixelshader()
	{
		return m_pPS;
	}

	void D3D9Fragmentprogram::set()
	{
		if (!isValid()) return;

		m_rD3D9Device.fragmentprogram(this);
		m_rD3D9Device.d3d9deviceinterface()->SetPixelShader(m_pPS);
	}

	bool D3D9Fragmentprogram::isValid() const
	{
		return m_IsValid;
	}

	LPD3DXCONSTANTTABLE D3D9Fragmentprogram::constantTable()
	{
		return m_pConstantTable;
	}

	void D3D9Fragmentprogram::setFloat(const base::String& varname,const float f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloat(
				m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f);
	}

	void D3D9Fragmentprogram::setFloat2(const base::String& varname,const base::Float2& f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloatArray(m_rD3D9Device.d3d9deviceinterface(),
			m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f,2);
	}

	void D3D9Fragmentprogram::setFloat3(const base::String& varname,const base::Float3& f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloatArray(m_rD3D9Device.d3d9deviceinterface(),
			m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f,3);
	}

	void D3D9Fragmentprogram::setFloat4(const base::String& varname,const base::Float4& f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloatArray(m_rD3D9Device.d3d9deviceinterface(),
			m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f,4);
	}

	void D3D9Fragmentprogram::setFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,false);
			m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),&m);
		}
	}

	void D3D9Fragmentprogram::setFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,false);
			m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),&m);
		}
	}

	void D3D9Fragmentprogram::setFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,false);
			m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),&m);
		}
	}

	void D3D9Fragmentprogram::setFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
		HRESULT hr;
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(f,m,false);
			D3DXHANDLE handle=m_pConstantTable->GetConstantByName(NULL,varname.cstr());
			hr=m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),handle,&m);
		}
	}

	void D3D9Fragmentprogram::bind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
		HRESULT hr;
		if (m_pConstantTable) {
			D3DXHANDLE handle=m_pConstantTable->GetConstantByName(NULL,varname.cstr());
			UINT samplerstage=m_pConstantTable->GetSamplerIndex(handle);
			hr=m_rD3D9Device.d3d9deviceinterface()->SetTexture(samplerstage,
				((D3D9Texture2D*)texVar)->d3d9texture());
		}
	}

	void D3D9Fragmentprogram::bindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
		HRESULT hr;
		if (m_pConstantTable) {
			D3DXHANDLE handle=m_pConstantTable->GetConstantByName(NULL,varname.cstr());
			UINT samplerstage=m_pConstantTable->GetSamplerIndex(handle);
			hr=m_rD3D9Device.d3d9deviceinterface()->SetTexture(samplerstage,
				((D3D9Cubemap*)texVar)->d3d9cubetexture());
		}
	}



}
}
