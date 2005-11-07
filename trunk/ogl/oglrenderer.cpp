#include "oglrenderer.hh"
#include "../video/lightserver.hh"
#include "../video/videodevice.hh"
#include "../video/visitorqueue.hh"

namespace ovis {
namespace opengldrv {


	OGLRenderer::OGLRenderer()
	{
		m_pOGLDevice=new OGLDevice;
	}

	OGLRenderer::~OGLRenderer()
	{
		delete m_pOGLDevice;
	}

	bool OGLRenderer::supported() const { return true; }
	
	void OGLRenderer::init(const ovis_uint32 width,const ovis_uint32 height,
			const video::Pixelformat colorbufferformat,const video::Pixelformat depthstencilformat,const ovis_uint32 Hz)
	{
		m_pOGLDevice->init(width,height,colorbufferformat,depthstencilformat,Hz);
	}
	
	void OGLRenderer::clear(const ovis_uint32 buffers,const base::Float4& color,const float zvalue,
			const ovis_uint32 stencilvalue)
	{
		m_pOGLDevice->clear(buffers,color,zvalue,stencilvalue);
	}
	
	video::Vertexstream* OGLRenderer::createVertexstream(const ovis_uint32 numVertices,const video::Vertexformat& format,
		const ovis_uint32 flags)
	{
		return m_pOGLDevice->createVertexstream(numVertices,format,flags,video::Mempool_Managed);
	}
		
	video::Indexstream* OGLRenderer::createIndexstream(const ovis_uint32 numIndices,const video::Indexformat format,
		const ovis_uint32 flags)
	{
		return m_pOGLDevice->createIndexstream(numIndices,format,flags,video::Mempool_Managed);
	}

	video::Texture2D* OGLRenderer::create2DTexture(const base::String& identifier,const base::String& texturesource,
		const ovis_uint32 width,const ovis_uint32 height,const ovis_uint32 levels,
		const ovis_uint32 flags,const video::Pixelformat format)
	{
		return m_pOGLDevice->create2DTexture(identifier,texturesource,width,height,levels,flags,format,video::Mempool_Managed);
	}

	video::Cubemap* OGLRenderer::createCubemap(const base::String& identifier,const base::String& texturesource,
		const ovis_uint32 edgelength,const ovis_uint32 levels,
		const ovis_uint32 flags,const video::Pixelformat format)
	{
		return m_pOGLDevice->createCubemap(identifier,texturesource,edgelength,levels,flags,format,video::Mempool_Managed);
	}

	void OGLRenderer::renderScene(const video::Camera& camera,video::Scenenode& scenenode)
	{
		// No device initialized -> bail out
		if (!m_pOGLDevice) return;

		// Deactivate shaders
		m_pOGLDevice->activateFFPipeline(video::FixedfunctionVertex);
		m_pOGLDevice->activateFFPipeline(video::FixedfunctionFragment);

		// Set up the visitor queue and visit the entire scenegraph
		video::Visitorqueue queue;

		video::Visitcontext vctx(camera,queue,*this);

		{
			std::set<video::Scenenode*> donenodes;
			traverseScene(donenodes,vctx,scenenode);
		}

		// Set up some states
		m_pOGLDevice->zbuffer(true);
		m_pOGLDevice->culling(true);
		m_pOGLDevice->cullmode(video::Cullingmode_Counterclockwise);

		// Find out if some lights are present
		bool hasLights=false;
		if (m_pLightserver) hasLights=(m_pLightserver->numLights()>0);

		
		if (hasLights) {
			// Switch on fixed lighting
			// TODO: change the hardcoded material diffuse color if some scenenode visits returned a diffuse color
			m_pOGLDevice->fixedSetMaterialDiffuseColor(base::Float4(1,1,1,1));
			m_pOGLDevice->fixedLighting(true);

			// This will contain the lights to be rendered for each visit
			video::Lightlist lightlist;

			std::vector< video::ScenenodeVisit* >::const_iterator it=queue.visits().begin();
			for (;it!=queue.visits().end();++it) {
				video::ScenenodeVisit *pVisit=(*it);
				video::Scenenode& node=pVisit->scenenode();

				// Clear the light list, find the lights nearest to the node visited, and store them in the list
				lightlist.clearLights();
				m_pLightserver->nearestLights(node,m_pOGLDevice->caps().m_MaxActiveFFLights,lightlist);

				// Set the light properties
				for (unsigned int lightnr=0;lightnr<lightlist.numLights();++lightnr) {
					video::Light& light=lightlist.light(lightnr);

					video::FFLightProperties fflight;

					fflight.m_Lighttype=light.lighttype();
					fflight.m_Range=light.range();
					fflight.m_Color[0]=1;
					fflight.m_Color[1]=light.diffuse()[0];
					fflight.m_Color[2]=light.diffuse()[1];
					fflight.m_Color[3]=light.diffuse()[2];
					fflight.m_Direction=light.direction();
					fflight.m_Position=light.position();
					fflight.m_Falloff=light.falloff();
					fflight.m_ConstantAttenuation=light.constantAttenuation();
					fflight.m_LinearAttenuation=light.linearAttenuation();
					fflight.m_QuadraticAttenuation=light.quadraticAttenuation();

					m_pOGLDevice->fixedSetLightEnabled(lightnr,true);
					m_pOGLDevice->fixedSetLightProperties(lightnr,fflight);
				}

				// Set the texture, if available
				video::Texture2D* pTex;
				bool b=pVisit->fetch2DTexture("diffuseTexture",pTex);
				m_pOGLDevice->fixedSet2DTexture(0,b ? pTex : 0);

				// Set transformation matrices
				m_pOGLDevice->fixedSetWorldMatrix(pVisit->worldmatrix());
				m_pOGLDevice->fixedSetViewMatrix(camera.viewmatrix());
				m_pOGLDevice->fixedSetProjectionMatrix(camera.projmatrix());

				// Draw with indexed primitives if an indexstream is present, simple primitives otherwise
				if (pVisit->indexstream()) {
					m_pOGLDevice->drawIndexedPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						*(pVisit->indexstream()),
						pVisit->callParams().m_IndexOffset,
						pVisit->callParams().m_NumElements);
				} else {
					m_pOGLDevice->drawPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						pVisit->callParams().m_FirstElement,
						pVisit->callParams().m_NumElements);
				}
			}

		} else { // no lights

			m_pOGLDevice->fixedLighting(false);

			std::vector< video::ScenenodeVisit* >::const_iterator it=queue.visits().begin();
			for (;it!=queue.visits().end();++it) {
				video::ScenenodeVisit *pVisit=(*it);
				//video::Scenenode& node=pVisit->scenenode();

				video::Texture2D* pTex;
				bool b=pVisit->fetch2DTexture("diffuseTexture",pTex);
				m_pOGLDevice->fixedSet2DTexture(0,b ? pTex : 0);

				m_pOGLDevice->fixedSetWorldMatrix(pVisit->worldmatrix());
				m_pOGLDevice->fixedSetViewMatrix(camera.viewmatrix());
				m_pOGLDevice->fixedSetProjectionMatrix(camera.projmatrix());

				if (pVisit->indexstream()) {
					m_pOGLDevice->drawIndexedPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						*(pVisit->indexstream()),
						pVisit->callParams().m_IndexOffset,
						pVisit->callParams().m_NumElements);
				} else {
					m_pOGLDevice->drawPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						pVisit->callParams().m_FirstElement,
						pVisit->callParams().m_NumElements);
				}
			}

		}
	}

	void OGLRenderer::traverseScene(std::set<video::Scenenode*> &donenodes,video::Visitcontext& vctx,video::Scenenode& scenenode)
	{
		scenenode.onVisit(vctx);

		for (unsigned long i=0;i<scenenode.numOutputNodes();++i) {
			video::Scenenode *pNode=scenenode.outputnode(i);
			std::set<video::Scenenode*>::iterator it=donenodes.find(pNode);
			if (it==donenodes.end())
				traverseScene(donenodes,vctx,*pNode);
		}
	}

	bool OGLRenderer::isValid() const
	{
		return m_pOGLDevice->isValid();
	}


}
}

extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
ovis::video::Renderer* createRendererInstance()
{
	return new ovis::opengldrv::OGLRenderer;
}


extern "C"
#ifdef WIN32
__declspec( dllexport )
#endif
const char* rendererDescription()
{
	return "OpenGL renderer v0.5 build 2005-11-07 19:48";
}
