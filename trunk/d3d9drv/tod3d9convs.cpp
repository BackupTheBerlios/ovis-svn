#include "tod3d9convs.hh"

namespace ovis {
namespace d3d9drv {

	D3DFORMAT d3dpixelformat(const video::Pixelformat ionpixelformat)
	{
		switch (ionpixelformat) {
			case video::Pixelformat_RGB888:return D3DFMT_R8G8B8;
			case video::Pixelformat_XRGB8888:return D3DFMT_X8R8G8B8;
			case video::Pixelformat_XBGR8888:return D3DFMT_X8B8G8R8;
			case video::Pixelformat_ARGB8888:return D3DFMT_A8R8G8B8;
			case video::Pixelformat_ABGR8888:return D3DFMT_A8B8G8R8;
			case video::Pixelformat_RGB565:return D3DFMT_R5G6B5;
			case video::Pixelformat_XRGB1555:return D3DFMT_X1R5G5B5;
			case video::Pixelformat_ARGB1555:return D3DFMT_A1R5G5B5;
			case video::Pixelformat_ARGB4444:return D3DFMT_A4R4G4B4;
			case video::Pixelformat_RGB332:return D3DFMT_R3G3B2;
			case video::Pixelformat_RGB444:return D3DFMT_X4R4G4B4;
			case video::Pixelformat_A2RGB101010:return D3DFMT_A2R10G10B10;
			case video::Pixelformat_A2BGR101010:return D3DFMT_A2B10G10R10;
			case video::Pixelformat_ARGB16161616:return D3DFMT_A16B16G16R16;
			case video::Pixelformat_FARGB16161616:return D3DFMT_A16B16G16R16F; // TODO: Rename FARGB to FABGR
			case video::Pixelformat_FARGB32323232:return D3DFMT_A32B32G32R32F; // TODO: Rename FARGB to FABGR
			case video::Pixelformat_D16:return D3DFMT_D16;
			case video::Pixelformat_D24:return D3DFMT_D24X8;
			case video::Pixelformat_D32:return D3DFMT_D32;
			case video::Pixelformat_D15S1:return D3DFMT_D15S1;
			case video::Pixelformat_D24S4:return D3DFMT_D24X4S4;
			case video::Pixelformat_D24S8:return D3DFMT_D24X8;
			case video::Pixelformat_FD24S8:return D3DFMT_D24FS8;
			case video::Pixelformat_RGB_DXT1:return D3DFMT_DXT1;
			case video::Pixelformat_RGBA_DXT1:return D3DFMT_DXT2; // TODO: Examine this one
			case video::Pixelformat_RGBA_DXT3:return D3DFMT_DXT3;
			case video::Pixelformat_RGBA_DXT5:return D3DFMT_DXT5;
			default:return D3DFMT_UNKNOWN;
		}
	}

	video::Pixelformat fromd3dpixelformat(const D3DFORMAT format)
	{
		switch (format) {
			case D3DFMT_R8G8B8:return video::Pixelformat_RGB888;
			case D3DFMT_X8R8G8B8:return video::Pixelformat_XRGB8888;
			case D3DFMT_X8B8G8R8:return video::Pixelformat_XBGR8888;
			case D3DFMT_A8R8G8B8:return video::Pixelformat_ARGB8888;
			case D3DFMT_A8B8G8R8:return video::Pixelformat_ABGR8888;
			case D3DFMT_R5G6B5:return video::Pixelformat_RGB565;
			case D3DFMT_X1R5G5B5:return video::Pixelformat_XRGB1555;
			case D3DFMT_A1R5G5B5:return video::Pixelformat_ARGB1555;
			case D3DFMT_A4R4G4B4:return video::Pixelformat_ARGB4444;
			case D3DFMT_R3G3B2:return video::Pixelformat_RGB332;
			case D3DFMT_X4R4G4B4:return video::Pixelformat_RGB444;
			case D3DFMT_A2R10G10B10:return video::Pixelformat_A2RGB101010;
			case D3DFMT_A2B10G10R10:return video::Pixelformat_A2BGR101010;
			case D3DFMT_A16B16G16R16:return video::Pixelformat_ARGB16161616;
			case D3DFMT_A16B16G16R16F:return video::Pixelformat_FARGB16161616;
			case D3DFMT_A32B32G32R32F:return video::Pixelformat_FARGB32323232;
			case D3DFMT_D16:return video::Pixelformat_D16;
			case D3DFMT_D24X8:return video::Pixelformat_D24;
			case D3DFMT_D32:return video::Pixelformat_D32;
			case D3DFMT_D15S1:return video::Pixelformat_D15S1;
			case D3DFMT_D24X4S4:return video::Pixelformat_D24S4;
			case D3DFMT_D24S8:return video::Pixelformat_D24S8;
			case D3DFMT_D24FS8:return video::Pixelformat_FD24S8;
			case D3DFMT_DXT1:return video::Pixelformat_RGB_DXT1;
			case D3DFMT_DXT2:return video::Pixelformat_RGBA_DXT1;
			case D3DFMT_DXT3:return video::Pixelformat_RGBA_DXT3;
			case D3DFMT_DXT5:return video::Pixelformat_RGBA_DXT5;
			default:return video::Pixelformat_Unknown;
		}
	}

	D3DPRIMITIVETYPE d3dprimitivetype(const video::PrimitivesType ionprimitivestype)
	{
		switch (ionprimitivestype) {
			case video::Primitives_Points:return D3DPT_POINTLIST;
			case video::Primitives_Lines:return D3DPT_LINELIST;
			case video::Primitives_Linestrip:return D3DPT_LINESTRIP;
			case video::Primitives_Triangles:return D3DPT_TRIANGLELIST;
			case video::Primitives_Trianglestrip:return D3DPT_TRIANGLESTRIP;
			case video::Primitives_Trianglefan:return D3DPT_TRIANGLEFAN;
			default:return D3DPT_FORCE_DWORD;
		}
	}

	D3DBLEND d3dblendparameter(const video::BlendFunc ionblendfunc)
	{
		switch (ionblendfunc) {
			case video::BlendFunc_Zero:return D3DBLEND_ZERO;
			case video::BlendFunc_One:return D3DBLEND_ONE;
			case video::BlendFunc_Src_Color:return D3DBLEND_SRCCOLOR;
			case video::BlendFunc_One_Minus_Src_Color:return D3DBLEND_INVSRCCOLOR;
			case video::BlendFunc_Src_Alpha:return D3DBLEND_SRCALPHA;
			case video::BlendFunc_One_Minus_Src_Alpha:return D3DBLEND_INVSRCALPHA;
			case video::BlendFunc_Dest_Alpha:return D3DBLEND_DESTALPHA;
			case video::BlendFunc_One_Minus_Dest_Alpha:return D3DBLEND_INVDESTALPHA;
			case video::BlendFunc_Dest_Color:return D3DBLEND_DESTCOLOR;
			case video::BlendFunc_One_Minus_Dest_Color:return D3DBLEND_INVDESTCOLOR;
			default:return D3DBLEND_FORCE_DWORD;
		}
	}

	D3DCMPFUNC d3dcomparefunc(const video::CompareFunc ioncmpfunc)
	{
		switch (ioncmpfunc) {
			case video::CompareFunc_Never:return D3DCMP_NEVER;
			case video::CompareFunc_Less:return D3DCMP_LESS;
			case video::CompareFunc_Lessequal:return D3DCMP_LESSEQUAL;
			case video::CompareFunc_Equal:return D3DCMP_EQUAL;
			case video::CompareFunc_Greaterequal:return D3DCMP_GREATEREQUAL;
			case video::CompareFunc_Greater:return D3DCMP_GREATER;
			case video::CompareFunc_Notequal:return D3DCMP_NOTEQUAL;
			case video::CompareFunc_Always:return D3DCMP_ALWAYS;
			default:return D3DCMP_FORCE_DWORD;
		}
	}

	D3DSTENCILOP d3dstenciloperation(const video::StencilOp ionstencilop)
	{
		switch (ionstencilop) {
			case video::StencilOp_Keep:return D3DSTENCILOP_KEEP;
			case video::StencilOp_Zero:return D3DSTENCILOP_ZERO;
			case video::StencilOp_Set_To_Reference:return D3DSTENCILOP_REPLACE;
			case video::StencilOp_Incr:return D3DSTENCILOP_INCR;
			case video::StencilOp_Decr:return D3DSTENCILOP_DECR;
			case video::StencilOp_Invert:return D3DSTENCILOP_INVERT;
			default:return D3DSTENCILOP_FORCE_DWORD;
		}
	}

	D3DFILLMODE d3dfillingmode(const video::Drawingmode iondrawingmode)
	{
		switch (iondrawingmode) {
			case video::Drawingmode_Points:return D3DFILL_POINT;
			case video::Drawingmode_Lines:return D3DFILL_WIREFRAME;
			case video::Drawingmode_Faces:return D3DFILL_SOLID;
			default:return D3DFILL_FORCE_DWORD;
		}
	}

	D3DPOOL d3dmempool(const video::Memorypool ionmemorypool)
	{
		switch (ionmemorypool) {
			case video::Mempool_Default:return D3DPOOL_DEFAULT;
			case video::Mempool_Systemmem:return D3DPOOL_SYSTEMMEM;
			case video::Mempool_Managed:return D3DPOOL_MANAGED;
			default:return D3DPOOL_FORCE_DWORD;
		}
	}

	DWORD d3dtexcoordgen(const video::Texcoordgeneration iontexgen)
	{
		switch(iontexgen) {
			case video::Texcoordgeneration_None:return D3DTSS_TCI_PASSTHRU;
			case video::Texcoordgeneration_CameraspaceNormal:return D3DTSS_TCI_CAMERASPACENORMAL;
			case video::Texcoordgeneration_CameraspacePosition:return D3DTSS_TCI_CAMERASPACEPOSITION;
			case video::Texcoordgeneration_CameraspaceReflectionVector:return D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
			case video::Texcoordgeneration_Spheremapping:return D3DTSS_TCI_SPHEREMAP;
			default:return 0xFFFFFFFF;
		}
	}










	D3DSAMPLERSTATETYPE d3dsamplerstatetype(const video::Samplerstageparameter ionsampparameter)
	{
		switch (ionsampparameter) {
			case video::Samplerstageparameter_Minfilter:return D3DSAMP_MINFILTER;
			case video::Samplerstageparameter_Magfilter:return D3DSAMP_MAGFILTER;
			case video::Samplerstageparameter_Mipfilter:return D3DSAMP_MIPFILTER;
			case video::Samplerstageparameter_UWrap:return D3DSAMP_ADDRESSU;
			case video::Samplerstageparameter_VWrap:return D3DSAMP_ADDRESSV;
			case video::Samplerstageparameter_WWrap:return D3DSAMP_ADDRESSW;
			case video::Samplerstageparameter_MaxAnisotropy:return D3DSAMP_MAXANISOTROPY;
			default:return D3DSAMP_FORCE_DWORD;
		}
	}

	D3DTEXTUREFILTERTYPE d3dtexturefilter(const video::Texfilter iontexfilter)
	{
		switch (iontexfilter) {
			case video::Texfilter_Point:return D3DTEXF_POINT;
			case video::Texfilter_Linear:return D3DTEXF_LINEAR;
			case video::Texfilter_Anisotropic:return D3DTEXF_ANISOTROPIC;
			default:return D3DTEXF_FORCE_DWORD;
		}
	}

	D3DTEXTUREADDRESS d3dtextureaddress(const video::Texwrap iontexwrap)
	{
		switch (iontexwrap) {
			case video::Texwrap_Repeat:return D3DTADDRESS_WRAP;
			case video::Texwrap_Mirror:return D3DTADDRESS_MIRROR;
			case video::Texwrap_Clamp:return D3DTADDRESS_CLAMP;
			default:return D3DTADDRESS_FORCE_DWORD;
		}
	}










	D3DTEXTURESTAGESTATETYPE d3dtexstagestatetype(const video::Texstageparameter iontexstageparameter)
	{
		switch (iontexstageparameter) {
			case video::Texstageparameter_Colorop:return D3DTSS_COLOROP;
			case video::Texstageparameter_Colorarg1:return D3DTSS_COLORARG1;
			case video::Texstageparameter_Colorarg2:return D3DTSS_COLORARG2;
			case video::Texstageparameter_Alphaop:return D3DTSS_ALPHAOP;
			case video::Texstageparameter_Alphaarg1:return D3DTSS_ALPHAARG1;
			case video::Texstageparameter_Alphaarg2:return D3DTSS_ALPHAARG2;
			case video::Texstageparameter_Texcoordindex:return D3DTSS_TEXCOORDINDEX;
			case video::Texstageparameter_Textransformations:return D3DTSS_TEXTURETRANSFORMFLAGS;
			default:return D3DTSS_FORCE_DWORD;
		}
	}

	D3DTEXTUREOP d3dtextureop(const video::Texstageoperation iontexop)
	{
		switch (iontexop) {
			case video::Texstageoperation_Disable:return D3DTOP_DISABLE;
			case video::Texstageoperation_Selectarg1:return D3DTOP_SELECTARG1;
			case video::Texstageoperation_Selectarg2:return D3DTOP_SELECTARG2;
			case video::Texstageoperation_Modulate1x:return D3DTOP_MODULATE;
			case video::Texstageoperation_Modulate2x:return D3DTOP_MODULATE2X;
			case video::Texstageoperation_Modulate4x:return D3DTOP_MODULATE4X;
			case video::Texstageoperation_Add:return D3DTOP_ADD;
			case video::Texstageoperation_Addsigned:return D3DTOP_ADDSIGNED;
			case video::Texstageoperation_Addsigned2x:return D3DTOP_ADDSIGNED2X;
			case video::Texstageoperation_Subtract:return D3DTOP_SUBTRACT;
			case video::Texstageoperation_Addsmooth:return D3DTOP_ADDSMOOTH;
			case video::Texstageoperation_BlendWithDiffusealpha:return D3DTOP_BLENDDIFFUSEALPHA;
			case video::Texstageoperation_BlendWithTexturealpha:return D3DTOP_BLENDTEXTUREALPHA;
			case video::Texstageoperation_BlendWithFactoralpha:return D3DTOP_BLENDFACTORALPHA;
			case video::Texstageoperation_BlendWithCurrentalpha:return D3DTOP_BLENDCURRENTALPHA;
			case video::Texstageoperation_Dotproduct3:return D3DTOP_DOTPRODUCT3;
			default:return D3DTOP_FORCE_DWORD;
		}
	}

	DWORD d3dtexturearg(const video::Texstageargument iontexarg)
	{
		switch (iontexarg) {
			case video::Texstageargument_Constant:return D3DTA_CONSTANT;
			case video::Texstageargument_Current:return D3DTA_CURRENT;
			case video::Texstageargument_Diffuse:return D3DTA_DIFFUSE;
			case video::Texstageargument_Specular:return D3DTA_SPECULAR;
			case video::Texstageargument_Texture:return D3DTA_TEXTURE;
			case video::Texstageargument_Factor:return D3DTA_TFACTOR;
			default:return 0xFFFFFFFF;
		}
	}









	void tod3dmatrix(const base::Float4x4& srcmatrix,D3DMATRIX& destmatrix,const bool transpose)
	{
		if (transpose) {
			for (unsigned int j=0;j<4;++j) {
				for (unsigned int i=0;i<4;++i) {
					destmatrix.m[j][i]=srcmatrix.value(j,i);
				}
			}
		} else {
			for (unsigned int j=0;j<4;++j) {
				for (unsigned int i=0;i<4;++i) {
					destmatrix.m[j][i]=srcmatrix.value(i,j);
				}
			}
		}

	}

	void tod3dxmatrix(const base::Float4x4& srcmatrix,D3DXMATRIX& destmatrix,const bool transpose)
	{
		if (transpose) {
			for (unsigned int j=0;j<4;++j) {
				for (unsigned int i=0;i<4;++i) {
					destmatrix.m[j][i]=srcmatrix.value(j,i);
				}
			}
		} else {
			for (unsigned int j=0;j<4;++j) {
				for (unsigned int i=0;i<4;++i) {
					destmatrix.m[j][i]=srcmatrix.value(i,j);
				}
			}
		}
	}


}
}
