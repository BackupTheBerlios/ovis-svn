#include "../base/itypes.hh"
#include "channeldatatype.hh"

namespace ovis {
namespace video {

	unsigned int channeldatatypeSizeLookup(const Channeldatatype channeldatatype)
	{
		switch (channeldatatype) {
			case Channeldatatype_Int8:return 1;
			case Channeldatatype_Uint8:return 1;
			case Channeldatatype_Int16:return 2;
			case Channeldatatype_Uint16:return 2;
			case Channeldatatype_Float16:return 2;
			case Channeldatatype_Float32:return 4;
			default:return 0;
		}
	}

	const char *channeldatatypeShortName(const Channeldatatype channeldatatype)
	{
		switch (channeldatatype) {
			case Channeldatatype_Int8:return "int8";
			case Channeldatatype_Uint8:return "uint8";
			case Channeldatatype_Int16:return "int16";
			case Channeldatatype_Uint16:return "uint16";
			case Channeldatatype_Float16:return "float16";
			case Channeldatatype_Float32:return "float32";
			default:return 0;
		}
	}

	const char *channeldatatypeLongName(const Channeldatatype channeldatatype)
	{
		switch (channeldatatype) {
			case Channeldatatype_Int8:return "signed 8-bit integer";
			case Channeldatatype_Uint8:return "unsigned 8-bit integer";
			case Channeldatatype_Int16:return "signed 16-bit integer";
			case Channeldatatype_Uint16:return "unsigned 16-bit integer";
			case Channeldatatype_Float16:return "signed 16-bit \"half\" float";
			case Channeldatatype_Float32:return "signed 32-bit IEEE float";
			default:return 0;
		}
	}


	/*Channeldatatype Channeldatatype_Unsigned_Byte(sizeof(ovis_uint8),false,true);
	Channeldatatype Channeldatatype_Unsigned_Word(sizeof(ovis_uint16),false,true);
	Channeldatatype Channeldatatype_Signed_Word(sizeof(ovis_int16),true,true);
	Channeldatatype Channeldatatype_32Bit_Float(sizeof(float),true,false);*/

}
}
