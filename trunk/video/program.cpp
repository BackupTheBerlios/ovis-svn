#include "program.hh"
#include "videodevice.hh"

namespace ovis {
namespace video {

	Program::Program(Videodevice &rVideodevice,const base::String& identifier,const base::String& entrypoint,
		const base::String& format):ManagedMemObj(identifier.cstr()),m_Identifier(identifier),
		m_Entrypoint(entrypoint),m_Format(format),
	m_rVideodevice(rVideodevice) {}
	
	Program::~Program()
	{
		//m_rVideodevice.unregisterProgram(this,false);
	}

}
}
