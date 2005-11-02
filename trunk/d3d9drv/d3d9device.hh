#ifndef OVIS_D3D9DRV_D3D9DEVICE_HH_INCLUDED
#define OVIS_D3D9DRV_D3D9DEVICE_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#include "../base/dll.hh"
#include "../video/videodevice.hh"

namespace ovis {
namespace d3d9drv {

	class OVIS_API D3D9Device:public video::Videodevice
	{
	public:
		D3D9Device();
		~D3D9Device();

		bool isValid() const;


		//*********** INITIALIZATION / ENUMERATION

		const video::Caps& caps() const;
		ovis_uint32 numVideoadapters() const;
		ovis_uint32 numVideomodes(const ovis_uint32 adapterindex,const video::Pixelformat pixelformat) const;
		bool fetchVideomode(const ovis_uint32 adapter,const ovis_uint32 modeindex,
			video::Videomode& mode,const video::Pixelformat pixelformat) const;

		void init(base::Window* pWindow,const ovis_uint32 adapter,
			const video::Pixelformat colorbufferformat,
			const video::Pixelformat depthstencilformat,const ovis_uint32 Hz);

		bool testVertexProgram(const base::String& program,const base::String& format);
		bool testFragmentProgram(const base::String& program,const base::String& format);

		ovis_uint32 displaywidth() const;
		ovis_uint32 displayheight() const;

		video::Pixelformat colorbufferFormat() const;
		video::Pixelformat depthstencilbufferFormat() const;





		//*********** FRAME START / END / UPDATE
		void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ovis_uint32 stencilvalue=0);

		void begin();
		void end();

		void updateScreen();

		bool resizeViewport(const ovis_uint32 newx,const ovis_uint32 newy,
			const ovis_uint32 newwidth,const ovis_uint32 newheight,video::ViewportResizingCallback* callback);


		inline LPDIRECT3D9 d3d9interface() const { return m_pD3D9; }
		inline LPDIRECT3DDEVICE9 d3d9deviceinterface() const { return m_pD3DDev9; }
		inline const D3DCAPS9& d3d9caps() const { return m_D3DCaps; }




		//*********** RESOURCE GENERATION

		video::Vertexstream* createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,
			const ovis_uint32 flags,const video::Memorypool pool);
		video::Indexstream* createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,
			const ovis_uint32 flags,const video::Memorypool pool);

		video::Texture2D* create2DTexture(const base::String& identifier,
			const ovis_uint32 width,const ovis_uint32 height,const ovis_uint32 levels,
			const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);

		video::Cubemap* createCubemap(const base::String& identifier,
			const ovis_uint32 edgelength,const ovis_uint32 levels,
			const ovis_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);

		video::Occlusionquery* createOcclusionquery();

		video::Program* createVertexProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);
		video::Program* createFragmentProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);





		//*********** PRIMITIVES DRAWING

		void drawPrimitives(const video::PrimitivesType type,
			video::Vertexstream& vertexstream,const ovis_uint32 firstElement,
			const ovis_uint32 numElements);
		void drawIndexedPrimitives(
			const video::PrimitivesType type,
			video::Vertexstream& vertexstream,
			video::Indexstream& indexstream,
			const ovis_uint32 indexOffset,
			const ovis_uint32 numElements);




		//*********** STATE SETTING

		// hw blending. default: false
		void blending(const bool state);
		// handles zbuffer (both test & writing). default: true
		void zbuffer(const bool state);
		// z-writing. default: true
		void zwrite(const bool state);
		// handles stencil buffer (both test & writing). default: false
		void stencilbuffer(const bool state);
		// stencil writing. default: 0xFFFFFFFF
		void stencilwrite(const ovis_uint32 stencilmask);
		// backface culling. default: true
		void culling(const bool state);
		void cullmode(const video::Cullingmode mode);
		// blending function. default: one,zero
		void blendfunc(const video::BlendFunc srcblend,const video::BlendFunc destblend);
		// z test function. default: lessequal
		void zfunc(const video::CompareFunc func);
		// z test function. default: lessequal,0,0
		void stencilfunc(const video::CompareFunc func,const ovis_uint32 ref,const ovis_uint32 writemask);
		// stencil operations. default: zero,zero,zero
		void stencilop(const video::StencilOp opfail,const video::StencilOp opzfail,const video::StencilOp oppass);
		// geometry drawing mode. default: faces
		void drawingmode(const video::Drawingmode mode);
		// shading of filled triangles. default: smooth
		void solidshading(const video::Solidshading type);
		// depth bias. default: 0,0 (disables depth bias)
		void depthbias(const float scaledbias,const float uniformbias);

		// ************* Sample stage parameters

		unsigned long samplerstageParameterInt(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam) const;
		float samplerstageParameterFloat(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam) const;
		void samplerstageParameterInt(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam,const unsigned long value);
		void samplerstageParameterFloat(const ovis_uint32 samplerstage,const video::Samplerstageparameter sampparam,const float value);

		// ************* Texture stage parameters

		unsigned long texstageParameterInt(const ovis_uint32 texstage,const video::Texstageparameter texparam) const;
		float texstageParameterFloat(const ovis_uint32 texstage,const video::Texstageparameter texparam) const;
		void texstageParameterInt(const ovis_uint32 texstage,const video::Texstageparameter texparam,const unsigned long value);
		void texstageParameterFloat(const ovis_uint32 texstage,const video::Texstageparameter texparam,const float value);

		// ************* Vertex/fragment program parameters

		void vprogramSetFloat2(const base::String& varname,const base::Float2&);
		void vprogramSetFloat3(const base::String& varname,const base::Float3&);
		void vprogramSetFloat4(const base::String& varname,const base::Float4&);
		void vprogramSetFloat3x3(const base::String& varname,const base::Float3x3&);
		void vprogramSetFloat4x3(const base::String& varname,const base::Float4x3&);
		void vprogramSetFloat3x4(const base::String& varname,const base::Float3x4&);
		void vprogramSetFloat4x4(const base::String& varname,const base::Float4x4&);

		void fprogramSetFloat2(const base::String& varname,const base::Float2&);
		void fprogramSetFloat3(const base::String& varname,const base::Float3&);
		void fprogramSetFloat4(const base::String& varname,const base::Float4&);
		void fprogramSetFloat3x3(const base::String& varname,const base::Float3x3&);
		void fprogramSetFloat4x3(const base::String& varname,const base::Float4x3&);
		void fprogramSetFloat3x4(const base::String& varname,const base::Float3x4&);
		void fprogramSetFloat4x4(const base::String& varname,const base::Float4x4&);
		void fprogramBind2DTexture(const base::String& varname,const video::Texture2D* texVar);
		void fprogramBindCubemap(const base::String& varname,const video::Cubemap* texVar);

		video::Program* vertexprogram();
		video::Program* fragmentprogram();

		void vertexprogram(video::Program* pVertexprogram);
		void fragmentprogram(video::Program* pFragmentprogram);

		// ************* Fixed function pipeline support

		void activateFFPipeline(const video::Fixedfunctionactivationtype pipeline);

		void fixedSetMaterialAmbientColor(const base::Float4& color);
		void fixedSetMaterialDiffuseColor(const base::Float4& color);
		void fixedSetMaterialSpecularColor(const base::Float4& color);
		void fixedSetMaterialEmissiveColor(const base::Float4& color);
		void fixedSetMaterialShininess(const float shininess);
		void fixedSet2DTexture(const ovis_uint32 texstage,const video::Texture2D* texVar);
		ovis_uint32 fixedMaxTextures();
		ovis_uint32 fixedMaxTextureStages();
		void fixedLighting(const bool state);
		void fixedSetLightEnabled(const ovis_uint32 lightNr,const bool state);
		void fixedSetLightProperties(const ovis_uint32 lightNr,const video::FFLightProperties& properties);
		void fixedSetProjectionMatrix(const base::Float4x4& matrix);
		void fixedSetWorldMatrix(const base::Float4x4& matrix);
		void fixedSetViewMatrix(const base::Float4x4& matrix);
		ovis_uint32 fixedMaxLights() const;





	protected:
		LPDIRECT3D9 m_pD3D9;
		LPDIRECT3DDEVICE9 m_pD3DDev9;
		D3DCAPS9 m_D3DCaps;
		D3DMATERIAL9 m_D3D9FFMaterial;
		D3DLIGHT9 m_D3D9FFLight;
		video::Caps m_Caps;

		video::Program *m_pCurrentVertexProgram;
		video::Program *m_pCurrentFragmentProgram;

		DWORD m_Cullmode;
		DWORD *m_Texcoordindexmodes,*m_Texcoordgenmodes;
		bool m_CullingEnabled,m_IsValid;

		video::Pixelformat m_ColorbufferFormat,m_DepthstencilbufferFormat;
		ovis_uint32 m_Displaywidth,m_Displayheight;

		struct Resources;
		Resources *m_pResources;

		D3DPRESENT_PARAMETERS m_D3DPP;
	};

}
}

#endif
