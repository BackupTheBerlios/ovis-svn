#ifndef OVIS_VIDEO_ATTRIBUTESTREAM_HH_INCLUDED
#define OVIS_VIDEO_ATTRIBUTESTREAM_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/dll.hh"
#include "../base/itypes.hh"

namespace ovis {
namespace video {

	class Indexstream;

	class OVIS_API Attributebuffer:public base::Datablock
	{
	public:
		inline void attributeAmount(const ovis_uint32 numAttributes) { resize(numAttributes*sizeof(ovis_uint32)); }
		inline ovis_uint32 attributeAmount() const { return size()/sizeof(ovis_uint32); }
		
		inline ovis_uint32 operator [](const ovis_uint32 index) const { return (reinterpret_cast<const ovis_uint32*>(data()))[index]; }
		inline ovis_uint32& operator [](const ovis_uint32 index) { return (reinterpret_cast<ovis_uint32*>(data()))[index]; }

		void reorganize(Indexstream* pIndexstream=0);
	private:
		void mergesort(Attributebuffer& array,Attributebuffer& permutation,int lo,int hi);
	};

}
}

#endif
