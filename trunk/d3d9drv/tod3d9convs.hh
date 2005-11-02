#ifndef OVIS_D3D9DRV_D3D9CONVERSIONS_HH_INCLUDED
#define OVIS_D3D9DRV_D3D9CONVERSIONS_HH_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <d3d9.h>
#include <d3dx9.h>

#include "../base/numericarrays.hh"
#include "../video/coreenums.hh"
#include "../video/mempool.hh"
#include "../video/pixelformat.hh"

namespace ovis {
namespace d3d9drv {

	D3DFORMAT d3dpixelformat(const video::Pixelformat ionpixelformat);
	D3DPRIMITIVETYPE d3dprimitivetype(const video::PrimitivesType ionprimitivestype);
	D3DBLEND d3dblendparameter(const video::BlendFunc ionblendfunc);
	D3DCMPFUNC d3dcomparefunc(const video::CompareFunc ioncmpfunc);
	D3DSTENCILOP d3dstenciloperation(const video::StencilOp ionstencilop);
	D3DPOOL d3dmempool(const video::Memorypool ionmemorypool);
	D3DFILLMODE d3dfillingmode(const video::Drawingmode iondrawingmode);
	DWORD d3dtexcoordgen(const video::Texcoordgeneration iontexgen);

	D3DSAMPLERSTATETYPE d3dsamplerstatetype(const video::Samplerstageparameter ionsampparameter);
	D3DTEXTUREFILTERTYPE d3dtexturefilter(const video::Texfilter iontexfilter);
	D3DTEXTUREADDRESS d3dtextureaddress(const video::Texwrap iontexwrap);

	D3DTEXTURESTAGESTATETYPE d3dtexstagestatetype(const video::Texstageparameter iontexstageparameter);
	D3DTEXTUREOP d3dtextureop(const video::Texstageoperation iontexop);
	DWORD d3dtexturearg(const video::Texstageargument iontexarg);

	video::Pixelformat fromd3dpixelformat(const D3DFORMAT format);

	void tod3dmatrix(const base::Float4x4& srcmatrix,D3DMATRIX& destmatrix,const bool transpose);
	void tod3dxmatrix(const base::Float4x4& srcmatrix,D3DXMATRIX& destmatrix,const bool transpose);

}
}

#endif
