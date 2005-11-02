#include <d3dx9.h>

#include "../base/datablock.hh"
#include "../base/log.hh"
#include "d3d9vertexprogram.hh"
#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	D3D9Vertexprogram::D3D9Vertexprogram(D3D9Device& d3d9device,const base::String& identifier,
		const base::String& entrypoint,const base::String& program,const base::String& format):
		Program(d3d9device,identifier,entrypoint,format),m_rD3D9Device(d3d9device),m_pVS(0),m_pConstantTable(0),
		m_IsValid(true)
	{

		if ((format=="d3d_vs11")
		|| (format=="d3d_vs20")
		|| (format=="d3d_vs30")) {
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
				hr=d3d9device.d3d9deviceinterface()->CreateVertexShader(
					(CONST DWORD*)(pBuffer->GetBufferPointer()),&m_pVS);

				if (FAILED(hr)) {
					m_IsValid=false;

					base::log("D3D9Vertexprogram()",base::Error) << "could not create vertex program \"" << format << "\n";
				}
			}

			if (pBuffer) pBuffer->Release();
		} else if ((format=="d3d_hlsl_vs11")
		|| (format=="d3d_hlsl_vs20")
		|| (format=="d3d_hlsl_vs2a")
		|| (format=="d3d_hlsl_vs30")) {
			LPD3DXBUFFER pBuffer,pErrors;

			const char *profile=0;

			if (format=="d3d_hlsl_vs11") profile="vs_1_1";
			else if (format=="d3d_hlsl_vs20") profile="vs_2_0";
			else if (format=="d3d_hlsl_vs2a") profile="vs_2_a";
			else if (format=="d3d_hlsl_vs30") profile="vs_3_0";

			if (profile==0) {
				m_IsValid=false;
				base::log("D3D9Vertexprogram()",base::Error) << "invalid HLSL format \"" << format << "\"\n";

				return;
			}

			HRESULT hr=D3DXCompileShader(program.cstr(),program.length(),0,0,entrypoint.cstr(),profile,0,
				&pBuffer,&pErrors,&m_pConstantTable);

			if (FAILED(hr)) {
				m_IsValid=false;

				base::log("D3D9Vertexprogram()",base::Error) << "could not compile vertex program \"" << identifier << "\"\n";
			}

			if (pErrors) {
				base::log("D3D9Vertexprogram()",base::Message) << "D3DXCompileShader() returns:\n";
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
						base::logstream() << "Vertex program \"" << identifier << "\" has no constants\n";
					} else {
						base::logstream() << "Constant table of vertex program \"" << identifier << "\":\n";

						for (unsigned long l=0;l<cttdesc.Constants;++l) {
							D3DXHANDLE handle=m_pConstantTable->GetConstant(NULL,l);
							D3DXCONSTANT_DESC ctdesc;
							UINT u=1;
							m_pConstantTable->GetConstantDesc(handle,&ctdesc,&u);

							base::logstream() << "[" << ctdesc.Name << "]\n";
						}
					}

				}

				hr=d3d9device.d3d9deviceinterface()->CreateVertexShader(
					(CONST DWORD*)(pBuffer->GetBufferPointer()),&m_pVS);

				if (FAILED(hr)) {
					m_IsValid=false;

					base::log("D3D9Vertexprogram()",base::Error) << "could not create vertex program \"" << format << "\n";
				}
			}

			if (pBuffer) pBuffer->Release();
		}
			
		
	}

	D3D9Vertexprogram::~D3D9Vertexprogram()
	{
		if (m_rD3D9Device.vertexprogram()==this) {
			m_rD3D9Device.vertexprogram(0);
			m_rD3D9Device.d3d9deviceinterface()->SetVertexShader(0);
		}

		if (m_pConstantTable) m_pConstantTable->Release();
		if (m_pVS) m_pVS->Release();
	}

	LPDIRECT3DVERTEXSHADER9 D3D9Vertexprogram::vertexshader()
	{
		return m_pVS;
	}

	void D3D9Vertexprogram::set()
	{
		if (!isValid()) return;

		m_rD3D9Device.vertexprogram(this);
		m_rD3D9Device.d3d9deviceinterface()->SetVertexShader(m_pVS);
	}

	bool D3D9Vertexprogram::isValid() const
	{
		return m_IsValid;
	}

	LPD3DXCONSTANTTABLE D3D9Vertexprogram::constantTable()
	{
		return m_pConstantTable;
	}

	void D3D9Vertexprogram::setFloat(const base::String& varname,const float f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloat(
				m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f);
	}

	void D3D9Vertexprogram::setFloat2(const base::String& varname,const base::Float2& f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloatArray(m_rD3D9Device.d3d9deviceinterface(),
			m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f,2);
	}

	void D3D9Vertexprogram::setFloat3(const base::String& varname,const base::Float3& f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloatArray(m_rD3D9Device.d3d9deviceinterface(),
			m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f,3);
	}

	void D3D9Vertexprogram::setFloat4(const base::String& varname,const base::Float4& f)
	{
		if (m_pConstantTable)
			m_pConstantTable->SetFloatArray(m_rD3D9Device.d3d9deviceinterface(),
			m_pConstantTable->GetConstantByName(NULL,varname.cstr()),f,4);
	}

	void D3D9Vertexprogram::setFloat3x3(const base::String& varname,const base::Float3x3& f)
	{
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,false);
			m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),&m);
		}
	}

	void D3D9Vertexprogram::setFloat4x3(const base::String& varname,const base::Float4x3& f)
	{
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,false);
			m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),&m);
		}
	}

	void D3D9Vertexprogram::setFloat3x4(const base::String& varname,const base::Float3x4& f)
	{
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(base::Float4x4(f),m,false);
			m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),m_pConstantTable->GetConstantByName(NULL,varname.cstr()),&m);
		}
	}

	void D3D9Vertexprogram::setFloat4x4(const base::String& varname,const base::Float4x4& f)
	{
		HRESULT hr;
		if (m_pConstantTable) {
			D3DXMATRIX m; tod3dxmatrix(f,m,false);
			D3DXHANDLE handle=m_pConstantTable->GetConstantByName(NULL,varname.cstr());
			hr=m_pConstantTable->SetMatrix(m_rD3D9Device.d3d9deviceinterface(),handle,&m);
		}
	}

	void D3D9Vertexprogram::bind2DTexture(const base::String& varname,const video::Texture2D* texVar)
	{
	}

	void D3D9Vertexprogram::bindCubemap(const base::String& varname,const video::Cubemap* texVar)
	{
	}



}
}
