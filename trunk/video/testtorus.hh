#ifndef OVIS_VIDEO_TESTTORUS_HH_INCLUDED
#define OVIS_VIDEO_TESTTORUS_HH_INCLUDED

#include "scenenode.hh"
#include "transformnode.hh"

namespace ovis {
namespace video {

	class Renderer;
	class Vertexstream;
	class Indexstream;
	class Texture2D;

	class OVIS_API Testtorus:public Transformnode
	{
	public:
		Testtorus(Renderer& renderer,const base::String& identifier,
			const float innerradius,const float outerradius,const unsigned long tesselation);
		~Testtorus();

		void onVisit(Visitcontext& visitcontext);
		bool isValid() const;

		void spawn(base::Streamable& datasource) {}
		void serialize(base::Streamable& datasink) const {}

		const char *type() const;

		void texture2d(Texture2D* pTex);

	protected:
		Indexstream *m_pIndexstream;
		Vertexstream *m_pVertexstream;
		Texture2D* m_pTexture2D;
	};

}
}

#endif
