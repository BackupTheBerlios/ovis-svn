#ifndef OVIS_VIDEO_TRANSFORMNODE_HH_INCLUDED
#define OVIS_VIDEO_TRANSFORMNODE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/managedmemobj.hh"
#include "../base/refcounted.hh"
#include "../base/streamable.hh"

#include "../math/matrix.hh"
#include "../math/quaternion.hh"
#include "../math/vectors.hh"

#include "scenenode.hh"

#include <deque>
#include <vector>

namespace ovis {
namespace video {

	class OVIS_API Transformnode:public Scenenode
	{
	public:
		virtual void onVisit(Visitcontext& visitcontext);

		virtual void spawn(base::Streamable& datasource);
		virtual void serialize(base::Streamable& datasink) const;

		virtual const char *type() const;

		bool isValid() const;

		Transformnode(const base::String& identifier);
		Transformnode(const Transformnode &srcTransformnode);
		virtual ~Transformnode();

		virtual void clone(const Transformnode &srcTransformnode);
		virtual Transformnode& operator =(const Transformnode& src);

		void parent(Transformnode *pParent);
		Transformnode* parent() const;

		virtual const math::Matrix4f& worldmatrix() const;
		virtual const math::Matrix4f& localWorldmatrix() const;

		bool worldmatrixIsValid() const;
		bool localWorldmatrixIsValid() const;

		const math::Vector3f& position() const;
		void position(const base::Float3& newposition);
		void position(const float newx,const float newy,const float newz);

		const math::Quaternion& rotation() const;
		void rotation(const base::Float4& newrotation);
		void rotation(const float newx,const float newy,const float newz,const float neww);

		const math::Vector3f& scale() const;
		void scale(const base::Float3& newscale);
		void scale(const float newx,const float newy,const float newz);

		void transformByMatrix(const math::Matrix4f& matrix);
	protected:

		math::Vector3f m_Position,m_Scale;
		math::Quaternion m_Rotation;
		Transformnode* m_pParent;

		mutable math::Matrix4f m_Worldmatrix,m_LocalWorldmatrix;
		mutable bool m_WMatrixValid,m_LocalWMatrixValid;
	};

}
}

#endif
