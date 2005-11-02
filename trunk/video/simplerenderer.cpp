#include "lightserver.hh"
#include "simplerenderer.hh"
#include "videodevice.hh"
#include "visitorqueue.hh"

namespace ovis {
namespace video {


	SimpleRenderer::SimpleRenderer(Videodevice& rVideodevice):
	Renderer(rVideodevice)
	{
	}

	SimpleRenderer::~SimpleRenderer()
	{
	}

	bool SimpleRenderer::supported() const { return true; }

	void SimpleRenderer::renderScene(const Camera& camera,Scenenode& scenenode)
	{
		m_rVideodevice.activateFFPipeline(FixedfunctionVertex);
		m_rVideodevice.activateFFPipeline(FixedfunctionFragment);

		Visitorqueue queue;

		Visitcontext vctx(camera,queue,*this);

		{
			std::set<Scenenode*> donenodes;
			traverseScene(donenodes,vctx,scenenode);
		}

		m_rVideodevice.zbuffer(true);
		m_rVideodevice.culling(true);
		//m_rVideodevice.drawingmode(Drawingmode_Lines);
		m_rVideodevice.cullmode(Cullingmode_Counterclockwise);

		bool hasLights=false;
		if (m_pLightserver) hasLights=(m_pLightserver->numLights()>0);

		if (hasLights) {
			m_rVideodevice.fixedSetMaterialDiffuseColor(base::Float4(1,1,1,1));
			m_rVideodevice.fixedLighting(true);

			Lightlist lightlist;

			std::vector< ScenenodeVisit* >::const_iterator it=queue.visits().begin();
			for (;it!=queue.visits().end();++it) {
				ScenenodeVisit *pVisit=(*it);
				Scenenode& node=pVisit->scenenode();

				lightlist.clearLights();
				m_pLightserver->nearestLights(node,m_rVideodevice.caps().m_MaxActiveFFLights,lightlist);

				for (unsigned int lightnr=0;lightnr<lightlist.numLights();++lightnr) {
					Light& light=lightlist.light(lightnr);

					FFLightProperties fflight;

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

					m_rVideodevice.fixedSetLightEnabled(lightnr,true);
					m_rVideodevice.fixedSetLightProperties(lightnr,fflight);
				}

				Texture2D* pTex;
				bool b=pVisit->fetch2DTexture("diffuseTexture",pTex);
				m_rVideodevice.fixedSet2DTexture(0,b ? pTex : 0);

				m_rVideodevice.fixedSetWorldMatrix(pVisit->worldmatrix());
				m_rVideodevice.fixedSetViewMatrix(camera.viewmatrix());
				m_rVideodevice.fixedSetProjectionMatrix(camera.projmatrix());

				if (pVisit->indexstream()) {
					m_rVideodevice.drawIndexedPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						*(pVisit->indexstream()),
						pVisit->callParams().m_IndexOffset,
						pVisit->callParams().m_NumElements);
				} else {
					m_rVideodevice.drawPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						pVisit->callParams().m_FirstElement,
						pVisit->callParams().m_NumElements);
				}
			}

		} else { // no lights

			m_rVideodevice.fixedLighting(false);

			std::vector< ScenenodeVisit* >::const_iterator it=queue.visits().begin();
			for (;it!=queue.visits().end();++it) {
				ScenenodeVisit *pVisit=(*it);
				Scenenode& node=pVisit->scenenode();

				Texture2D* pTex;
				bool b=pVisit->fetch2DTexture("diffuseTexture",pTex);
				m_rVideodevice.fixedSet2DTexture(0,b ? pTex : 0);

				m_rVideodevice.fixedSetWorldMatrix(pVisit->worldmatrix());
				m_rVideodevice.fixedSetViewMatrix(camera.viewmatrix());
				m_rVideodevice.fixedSetProjectionMatrix(camera.projmatrix());

				if (pVisit->indexstream()) {
					m_rVideodevice.drawIndexedPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						*(pVisit->indexstream()),
						pVisit->callParams().m_IndexOffset,
						pVisit->callParams().m_NumElements);
				} else {
					m_rVideodevice.drawPrimitives(
						pVisit->callParams().m_PrimitivesType,
						*(pVisit->vertexstream()),
						pVisit->callParams().m_FirstElement,
						pVisit->callParams().m_NumElements);
				}
			}

		}
	}

	void SimpleRenderer::checkResources()
	{
	}

	void SimpleRenderer::traverseScene(std::set<Scenenode*> &donenodes,Visitcontext& vctx,Scenenode& scenenode)
	{
		scenenode.onVisit(vctx);

		for (unsigned long i=0;i<scenenode.numOutputNodes();++i) {
			Scenenode *pNode=scenenode.outputnode(i);
			std::set<Scenenode*>::iterator it=donenodes.find(pNode);
			if (it==donenodes.end())
				traverseScene(donenodes,vctx,*pNode);
		}
	}


}
}
