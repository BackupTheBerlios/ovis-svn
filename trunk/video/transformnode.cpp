#include <assert.h>
#include "transformnode.hh"

namespace ovis {
namespace video {

	void Transformnode::onVisit(Visitcontext& visitcontext)
	{
	}

	void Transformnode::spawn(base::Streamable& datasource)
	{
	}

	void Transformnode::serialize(base::Streamable& datasink) const
	{
	}

	const char* Transformnode::type() const
	{
		return "Transformnode";
	}

	bool Transformnode::isValid() const
	{
		return true;
	}

	Transformnode::Transformnode(const base::String& identifier):Scenenode(identifier),
	m_Position(0,0,0),m_Scale(1,1,1),m_Rotation(0,0,0,1),m_pParent(0),m_WMatrixValid(false),m_LocalWMatrixValid(false)
	{
		m_pParent=0;
	}

	Transformnode::Transformnode(const Transformnode &srcTransformnode):Scenenode(srcTransformnode),
	m_pParent(0),m_WMatrixValid(false),m_LocalWMatrixValid(false)
	{
		m_pParent=0;
		clone(srcTransformnode);
	}

	Transformnode::~Transformnode()
	{
	}

	void Transformnode::clone(const Transformnode &srcTransformnode)
	{
		Scenenode::clone(srcTransformnode);

		m_Position=srcTransformnode.position();
		m_Scale=srcTransformnode.scale();
		m_Rotation=srcTransformnode.rotation();
		m_pParent=srcTransformnode.parent();
		//m_pTransformnodeIData->m_Children=srcTransformnode.transformnodeIData().m_Children;
	}

	Transformnode& Transformnode::operator =(const Transformnode& src)
	{
		clone(src); return *this;
	}

	void Transformnode::parent(Transformnode *pParent)
	{
		m_pParent=pParent;
	}

	Transformnode* Transformnode::parent() const
	{
		return m_pParent;
	}

	bool Transformnode::worldmatrixIsValid() const
	{
		if (!m_WMatrixValid) return false;
		else {
			if (m_pParent)
				m_WMatrixValid=m_pParent->worldmatrixIsValid();

			return m_WMatrixValid;
		}
	}

	bool Transformnode::localWorldmatrixIsValid() const
	{
		return m_LocalWMatrixValid;
	}

	const math::Matrix4f& Transformnode::worldmatrix() const
	{
		bool b=m_WMatrixValid&&m_LocalWMatrixValid;

		if (b && (m_pParent!=0)) {
			b=(m_pParent->worldmatrixIsValid());
		}

		if (!b) {
			m_Worldmatrix=localWorldmatrix();
			if (m_pParent)
				m_Worldmatrix*=m_pParent->worldmatrix();

			m_WMatrixValid=true;
		}

		return m_Worldmatrix;
	}

	const math::Matrix4f& Transformnode::localWorldmatrix() const
	{
		if (!m_LocalWMatrixValid) {
			math::Matrix4f mrot,mpos,mscale;
			mrot.fromQuaternion(m_Rotation);
			mpos.translate(m_Position);
			mscale.scale(m_Scale);

			m_LocalWorldmatrix.loadIdentity();
			m_LocalWorldmatrix=(mscale*mrot*mpos);

			m_LocalWMatrixValid=true;
			m_WMatrixValid=false;
		}
		return m_LocalWorldmatrix;
	}

	const math::Vector3f& Transformnode::position() const
	{
		return m_Position;
	}

	void Transformnode::position(const base::Float3& newposition)
	{
		m_Position=newposition;
		m_LocalWMatrixValid=false;
	}

	void Transformnode::position(const float newx,const float newy,const float newz)
	{
		m_Position.x()=newx;
		m_Position.y()=newy;
		m_Position.z()=newz;
		m_LocalWMatrixValid=false;
	}

	const math::Quaternion& Transformnode::rotation() const
	{
		return m_Rotation;
	}

	void Transformnode::rotation(const base::Float4& newrotation)
	{
		m_Rotation=newrotation;
		m_LocalWMatrixValid=false;
	}

	void Transformnode::rotation(const float newx,const float newy,const float newz,const float neww)
	{
		m_Rotation.x()=newx;
		m_Rotation.y()=newy;
		m_Rotation.z()=newz;
		m_Rotation.w()=neww;
		m_LocalWMatrixValid=false;
	}

	const math::Vector3f& Transformnode::scale() const
	{
		return m_Scale;
	}

	void Transformnode::scale(const base::Float3& newscale)
	{
		m_Scale=newscale;
		m_LocalWMatrixValid=false;
	}

	void Transformnode::scale(const float newx,const float newy,const float newz)
	{
		m_Scale.x()=newx;
		m_Scale.y()=newy;
		m_Scale.z()=newz;
		m_LocalWMatrixValid=false;
	}

	void Transformnode::transformByMatrix(const math::Matrix4f& matrix)
	{
		// TODO: Check if the translation is extracted correctly & extract scale
		math::Quaternion q;
		q.fromMatrix(matrix);
		rotation(q);
		position(math::Vector3f(matrix.value(0,3),matrix.value(1,3),matrix.value(2,3)));
	}

}
}
