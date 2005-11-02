#include "videodevice.hh"

namespace ovis {
namespace video {

	struct Videodevice::Renderdata
	{
		math::Vector3f m_Eyepos;
		math::Matrix4f m_Worldmatrix,m_Viewmatrix,m_Projmatrix,m_Worldview,m_Viewproj,m_Worldviewproj;
		bool m_UpdateEyeposition,m_UpdateWorldview,m_UpdateViewproj,m_UpdateWorldviewproj;

		Renderdata():m_UpdateEyeposition(true),m_UpdateWorldview(true),m_UpdateViewproj(true),
			m_UpdateWorldviewproj(true) {}
	};

	Videodevice::Videodevice():m_pInternalData(new Renderdata)
	{
	}

	Videodevice::~Videodevice()
	{
		delete m_pInternalData;
	}

	const math::Matrix4f Videodevice::worldmatrix() const
	{
		return m_pInternalData->m_Worldmatrix;
	}

	void Videodevice::worldmatrix(const math::Matrix4f& newmatrix)
	{
		m_pInternalData->m_Worldmatrix=newmatrix;
		m_pInternalData->m_UpdateWorldview=m_pInternalData->m_UpdateWorldviewproj=m_pInternalData->m_UpdateEyeposition=true;
	}


	const math::Matrix4f Videodevice::viewmatrix() const
	{
		return m_pInternalData->m_Viewmatrix;
	}

	void Videodevice::viewmatrix(const math::Matrix4f& newmatrix)
	{
		m_pInternalData->m_Viewmatrix=newmatrix;
		m_pInternalData->m_UpdateWorldview=m_pInternalData->m_UpdateWorldviewproj=m_pInternalData->m_UpdateEyeposition=m_pInternalData->m_UpdateViewproj=true;
	}


	const math::Matrix4f Videodevice::projmatrix() const
	{
		return m_pInternalData->m_Projmatrix;
	}

	void Videodevice::projmatrix(const math::Matrix4f& newmatrix)
	{
		m_pInternalData->m_Projmatrix=newmatrix;
		m_pInternalData->m_UpdateWorldviewproj=m_pInternalData->m_UpdateViewproj=true;
	}


	math::Matrix4f Videodevice::worldView() const
	{
		if (m_pInternalData->m_UpdateWorldview) {
			m_pInternalData->m_Worldview=m_pInternalData->m_Worldmatrix*m_pInternalData->m_Viewmatrix;
			m_pInternalData->m_UpdateWorldview=false;
			m_pInternalData->m_UpdateEyeposition=true;
		}

		return m_pInternalData->m_Worldview;
	}

	math::Matrix4f Videodevice::viewProj() const
	{
		if (m_pInternalData->m_UpdateViewproj) {
			m_pInternalData->m_Viewproj=m_pInternalData->m_Viewmatrix*m_pInternalData->m_Projmatrix;
			m_pInternalData->m_UpdateViewproj=false;
		}

		return m_pInternalData->m_Viewproj;
	}

	math::Matrix4f Videodevice::worldViewProj() const
	{
		if (m_pInternalData->m_UpdateWorldviewproj) {
			m_pInternalData->m_Worldviewproj=m_pInternalData->m_Worldmatrix*m_pInternalData->m_Viewmatrix*m_pInternalData->m_Projmatrix;
			m_pInternalData->m_UpdateWorldviewproj=false;
		}

		return m_pInternalData->m_Worldviewproj;
	}

	math::Vector3f Videodevice::eyePosition() const
	{
		/*Vector3f viewpos(0.0f,0.0f,0.0f);
		tempmatrix=(*itIjob)->worldmatrix()*(*m_pViewmatrix);

		viewpos=tempmatrix.invert()*viewpos;
		(*itautofloatvar)->m_Floats[0]=viewpos.x();
		(*itautofloatvar)->m_Floats[1]=viewpos.y();
		(*itautofloatvar)->m_Floats[2]=viewpos.z();*/

		if (m_pInternalData->m_UpdateEyeposition) {
			math::Vector3f viewpos(0.0f,0.0f,0.0f);
			math::Matrix4f m=(worldView());
			base::Float3 float3=m.invert()*viewpos;
			m_pInternalData->m_Eyepos.x()=float3[0];
			m_pInternalData->m_Eyepos.y()=float3[1];
			m_pInternalData->m_Eyepos.z()=float3[2];

			m_pInternalData->m_UpdateEyeposition=false;
		}

		return m_pInternalData->m_Eyepos;
	}

}
}
