#include <assert.h>
#include "attributebuffer.hh"
#include "indexiterator.hh"	
#include "indexstream.hh"

namespace ovis {
namespace video {

	void Attributebuffer::mergesort(Attributebuffer& array,Attributebuffer& permutation,int lo,int hi)
	{
		if (lo==hi) return;

		int length=hi-lo+1,pivot=(lo+hi)/2,i;
		mergesort(array,permutation,lo,pivot);
		mergesort(array,permutation,pivot+1,hi);

		ovis_uint32 *working=new ovis_uint32[length];
		ovis_uint32 *temppermutation=new ovis_uint32[length];
		for(i=0;i<length;++i) { working[i]=array[lo+i]; temppermutation[i]=permutation[lo+i]; }
		int m1=0,m2=pivot-lo+1;

		for(i=0;i<length;++i) {
			if(m2<=hi-lo) {
				if(m1<=pivot-lo) {
					if(working[m1]>working[m2]) {
						array[i+lo]=working[m2];
						permutation[i+lo]=temppermutation[m2++];
					}
					else {
						array[i+lo]=working[m1];
						permutation[i+lo]=temppermutation[m1++];
					}
				}
				else {
					array[i+lo]=working[m2];
					permutation[i+lo]=temppermutation[m2++];
				}
			}
			else {
				array[i+lo]=working[m1];
				permutation[i+lo]=temppermutation[m1++];
			}
		}

		delete [] working;
		delete [] temppermutation;
	}

	void Attributebuffer::reorganize(Indexstream* pIndexstream)
	{
		Attributebuffer permutation;
		permutation.attributeAmount(attributeAmount());
		ovis_uint32 attridx;
		for (attridx=0;attridx<attributeAmount();++attridx) permutation[attridx]=attridx;

		mergesort(*this,permutation,0,attributeAmount());

		if (!pIndexstream) return;
		pIndexstream->map(0);

		Indexiterator iiterator(*pIndexstream);
		ovis_uint32 *indices=new ovis_uint32[permutation.attributeAmount()*3],i;

		for (i=0;i<3*permutation.attributeAmount();++i) { indices[i]=iiterator.index(); ++iiterator; }

		iiterator.positionInStream(0);

		for (attridx=0;attridx<attributeAmount();++attridx) {
			ovis_uint32 faceidx=permutation[attridx];
			assert(faceidx<permutation.attributeAmount());

			for (ovis_uint32 i=0;i<3;++i) {
				iiterator.index(indices[faceidx*3+i]);

				++iiterator;
			}
		}

		delete [] indices;
		pIndexstream->unmap();
	}

}
}
