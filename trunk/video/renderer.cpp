#include "renderer.hh"

namespace ovis {
namespace video {

	Renderer::Renderer():m_pLightserver(0) {}
	Renderer::~Renderer() {}

	void Renderer::lightserver(Lightserver& lserver) { m_pLightserver=&lserver; }

}
}
