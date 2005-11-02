#include <zlib.h>
#include <memory.h>
#include "zpkhandler.hh"

namespace ovis {
namespace base {

	void ZPackHandler::pack(const Datablock &rDatablockSrc,PackedDatablock &rPackedBlockDest)
	{
		z_stream stream;
		Datablock tempblk;
		tempblk.resize(512*1024);
		
		memset(&stream,0,sizeof(stream));
		stream.next_in=(Bytef*)rDatablockSrc.data();
		stream.avail_in=(uInt)rDatablockSrc.size();
		stream.next_out=(Bytef*)tempblk.data();
		stream.avail_out=(uInt)tempblk.size();
		
		deflateInit(&stream,9);
		int ret;
		unsigned long oldsize=0;

		while (1) {
			ret=deflate(&stream,Z_FINISH);
			if ((ret!=Z_OK) && (ret!=Z_STREAM_END)) break;
			unsigned long newsize=oldsize+(unsigned long)stream.total_out;
			rPackedBlockDest.resize(newsize);
			memcpy((rPackedBlockDest.data())+oldsize,tempblk.data(),(unsigned long)stream.total_out);
			
			if (ret==Z_STREAM_END) break; // end of stream
			
			stream.next_out=(Bytef*)tempblk.data();
			stream.avail_out=(uInt)tempblk.size();
			stream.total_out=0;
			
			if (stream.avail_in==0) break;
			oldsize=newsize;
		}
		deflateEnd(&stream);

		zlibHeader(true);
		compressedSize(rPackedBlockDest.size());
		uncompressedSize(rDatablockSrc.size());
	}

	void ZPackHandler::unpack(const PackedDatablock &rPackedBlockSrc,Datablock &rDatablockDest)
	{
		z_stream stream;
		Datablock tempblk;
		tempblk.resize(512*1024);
		
		memset(&stream,0,sizeof(stream));
		stream.next_in=(Bytef*)rPackedBlockSrc.data();
		stream.avail_in=(uInt)rPackedBlockSrc.size();
		stream.next_out=(Bytef*)tempblk.data();
		stream.avail_out=(uInt)tempblk.size();
		
		int ret;
		if (zlibHeader()) {
			inflateInit(&stream);
			unsigned long oldsize=0;
		
			while (1) {
				ret=inflate(&stream,Z_SYNC_FLUSH);
				if ((ret!=Z_OK) && (ret!=Z_STREAM_END)) break;
				unsigned long newsize=oldsize+(unsigned long)stream.total_out;
				rDatablockDest.resize(newsize);
				memcpy((rDatablockDest.data())+oldsize,tempblk.data(),(unsigned long)stream.total_out);
			
				if (ret==Z_STREAM_END) break; // end of stream
			
				stream.next_out=(Bytef*)tempblk.data();
				stream.avail_out=(uInt)tempblk.size();
				stream.total_out=0;
			
				if (stream.avail_in==0) break;
				oldsize=newsize;
			}
			inflateEnd(&stream);
		} else { // !zlibHeader
			rDatablockDest.resize(uncompressedSize());
			stream.next_out=(Bytef*)rDatablockDest.data();
			stream.avail_out=(uInt)rDatablockDest.size();

			ret = inflateInit2(&stream,-MAX_WBITS); // -MAX_WBITS is necessary for deflated data in a zip
			ret = inflate(&stream, Z_SYNC_FLUSH);
			ret = inflateEnd(&stream);	
		}
	}

}
}
