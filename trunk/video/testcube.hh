#ifndef OVIS_VIDEO_TESTCUBE_HH_INCLUDED
#define OVIS_VIDEO_TESTCUBE_HH_INCLUDED

#include "scenenode.hh"
#include "transformnode.hh"

namespace ovis {
namespace video {

	class Videodevice;
	class Vertexstream;
	class Indexstream;
	class Texture2D;

	class OVIS_API Testcube:public Transformnode
	{
	public:
		Testcube(Videodevice& videodevice,const base::String& identifier,
			const float width,const float height,const float depth);
		~Testcube();

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
		Transformnode *m_pTransformnode;
	};

}
}

#endif
