#include "picbuffer.hh"
#include "pixmap.hh"
#include "simplepicbuffer.hh"
#include "cubemap.hh"
#include <math.h>

void ovis::video::Cubemap::transferPic(const ovis_uint32 level,const Picbuffer& pic)
{	
	transferData(level,pic,pic.width()*pic.bytesPerPixel(),pic.rgbalayout(),pic.channeldatatype());
}

