#ifndef OVIS_VIDEO_LIGHT_HH_INCLUDED
#define OVIS_VIDEO_LIGHT_HH_INCLUDED

#include "../base/numericarrays.hh"
#include "../math/vectors.hh"
#include "coreenums.hh"
#include "transformnode.hh"

namespace ovis {
namespace video {

	class Cubemap;
	class Texture2D;

	class OVIS_API Light:public Transformnode
	{
	public:
		Light(const base::String& identifier);
		virtual ~Light() {}

		void spawn(base::Streamable& datasource);
		void serialize(base::Streamable& datasink) const;

		bool isValid() const { return true; } // TODO:

		void onVisit(const Camera& camera,Visitorqueue& visitorqueue) {}

		inline const char *type() const { return "Light"; }

		inline Lighttype lighttype() const { return m_Lighttype; }
		inline const math::Vector3f& direction() const { return m_Direction; }
		inline const base::Float3& diffuse() const { return m_Diffuse; }
		inline const base::Float3& specular() const { return m_Specular; }
		inline float range() const { return m_Range; }
		inline float falloff() const { return m_Falloff; }
		inline float constantAttenuation() const { return m_ConstantAttenuation; }
		inline float linearAttenuation() const { return m_LinearAttenuation; }
		inline float quadraticAttenuation() const { return m_QuadraticAttenuation; }
		inline bool enabled() const { return m_Enabled; }

		inline void lighttype(const Lighttype ltype) { m_Lighttype=ltype; }
		inline void direction(const math::Vector3f& dir) { m_Direction=dir; }
		inline void diffuse(const base::Float3& col) { m_Diffuse=col; }
		inline void specular(const base::Float3& col) { m_Specular=col; }
		inline void range(const float newrange) { m_Range=newrange; }
		inline void falloff(const float newfalloff) { m_Falloff=newfalloff; }
		inline void constantAttenuation(const float constatt) { m_ConstantAttenuation=constatt; }
		inline void linearAttenuation(const float linearatt) { m_LinearAttenuation=linearatt; }
		inline void quadraticAttenuation(const float quadratt) { m_QuadraticAttenuation=quadratt; }
		inline void enabled(const bool state) { m_Enabled=state; }

	protected:
		Lighttype m_Lighttype;
		mutable math::Vector3f m_Direction;
		base::Float3 m_Diffuse,m_Specular;
		float m_Range,m_Falloff;
		float m_ConstantAttenuation;
		float m_LinearAttenuation;
		float m_QuadraticAttenuation;
		bool m_Enabled;

	};

}
}

#endif
