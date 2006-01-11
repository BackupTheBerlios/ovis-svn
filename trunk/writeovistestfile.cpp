#include <fstream>
#include <inttypes.h>

const uint64_t numtriangles=4;
const uint64_t numvertices=4;
const uint32_t numproperties=1;
const uint32_t numfaceattributearrays=1;
const double minProp=0,maxProp=100.0;

struct Vertex
{
	float x,y,z;
	double prop[numproperties];
};

int main()
{

	const uint64_t indices[4*3]={
		0,1,2,
		0,2,3,
		0,3,1,
		1,2,3
	};
	const uint32_t faceattributes[4]={0xFFFFFFFF,0,0,0};
	const Vertex vertices[4]={
		{0,1,0,		50.0},
		{-1,-1,-1,	1.0},
		{1,-1,-1,	100.0},
		{0,-1,1,	37.0}
	};
	



	std::ofstream f("blaovismesh",std::ios::binary);

#define BINWRITE(foo) f.write((const char*)(&(foo)),sizeof(foo))
	BINWRITE(numtriangles);
	BINWRITE(numvertices);
	BINWRITE(numproperties);
	BINWRITE(numfaceattributearrays);
	BINWRITE(minProp);
	BINWRITE(maxProp);
	
	uint64_t i;
	uint32_t j;
	
	for (i=0;i<numvertices;++i) {
		BINWRITE(vertices[i].x);
		BINWRITE(vertices[i].y);
		BINWRITE(vertices[i].z);
		
		for (j=0;j<numproperties;++j)
			BINWRITE(vertices[i].prop[j]);
	}
	
	for (i=0;i<numtriangles;++i) {
		BINWRITE(indices[i*3+0]);
		BINWRITE(indices[i*3+1]);
		BINWRITE(indices[i*3+2]);
	}
	
	for (i=0;i<numtriangles;++i) {
		BINWRITE(faceattributes[i]);
	}

	return 0;
}
