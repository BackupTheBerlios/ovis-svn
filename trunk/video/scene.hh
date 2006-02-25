#ifndef OVIS_VIDEO_SCENE_HH_INCLUDED
#define OVIS_VIDEO_SCENE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/basememobj.hh"

#include "attributebuffer.hh"
#include "attributetable.hh"
#include "camera.hh"
#include "colorscale.hh"
#include "material.hh"
#include "memindexstream.hh"
#include "memvertexstream.hh"
#include "renderer.hh"

namespace ovis {
namespace video {

	const ovis_uint32 MATERIAL_VERTEXCOLOR=0xFFFFFFFF;

	//! Scene container.
	/**
	* Usage:
	* - Create the Scene instance and supply a valid renderer reference
	* - Call allocGeometryStreams() to initialize space for the memory streams and the attribute buffer
	* - Fill the memstreams using the non-const memIndexstream() and memVertexstream() accessors
	*   (That is, call memIndexstream().index() for example)
	* - Fill the attribute buffer using the non-const attributebuffer() accessor
	* - Call initGeometrystreams(); this copies the contents of the memstreams to the renderer streams (and creates those),
	*   and initializes the attribute table out of the attribute buffer
	*
	* \note 
	* If you call renderer() to set a new renderer, the renderer streams will be erased. The same applies to an 
	* allocGeometryStreams() call (it erases the renderer streams too). In these cases, you don't have to call
	* initGeometrystreams() manually (the render() call takes care of this if needed).
	* However, if geometry data in the memstrams is changed, you HAVE to call it manually to copy the new
	* contents to the renderer streams.
	* Use the attribute MATERIAL_VERTEXCOLOR in the attribute buffer to mark areas to be rendered using vertex colors.
	* To include factors, use a 1D texcoord (VertexFormatEntry_Texcoord1D) with the VertexFormatSemantic_Factor semantic.
	*/

	class OVIS_API Scene:public base::BaseMemObj
	{
	public:
		Scene(Renderer *pRenderer);
		~Scene();

		void allocGeometryStreams(const Vertexformat& format,const ovis_uint32 numVertices,const ovis_uint32 numIndices);

		const MemIndexstream& memIndexstream() const;
		MemIndexstream& memIndexstream();

		const MemVertexstream& memVertexstream() const;
		MemVertexstream& memVertexstream();

		const Attributebuffer& attributebuffer() const;
		Attributebuffer& attributebuffer();

		const Attributetable& attributetable() const;
		Attributetable& attributetable();

		const Indexstream* indexstream() const;
		Indexstream* indexstream();

		const Vertexstream* vertexstream() const;
		Vertexstream* vertexstream();

		const Renderer& renderer() const;
		void renderer(Renderer &rRenderer);

		const Camera* camera() const;
		void camera(Camera &rCamera);

		const Colorscale* colorscale() const;
		void colorscale(Colorscale &rColorscale);

		void addMaterial(const Material& m);
		void removeMaterial(const Material* m);
		unsigned long numMaterials() const;
		Material* material(const unsigned long index);
		const Material* material(const unsigned long index) const;


		bool isValid() const;

		//! Renders the scene.
		/**
		* Internally, this calls m_pRenderer->render(*this). However, it is recommended to use this method,
		* since the index/vertexstreams depend on the renderer. Using this method ensures that
		* the correct renderer is called. It also makes index/vertexstream changes due to a renderer change
		* transparent. See above for details.
		*/
		void render();

		void initGeometrystreams();
	protected:
		MemIndexstream *m_pMemIndexstream;
		MemVertexstream *m_pMemVertexstream;
		Indexstream *m_pIndexstream;
		Vertexstream *m_pVertexstream;
		Renderer *m_pRenderer;		
		Camera *m_pCamera;
		Colorscale *m_pColorscale;
		Attributebuffer m_Attributebuffer;
		Attributetable m_Attributetable;

		struct Materiallist;
		Materiallist *m_pMateriallist;
	};

}
}

#endif
