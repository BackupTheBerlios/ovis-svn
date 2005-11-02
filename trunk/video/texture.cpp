#include "texture.hh"

namespace ovis {
namespace video {

	Pixelformat Texture::requestedpixelformat() const { return m_RequestedPixelformat; }

	Texture::Texture(const Pixelformat requestedpixelformat,const base::String& identifier,const base::String& texturesource):
	RefCounted(identifier.cstr()),m_RequestedPixelformat(requestedpixelformat),m_Texturesource(texturesource) {}
	
	const base::String& Texture::texturesource() const
	{
		return m_Texturesource;
	}
}
}
