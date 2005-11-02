#include "renderer.hh"

namespace ovis {
namespace video {

	Renderer::Renderer(Videodevice& rVideodevice):m_rVideodevice(rVideodevice),m_pLightserver(0) {}
	Renderer::~Renderer() {}

	void Renderer::lightserver(Lightserver& lserver) { m_pLightserver=&lserver; }

}
}
