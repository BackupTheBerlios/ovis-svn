#ifndef OVIS_VIDEO_VIDEODEVICE_HH_INCLUDED
#define OVIS_VIDEO_VIDEODEVICE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/basememobj.hh"
#include "../base/numericarrays.hh"
#include "../base/itypes.hh"
#include "../base/string.hh"
#include "../base/window.hh"
#include "../math/matrix.hh"
#include "caps.hh"
#include "coreenums.hh"
#include "cubemap.hh"
#include "indexformat.hh"
#include "indexstream.hh"
#include "occlusionquery.hh"
#include "program.hh"
#include "texture2d.hh"
#include "vertexformat.hh"
#include "vertexstream.hh"

namespace ovis {
namespace video {

	struct OVIS_API Videomode {
		ovis_uint32 m_Width,m_Height,m_Hz;
		Pixelformat m_Pixelformat;
	};

	class ViewportResizingCallback
	{
	public:
		virtual void checkIonVideodeviceResources()=0;
		virtual ~ViewportResizingCallback() {}
	};

	struct OVIS_API FFLightProperties
	{
		Lighttype m_Lighttype;
		math::Vector3f m_Position,m_Direction;
		base::Float4 m_Color;
		float m_Range,m_Falloff,m_ConstantAttenuation,m_LinearAttenuation,m_QuadraticAttenuation;
	};

	class OVIS_API Videodevice:public base::BaseMemObj
	{
	public:
		~Videodevice();



		//*********** INITIALIZATION / ENUMERATION

		virtual const Caps& caps() const=0;
		virtual ovis_uint32 numVideoadapters() const=0;
		virtual ovis_uint32 numVideomodes(const ovis_uint32 adapterindex,const Pixelformat pixelformat) const=0;
		virtual bool fetchVideomode(const ovis_uint32 adapter,
			const ovis_uint32 modeindex,Videomode& mode,
			const Pixelformat pixelformat) const=0;

		virtual void init(const ovis_uint32 width,const ovis_uint32 height,
			const Pixelformat colorbufferformat,const Pixelformat depthstencilformat,
			const ovis_uint32 Hz)=0;

		virtual bool testVertexProgram(const base::String& program,const base::String& format)=0;
		virtual bool testFragmentProgram(const base::String& program,const base::String& format)=0;

		virtual ovis_uint32 displaywidth() const=0;
		virtual ovis_uint32 displayheight() const=0;

		virtual Pixelformat colorbufferFormat() const=0;
		virtual Pixelformat depthstencilbufferFormat() const=0;




		//*********** FRAME START / END / UPDATE
		virtual void clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue=1.0f,
			const ovis_uint32 stencilvalue=0)=0;

		virtual void begin()=0;
		virtual void end()=0;

		virtual void updateScreen()=0;

		virtual bool resizeViewport(const ovis_uint32 newx,const ovis_uint32 newy,
			const ovis_uint32 newwidth,const ovis_uint32 newheight,ViewportResizingCallback* callback)=0;



		//*********** RESOURCE GENERATION

		virtual Vertexstream* createVertexstream(const ovis_uint32 numVertices,const Vertexformat& format,
			const ovis_uint32 flags,const Memorypool pool)=0;
		virtual Indexstream* createIndexstream(const ovis_uint32 numIndices,const Indexformat format,
			const ovis_uint32 flags,const Memorypool pool)=0;

		virtual Texture2D* create2DTexture(const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 width,const ovis_uint32 height,const ovis_uint32 levels,
			const ovis_uint32 flags,const Pixelformat format,const Memorypool pool)=0;

		virtual Cubemap* createCubemap(const base::String& identifier,const base::String& texturesource,
			const ovis_uint32 edgelength,const ovis_uint32 levels,
			const ovis_uint32 flags,const Pixelformat format,const Memorypool pool)=0;

		virtual Occlusionquery* createOcclusionquery()=0;

		virtual Program* createVertexProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format)=0;
		virtual Program* createFragmentProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format)=0;





		//*********** PRIMITIVES DRAWING

		virtual void drawPrimitives(const PrimitivesType type,
			Vertexstream& vertexstream,const ovis_uint32 firstElement,
			const ovis_uint32 numElements)=0;
		virtual void drawIndexedPrimitives(
			const PrimitivesType type,
			Vertexstream& vertexstream,
			Indexstream& indexstream,
			const ovis_uint32 indexOffset,
			const ovis_uint32 numElements)=0;



		//*********** STATE SETTING/GETTING

		// hw blending. default: false
		virtual void blending(const bool state)=0;
		// handles zbuffer (both test & writing). default: true
		virtual void zbuffer(const bool state)=0;
		// z-writing. default: true
		virtual void zwrite(const bool state)=0;
		// handles stencil buffer (both test & writing). default: false
		virtual void stencilbuffer(const bool state)=0;
		// stencil writing. default: 0xFFFFFFFF
		virtual void stencilwrite(const ovis_uint32 stencilmask)=0;
		// backface culling. default: true
		virtual void culling(const bool state)=0;
		// culling mode. default: counter clockwise
		virtual void cullmode(const Cullingmode mode)=0;
		// blending function. default: one,zero
		virtual void blendfunc(const BlendFunc srcblend,const BlendFunc destblend)=0;
		// z test function. default: lessequal
		virtual void zfunc(const CompareFunc func)=0;
		// z test function. default: lessequal,0,0
		virtual void stencilfunc(const CompareFunc func,const ovis_uint32 ref,const ovis_uint32 writemask)=0;
		// stencil operations. default: zero,zero,zero
		virtual void stencilop(const StencilOp opfail,const StencilOp opzfail,const StencilOp oppass)=0;
		// geometry drawing mode. default: faces
		virtual void drawingmode(const Drawingmode mode)=0;
		// shading of filled triangles. default: smooth
		virtual void solidshading(const Solidshading type)=0;
		// depth bias. default: 0,0 (disables depth bias)
		virtual void depthbias(const float scaledbias,const float uniformbias)=0;

		// ************* Matrices

		const math::Matrix4f worldmatrix() const;
		void worldmatrix(const math::Matrix4f& newmatrix);

		const math::Matrix4f viewmatrix() const;
		void viewmatrix(const math::Matrix4f& newmatrix);

		const math::Matrix4f projmatrix() const;
		void projmatrix(const math::Matrix4f& newmatrix);

		math::Matrix4f worldView() const;
		math::Matrix4f viewProj() const;
		math::Matrix4f worldViewProj() const;
		math::Vector3f eyePosition() const;

		// ************* Sample stage parameters

		virtual unsigned long samplerstageParameterInt(const ovis_uint32 samplerstage,const Samplerstageparameter sampparam) const=0;
		virtual float samplerstageParameterFloat(const ovis_uint32 samplerstage,const Samplerstageparameter sampparam) const=0;
		virtual void samplerstageParameterInt(const ovis_uint32 samplerstage,const Samplerstageparameter sampparam,const unsigned long value)=0;
		virtual void samplerstageParameterFloat(const ovis_uint32 samplerstage,const Samplerstageparameter sampparam,const float value)=0;

		// ************* Texture stage parameters

		virtual unsigned long texstageParameterInt(const ovis_uint32 texstage,const Texstageparameter texparam) const=0;
		virtual float texstageParameterFloat(const ovis_uint32 texstage,const Texstageparameter texparam) const=0;
		virtual void texstageParameterInt(const ovis_uint32 texstage,const Texstageparameter texparam,const unsigned long value)=0;
		virtual void texstageParameterFloat(const ovis_uint32 texstage,const Texstageparameter texparam,const float value)=0;

		// ************* Vertex/fragment program parameters

		virtual void vprogramSetFloat2(const base::String& varname,const base::Float2&)=0;
		virtual void vprogramSetFloat3(const base::String& varname,const base::Float3&)=0;
		virtual void vprogramSetFloat4(const base::String& varname,const base::Float4&)=0;
		virtual void vprogramSetFloat3x3(const base::String& varname,const base::Float3x3&)=0;
		virtual void vprogramSetFloat4x3(const base::String& varname,const base::Float4x3&)=0;
		virtual void vprogramSetFloat3x4(const base::String& varname,const base::Float3x4&)=0;
		virtual void vprogramSetFloat4x4(const base::String& varname,const base::Float4x4&)=0;

		virtual void fprogramSetFloat2(const base::String& varname,const base::Float2&)=0;
		virtual void fprogramSetFloat3(const base::String& varname,const base::Float3&)=0;
		virtual void fprogramSetFloat4(const base::String& varname,const base::Float4&)=0;
		virtual void fprogramSetFloat3x3(const base::String& varname,const base::Float3x3&)=0;
		virtual void fprogramSetFloat4x3(const base::String& varname,const base::Float4x3&)=0;
		virtual void fprogramSetFloat3x4(const base::String& varname,const base::Float3x4&)=0;
		virtual void fprogramSetFloat4x4(const base::String& varname,const base::Float4x4&)=0;
		virtual void fprogramBind2DTexture(const base::String& varname,const Texture2D* texVar)=0;
		virtual void fprogramBindCubemap(const base::String& varname,const Cubemap* texVar)=0;

		// ************* Fixed function pipeline support

		virtual void activateFFPipeline(const Fixedfunctionactivationtype pipeline)=0;

		virtual void fixedSetMaterialAmbientColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialDiffuseColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialSpecularColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialEmissiveColor(const base::Float4& color)=0;
		virtual void fixedSetMaterialShininess(const float shininess)=0;
		virtual void fixedSet2DTexture(const ovis_uint32 texstage,const Texture2D* texVar)=0;
		virtual ovis_uint32 fixedMaxTextures()=0;
		virtual ovis_uint32 fixedMaxTextureStages()=0;
		virtual void fixedLighting(const bool state)=0;
		virtual void fixedSetLightEnabled(const ovis_uint32 lightNr,const bool state)=0;
		virtual void fixedSetLightProperties(const ovis_uint32 lightNr,const FFLightProperties& properties)=0;
		virtual void fixedSetProjectionMatrix(const base::Float4x4& matrix)=0;
		virtual void fixedSetWorldMatrix(const base::Float4x4& matrix)=0;
		virtual void fixedSetViewMatrix(const base::Float4x4& matrix)=0;
		virtual ovis_uint32 fixedMaxLights() const=0;



	protected:
		Videodevice();

		struct Renderdata;
		Renderdata *m_pInternalData;
	};

}
}

#endif
