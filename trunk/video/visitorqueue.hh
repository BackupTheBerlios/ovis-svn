#ifndef OVIS_VIDEO_VISITORQUEUE_HH_INCLUDED
#define OVIS_VIDEO_VISITORQUEUE_HH_INCLUDED

#include "../math/matrix.hh"
#include "coreenums.hh"
#include "propertytable.hh"
#include "indexstream.hh"
#include "vertexstream.hh"

#include <vector>

namespace ovis {
namespace video {

	class Scenenode;

	class ScenenodeVisit:public Propertytable
	{
	public:
		struct OVIS_API Callparams {
			PrimitivesType m_PrimitivesType;
			unsigned long m_FirstElement,m_NumElements,m_IndexOffset;
		};

		OVIS_API ScenenodeVisit(Scenenode &rScenenode);
		OVIS_API ScenenodeVisit(const ScenenodeVisit &src);

		OVIS_API inline ScenenodeVisit& operator =(const ScenenodeVisit &src) { clone(src); return *this; }

		OVIS_API void clone(const ScenenodeVisit &src);

		OVIS_API void vertexstream(Vertexstream* pStream);
		OVIS_API Vertexstream* vertexstream() const;

		OVIS_API void indexstream(Indexstream* pStream);
		OVIS_API Indexstream* indexstream() const;

		OVIS_API inline void scenenode(Scenenode& node) { m_pScenenode=&node; }
		OVIS_API inline Scenenode& scenenode() const { return *m_pScenenode; }

		OVIS_API inline Callparams& callParams() { return m_Callparams; }
		OVIS_API inline const Callparams& callParams() const { return m_Callparams; }

		OVIS_API inline const math::Matrix4f& worldmatrix() const { return m_Worldmatrix; }
		OVIS_API inline math::Matrix4f& worldmatrix() { return m_Worldmatrix; }
		OVIS_API inline void worldmatrix(const math::Matrix4f& m) { m_Worldmatrix=m; }
	protected:
		Scenenode *m_pScenenode;

		Vertexstream *m_pVertexstream;
		Indexstream *m_pIndexstream;
		Callparams m_Callparams;
		math::Matrix4f m_Worldmatrix;
	};

	class Visitorqueue
	{
	public:
		OVIS_API Visitorqueue();
		OVIS_API ~Visitorqueue();

		OVIS_API void addVisit(ScenenodeVisit* pVisit);

		inline const std::vector< ScenenodeVisit* >& visits() const { return m_Visits; }
	protected:
		std::vector< ScenenodeVisit* > m_Visits;
	};

}
}

#endif
