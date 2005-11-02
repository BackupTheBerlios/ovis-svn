#ifndef OVIS_VIDEO_INDEXFORMAT_HH_INCLUDED
#define OVIS_VIDEO_INDEXFORMAT_HH_INCLUDED

#include "../base/dll.hh"

namespace ovis {
namespace video {

enum Indexformat {
	Indexformat_16bit=0,
	Indexformat_32bit=1
};

OVIS_API unsigned long indexformatSizeLookup(const Indexformat format);

}
}

#endif
