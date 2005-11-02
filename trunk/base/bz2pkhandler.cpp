#include <bzlib.h>
#include <memory.h>
#include "bz2pkhandler.hh"

namespace ovis {
namespace base {

	void BZ2PackHandler::pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest)
	{
		Datablock tempblk;
		tempblk.resize(512*1024);
		
		bz_stream strm;
		memset(&strm,0,sizeof(strm));
		strm.next_in=(char*)rDatablockSrc.data();
		strm.avail_in=(unsigned int)rDatablockSrc.size();
		strm.next_out=(char*)tempblk.data();
		strm.avail_out=(unsigned int)tempblk.size();
			
		BZ2_bzCompressInit(&strm,m_Blocksize100k,0,30);
		
		unsigned long oldsize=0;
		int ret;
		while (1) {
			ret=BZ2_bzCompress(&strm,BZ_FINISH);
			if ((ret!=BZ_FINISH_OK) && (ret!=BZ_STREAM_END)) break; // error
			//printf("ret %i out_lo32 %u avail_out %u\n",ret,strm.total_out_lo32,strm.avail_out);
			//printf("f_out_lo32: %u\n",strm.total_out_lo32);
			unsigned long newsize=oldsize+(unsigned long)strm.total_out_lo32;
			rPackedBlockDest.resize(newsize);
			memcpy((rPackedBlockDest.data())+oldsize,tempblk.data(),(unsigned long)strm.total_out_lo32);
			
			if (ret==BZ_STREAM_END) break; // end of stream
			
			strm.next_out=(char*)tempblk.data();
			strm.avail_out=(unsigned int)tempblk.size();
			strm.total_out_lo32=0;
			strm.total_out_hi32=0;
			
			oldsize=newsize;
		}
		//printf("ret %i total_in %u total_out %u\n",ret,strm.total_in_lo32,strm.total_out_lo32);
		BZ2_bzCompressEnd(&strm);
	}

	void BZ2PackHandler::unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest)
	{	
		Datablock tempblk;
		tempblk.resize(512*1024);
		
		bz_stream strm;
		memset(&strm,0,sizeof(strm));
		strm.next_in=(char*)rPackedBlockSrc.data();
		strm.avail_in=(unsigned int)rPackedBlockSrc.size();
		strm.next_out=(char*)tempblk.data();
		strm.avail_out=(unsigned int)tempblk.size();
			
		BZ2_bzDecompressInit(&strm,0,0);
		
		unsigned long oldsize=0;
		int ret;
		while (1) {
			ret=BZ2_bzDecompress(&strm);
			if ((ret!=BZ_OK) && (ret!=BZ_STREAM_END)) break; // error
	//		printf("ret %i out_lo32 %u avail_out %u\n",ret,strm.total_out_lo32,strm.avail_out);
			unsigned long newsize=oldsize+(unsigned long)strm.total_out_lo32;
			rDatablockDest.resize(newsize);
			memcpy((rDatablockDest.data())+oldsize,tempblk.data(),(unsigned long)strm.total_out_lo32);
			
			if (ret==BZ_STREAM_END) break; // end of stream
			
			strm.next_out=(char*)tempblk.data();
			strm.avail_out=(unsigned int)tempblk.size();
			strm.total_out_lo32=0;
			strm.total_out_hi32=0;
			
			oldsize=newsize;
		}
	//	printf("ret %i\n",ret);
		BZ2_bzDecompressEnd(&strm);
	}

}
}
