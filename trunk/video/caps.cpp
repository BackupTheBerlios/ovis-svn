#include "../base/log.hh"
#include "caps.hh"

namespace ovis {

	using namespace base;

	void video::Caps::dumpToLog() const
	{
#define YESNO(b) ((b) ? "yes" : "no")
#define ISSUPPORTED(b) ((b) ? "supported" : "unsupported")

		logstream() << "Video device capabilities:\n";
		logstream() << "  Non-power of two textures: " << ISSUPPORTED(m_NPOTTextures) << "\n";
		logstream() << "  Rectangular textures: " << ISSUPPORTED(m_RectangularTextures) << "\n";
		logstream() << "  Cubemaps: " << ISSUPPORTED(m_Cubemaps) << "\n";
		logstream() << "  Non-power of two cubemaps: " << ISSUPPORTED(m_NPOTCubemaps) << "\n";
		logstream() << "  Compressed textures: " << ISSUPPORTED(m_CompressedTextures) << "\n";
		logstream() << "  Projected textures: " << ISSUPPORTED(m_ProjectedTextures) << "\n";
		logstream() << "  Hardware occlusion queries: " << ISSUPPORTED(m_HWOcclusionQueries) << "\n";

		logstream() << "\n";

		logstream() << "  Max texture width: " << m_MaxTextureWidth << "\n";
		logstream() << "  Max texture height: " << m_MaxTextureHeight << "\n";
		logstream() << "  Max texture depth: " << m_MaxTextureVolumeDepth << "\n";
		logstream() << "  Max anisotropy: " << m_MaxAnisotropy << "\n";
		logstream() << "  Max texture blending stages: " << m_MaxTextureBlendStages << "\n";
		logstream() << "  Max active ffp lights: " << m_MaxActiveFFLights << "\n";
		logstream() << "  Max primitive count: " << m_MaxPrimitiveCount << "\n";
		logstream() << "  Max vertex index: " << m_MaxVertexIndex << "\n";
		logstream() << "  Max point size: " << m_MaxPointSize << "\n";

		logstream() << "\n";

		logstream() << "  Supported GPU program formats:\n";
		for (size_t i=0;i<m_SupportedProgramFormats.numStrings();++i)
			logstream() << "    " << (*m_SupportedProgramFormats.string(i)) << "\n";

		logstream() << "\nCapabilities dump complete.\n";
	}

}
