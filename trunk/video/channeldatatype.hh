#ifndef OVIS_VIDEO_CHANNELDATATYPE_HH_INCLUDED
#define OVIS_VIDEO_CHANNELDATATYPE_HH_INCLUDED

#include "../base/dll.hh"

namespace ovis {
namespace video {

	enum Channeldatatype {
		Channeldatatype_Int8,
		Channeldatatype_Uint8,
		Channeldatatype_Uint16,
		Channeldatatype_Int16,
		Channeldatatype_Float16,
		Channeldatatype_Float32
	};

	OVIS_API unsigned int channeldatatypeSizeLookup(const Channeldatatype channeldatatype);
	OVIS_API const char *channeldatatypeShortName(const Channeldatatype channeldatatype);
	OVIS_API const char *channeldatatypeLongName(const Channeldatatype channeldatatype);

	/*class OVIS_API Channeldatatype {
	public:
		Channeldatatype(const unsigned long size,const bool _signed,const bool _integer):
		m_Size(size),m_Signed(_signed),m_Integer(_integer) {}

		inline const unsigned long size() const { return m_Size; }
		inline const bool isInteger() const { return m_Integer; }
		inline const bool isSigned() const { return m_Signed; }

		inline void size(const unsigned long newsize) { m_Size=newsize; }
		inline void isInteger(const bool state) { m_Integer=state; }
		inline void isSigned(const bool state) { m_Signed=state; }

		inline bool operator ==(const Channeldatatype& chtype) const {
			return (size()==chtype.size()) && (isInteger()==chtype.isInteger()) && (isSigned()==chtype.isSigned());
		}
	protected:
		unsigned long m_Size; // size of the datatype, in bytes.
		bool m_Signed,m_Integer;
	};

	extern OVIS_API Channeldatatype Channeldatatype_Unsigned_Byte;
	extern OVIS_API Channeldatatype Channeldatatype_Unsigned_Word;
	extern OVIS_API Channeldatatype Channeldatatype_Signed_Word;
	extern OVIS_API Channeldatatype Channeldatatype_32Bit_Float;*/

}
}

#endif
