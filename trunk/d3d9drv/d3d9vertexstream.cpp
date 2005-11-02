#include <d3dx9.h>

#include "d3d9device.hh"
#include "d3d9vertexstream.hh"
#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	D3D9Vertexstream::D3D9Vertexstream(D3D9Device& d3d9device,
									const video::Vertexformat& format,
									const ovis_uint32 flags,
									const video::Memorypool mempool,
									const ovis_uint32 _capacity,
									video::Vertexstream::Vertexsource *pSource):
	Vertexstream(format,_capacity,pSource),m_rD3D9Device(d3d9device),m_pMappedPointer(0),m_IsDataOK(true)
	{
		m_FVF=0;
		DWORD vbflags=0;

		if (flags&video::Streamflags_Points) vbflags|=D3DUSAGE_POINTS;
		if (flags&video::Streamflags_Writeonly) vbflags|=D3DUSAGE_WRITEONLY;
		if (flags&video::Streamflags_Dynamic) vbflags|=D3DUSAGE_DYNAMIC;

		unsigned long numtexcoords=0;

		for (unsigned long fentry=0;fentry<format.numEntries();++fentry) {
			switch (format.entry(fentry)) {
				case video::VertexFormatEntry_Position:m_FVF|=D3DFVF_XYZ; break;
				case video::VertexFormatEntry_Normal:m_FVF|=D3DFVF_NORMAL; break;
				case video::VertexFormatEntry_Diffuse:m_FVF|=D3DFVF_DIFFUSE; break;
				case video::VertexFormatEntry_Specular:m_FVF|=D3DFVF_SPECULAR; break;
				case video::VertexFormatEntry_Texcoord1D:m_FVF|=D3DFVF_TEXCOORDSIZE1(numtexcoords); ++numtexcoords; break;
				case video::VertexFormatEntry_Texcoord2D:m_FVF|=D3DFVF_TEXCOORDSIZE2(numtexcoords); ++numtexcoords; break;
				case video::VertexFormatEntry_Texcoord3D:m_FVF|=D3DFVF_TEXCOORDSIZE3(numtexcoords); ++numtexcoords; break;
				default:break;
			}
		}

		m_FVF|=(numtexcoords<<D3DFVF_TEXCOUNT_SHIFT);

		D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
		D3DXDeclaratorFromFVF(m_FVF,decl);

		d3d9device.d3d9deviceinterface()->CreateVertexDeclaration(decl,&m_pDecl);

		d3d9device.d3d9deviceinterface()->CreateVertexBuffer(
			format.stride()*_capacity,vbflags,m_FVF,d3dmempool(mempool),&m_pVB,0);

		m_Pointers.m_CurrentVtx=0;
		m_Pointers.m_pp1DTexcoords=m_pp1DTexcoords;
		m_Pointers.m_pp2DTexcoords=m_pp2DTexcoords;
		m_Pointers.m_pp3DTexcoords=m_pp3DTexcoords;
	}

	D3D9Vertexstream::~D3D9Vertexstream()
	{
		unmap();

		if (m_pVB) m_pVB->Release();
		if (m_pDecl) m_pDecl->Release();
	}

	bool D3D9Vertexstream::isValid() const
	{
		return (m_pVB!=0);
	}

	bool D3D9Vertexstream::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Vertexstream::dataIsOK() { m_IsDataOK=true; }

	void D3D9Vertexstream::bind()
	{
		if (m_pMappedPointer!=0) return;

		if (m_rD3D9Device.vertexprogram()) m_rD3D9Device.d3d9deviceinterface()->SetVertexDeclaration(m_pDecl);
		else m_rD3D9Device.d3d9deviceinterface()->SetFVF(m_FVF);

		m_rD3D9Device.d3d9deviceinterface()->SetStreamSource(0,
			m_pVB,0,m_Format.stride());
	}

	void D3D9Vertexstream::map(const ovis_uint32 flags)
	{
		if (m_pMappedPointer) return;

		DWORD lockflags=0;
		if (flags&video::Map_Readonly) lockflags|=D3DLOCK_READONLY;
		if (flags&video::Map_Discard) lockflags|=D3DLOCK_DISCARD;
		if (flags&video::Map_Nooverwrite) lockflags|=D3DLOCK_NOOVERWRITE;

		m_pVB->Lock(0,0,(void**)&m_pMappedPointer,lockflags);
		calculatePointers(m_Pointers);
	}

	void D3D9Vertexstream::unmap()
	{
		if (m_pMappedPointer) m_pVB->Unlock();
		m_pMappedPointer=0;
	}

	void D3D9Vertexstream::calculatePointers(video::VertexiteratorPointers& vitp)
	{
		if ((vitp.m_CurrentVtx>=capacity()) || !isMapped()) return;

		ovis_uint8* pPtr=m_pMappedPointer+m_Format.stride()*vitp.m_CurrentVtx;

		if (m_Format.contains(video::VertexFormatEntry_Position,0)) {
			vitp.m_pPosition=(math::Vector3f*)pPtr;
			pPtr+=3*sizeof(float);
		}

		if (m_Format.contains(video::VertexFormatEntry_Normal,0)) {
			vitp.m_pNormal=(math::Vector3f*)pPtr;
			pPtr+=3*sizeof(float);
		}

		if (m_Format.contains(video::VertexFormatEntry_Diffuse,0)) {
			vitp.m_pDiffuseColor=(ovis_uint32*)pPtr;
			pPtr+=sizeof(ovis_uint32);
		}

		if (m_Format.contains(video::VertexFormatEntry_Specular,0)) {
			vitp.m_pSpecularColor=(ovis_uint32*)pPtr;
			pPtr+=sizeof(ovis_uint32);
		}

		int numtexcoords[3]={0,0,0};
		for (unsigned long entrynr=0;entrynr<m_Format.numEntries();++entrynr) {
			video::VertexFormatEntry entry=m_Format.entry(entrynr);
			switch (entry) {
				case video::VertexFormatEntry_Texcoord1D:
					vitp.m_pp1DTexcoords[numtexcoords[0]++]=(float*)pPtr;
					pPtr+=1*sizeof(float);
					break;
				case video::VertexFormatEntry_Texcoord2D:
					vitp.m_pp2DTexcoords[numtexcoords[1]++]=(math::Vector2f*)pPtr;
					pPtr+=2*sizeof(float);
					break;
				case video::VertexFormatEntry_Texcoord3D:
					vitp.m_pp3DTexcoords[numtexcoords[2]++]=(math::Vector3f*)pPtr;
					pPtr+=3*sizeof(float);
					break;
				default:break;
			}
		}

	}


	void D3D9Vertexstream::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Vertexstream::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}

	const math::Vector3f& D3D9Vertexstream::position(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	const math::Vector3f& D3D9Vertexstream::normal(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	ovis_uint32 D3D9Vertexstream::diffuseColor(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		return *((ovis_uint32*)pPtr);
	}

	ovis_uint32 D3D9Vertexstream::specularColor(const ovis_uint32 vtxindex) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		return *((ovis_uint32*)pPtr);
	}

	float D3D9Vertexstream::texcoord1D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((float*)pPtr);
	}

	const math::Vector2f& D3D9Vertexstream::texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector2f*)pPtr);
	}

	const math::Vector3f& D3D9Vertexstream::texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage) const
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		return *((math::Vector3f*)pPtr);
	}

	void D3D9Vertexstream::position(const ovis_uint32 vtxindex,const float x,const float y,const float z)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void D3D9Vertexstream::position(const ovis_uint32 vtxindex,const math::Vector3f& newPosition)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pPosition)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newPosition.x();
		((math::Vector3f*)pPtr)->y()=newPosition.y();
		((math::Vector3f*)pPtr)->z()=newPosition.z();
	}

	void D3D9Vertexstream::normal(const ovis_uint32 vtxindex,const float x,const float y,const float z)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=x;
		((math::Vector3f*)pPtr)->y()=y;
		((math::Vector3f*)pPtr)->z()=z;
	}

	void D3D9Vertexstream::normal(const ovis_uint32 vtxindex,const math::Vector3f& newNormal)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pNormal)+m_Format.stride()*vtxindex;
		((math::Vector3f*)pPtr)->x()=newNormal.x();
		((math::Vector3f*)pPtr)->y()=newNormal.y();
		((math::Vector3f*)pPtr)->z()=newNormal.z();
	}

	void D3D9Vertexstream::diffuseColor(const ovis_uint32 vtxindex,const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ovis_uint32*)pPtr)=(((ovis_uint32)a)<<24)|(((ovis_uint32)r)<<16)|(((ovis_uint32)g)<<8)|((ovis_uint32)b);
	}

	void D3D9Vertexstream::diffuseColor(const ovis_uint32 vtxindex,const ovis_uint32 color)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pDiffuseColor)+m_Format.stride()*vtxindex;
		*((ovis_uint32*)pPtr)=color;
	}

	void D3D9Vertexstream::specularColor(const ovis_uint32 vtxindex,const ovis_uint8 a,const ovis_uint8 r,const ovis_uint8 g,const ovis_uint8 b)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ovis_uint32*)pPtr)=(((ovis_uint32)a)<<24)|(((ovis_uint32)r)<<16)|(((ovis_uint32)g)<<8)|((ovis_uint32)b);
	}

	void D3D9Vertexstream::specularColor(const ovis_uint32 vtxindex,const ovis_uint32 color)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pSpecularColor)+m_Format.stride()*vtxindex;
		*((ovis_uint32*)pPtr)=color;
	}

	void D3D9Vertexstream::texcoord1D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float newTexcoord1D)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp1DTexcoords[texstage])+m_Format.stride()*vtxindex;
		*((float*)pPtr)=newTexcoord1D;
	}

	void D3D9Vertexstream::texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float u,const float v)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
	}

	void D3D9Vertexstream::texcoord2D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp2DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord2D.x();
		((float*)pPtr)[1]=newTexcoord2D.y();
	}

	void D3D9Vertexstream::texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const float u,const float v,const float w)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=u;
		((float*)pPtr)[1]=v;
		((float*)pPtr)[2]=w;
	}

	void D3D9Vertexstream::texcoord3D(const ovis_uint32 vtxindex,const ovis_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		ovis_uint8 *pPtr=(ovis_uint8*)(m_Pointers.m_pp3DTexcoords[texstage])+m_Format.stride()*vtxindex;
		((float*)pPtr)[0]=newTexcoord3D.x();
		((float*)pPtr)[1]=newTexcoord3D.y();
		((float*)pPtr)[2]=newTexcoord3D.z();
	}

}
}
