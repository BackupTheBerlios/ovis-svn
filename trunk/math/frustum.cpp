#include "frustum.hh"
#include "aabbvolume.hh"
#include "spherevolume.hh"

namespace ovis {
namespace math {

Frustum::Frustum()
{
	planes=new std::vector<Plane>;
}

Frustum::~Frustum()
{
	delete planes;
}

bool Frustum::intersectsWith(const SphereVolume& sphere) const
{
	return checkSphere(sphere.center(),sphere.radius())!=NotInFrustum;
}

bool Frustum::intersectsWith(const AABBVolume& aabb) const
{
	Vector3f extentsHalf=aabb.extents()*0.5f;
	return checkAABB(aabb.center()-extentsHalf,aabb.center()+extentsHalf)!=NotInFrustum;
}

bool Frustum::intersectsWith(const OBBVolume& obb) const
{
	return false;
}

Frustum::CullingResult Frustum::checkSphere(const Vector3f &center, const float radius) const
{
/*	int i=0;
	for (std::vector<Plane>::iterator itp=planes->begin();itp!=planes->end();++itp) {
		float distance=(*itp).normal()*center-(*itp).distance();
		
		Kernel::logger() << "dist " << (i++) << ": " << distance << " " << (*itp).normal()*center << " " << (*itp).distance() << "\n";
		
		if (distance<(-radius)) return NotInFrustum;
		else if (fabsf(distance)<radius) return PartialIntersection;
	}
		
	return FullInclusion;*/

/*	for (size_t i=0;i<planes->size();i++) {
		D3DXPLANE &p=(*planes)[i];
		if (p.a*center.x+p.b*center.y+p.c*center.z+p.d+radius < 0)
			return false;
	}()
		
	return true;*/
	
	//int i=0;
	for (std::vector<Plane>::iterator itp=planes->begin();itp!=planes->end();++itp) {
		Plane& p=(*itp);
		
		//Kernel::logger() << (i++) << ": " << p.a() << " " << p.b() << " " << p.c() << " " << p.d() << "\n";
		
		if (p.a()*center.x()+p.b()*center.y()+p.c()*center.z()+p.d()<=-radius) return NotInFrustum;
	}
	
	return FullInclusion;
}

Frustum::CullingResult Frustum::checkAABB(const Vector3f &vec1, const Vector3f &vec2) const
{
	Vector3f vCorner[8];
	int totalIn=0;
	
	vCorner[0].x()=vec1.x();	vCorner[0].y()=vec1.y();	vCorner[0].z()=vec1.z();
	vCorner[1].x()=vec2.x();	vCorner[1].y()=vec1.y();	vCorner[1].z()=vec1.z();
	vCorner[2].x()=vec2.x();	vCorner[2].y()=vec2.y();	vCorner[2].z()=vec1.z();
	vCorner[3].x()=vec1.x();	vCorner[3].y()=vec2.y();	vCorner[3].z()=vec1.z();

	vCorner[4].x()=vec1.x();	vCorner[4].y()=vec1.y();	vCorner[4].z()=vec2.z();
	vCorner[5].x()=vec2.x();	vCorner[5].y()=vec1.y();	vCorner[5].z()=vec2.z();
	vCorner[6].x()=vec2.x();	vCorner[6].y()=vec2.y();	vCorner[6].z()=vec2.z();
	vCorner[7].x()=vec1.x();	vCorner[7].y()=vec2.y();	vCorner[7].z()=vec2.z();

	for (std::vector<Plane>::iterator itp=planes->begin();itp!=planes->end();++itp)
	{
		int inCount=8;
		int ptsIn=1;

		for (int i=0;i<8;++i) {
			if ((*itp).distanceFromPlane(vCorner[i])>0.0f) {
				ptsIn=0;
				--inCount;
			}
		}

		if (inCount==0) return NotInFrustum;
		totalIn+=ptsIn;
	}

	if (totalIn==(int)(planes->size())) return FullInclusion;
	return PartialIntersection;
}

Frustum::CullingResult Frustum::checkPoint(const Vector3f &Point) const
{
	for (std::vector<Plane>::iterator itp=planes->begin();itp!=planes->end();++itp) {
		Plane &p=(*itp);
		float fA=p.a()*Point.x();
		float fB=p.b()*Point.y();
		float fC=p.c()*Point.z();
		if (fA+fB+fC+p.d()<0) return NotInFrustum;
	}
	return FullInclusion;
}

Frustum::CullingResult Frustum::checkPolygon(const Vector3f *points,const int numPoints) const
{
	int totalIn=0;

	for (std::vector<Plane>::iterator itp=planes->begin();itp!=planes->end();++itp)
	{
		int inCount=8;
		int ptsIn=1;

		for (int i=0;i<numPoints;++i) {
			if ((*itp).distanceFromPlane(points[i])>0.0f) {
				ptsIn=0;
				--inCount;
			}
		}

		if (inCount==0) return NotInFrustum;
		totalIn+=ptsIn;
	}

	if (totalIn==(int)(planes->size())) return FullInclusion;
	return PartialIntersection;
}


void Frustum::extract(const Matrix4f &projmatrix,const Matrix4f &view)
{
	planes->clear();

	Plane plane;
	Matrix4f matr;

	matr.loadIdentity();
	matr=view*projmatrix;
	
//	matr.dumpMatrix();
//	Kernel::logger() << "\n";
	
	// Left
	plane.a(matr[3]+matr[0]);
	plane.b(matr[7]+matr[4]);
	plane.c(matr[11]+matr[8]);
	plane.d(matr[15]+matr[12]);
	//plane.normal().normalize();
	planes->push_back(plane);
	
	// Right
	plane.a(matr[3]-matr[0]);
	plane.b(matr[7]-matr[4]);
	plane.c(matr[11]-matr[8]);
	plane.d(matr[15]-matr[12]);
	//plane.normal().normalize();
	planes->push_back(plane);

	// Bottom
	plane.a(matr[3]+matr[1]);
	plane.b(matr[7]+matr[5]);
	plane.c(matr[11]+matr[9]);
	plane.d(matr[15]+matr[13]);
	//plane.normal().normalize();
	planes->push_back(plane);

	// Top
	plane.a(matr[3]-matr[1]);
	plane.b(matr[7]-matr[5]);
	plane.c(matr[11]-matr[9]);
	plane.d(matr[15]-matr[13]);
	//plane.normal().normalize();
	planes->push_back(plane);

	// Near
	plane.a(matr[3]+matr[2]);
	plane.b(matr[7]+matr[6]);
	plane.c(matr[11]+matr[10]);
	plane.d(matr[15]+matr[14]);
	//plane.normal().normalize();
	planes->push_back(plane);
		
	// Far
	plane.a(matr[3]-matr[2]);
	plane.b(matr[7]-matr[6]);
	plane.c(matr[11]-matr[10]);
	plane.d(matr[15]-matr[14]);
//	plane.normal().normalize();

/*	projmatrix->dumpMatrix();
	printf("]\n%.02f %.02f %.02f %.02f\n",
		plane.a(),
		plane.b(),
		plane.c(),
		plane.d());*/

//	planes->push_back(plane);	
/*     p=&frustumPlane[RIGHT];
     p->n.x=m[3]-m[0];
     p->n.y=m[7]-m[4];
     p->n.z=m[11]-m[8];
     p->d=m[15]-m[12];
   
     p=&frustumPlane[LEFT];
     p->n.x=m[3]+m[0];
     p->n.y=m[7]+m[4];
     p->n.z=m[11]+m[8];
     p->d=m[15]+m[12];
   
     p=&frustumPlane[BOTTOM];
     p->n.x=m[3]+m[1];
     p->n.y=m[7]+m[5];
     p->n.z=m[11]+m[9];
     p->d=m[15]+m[13];
   
     p=&frustumPlane[TOP];
     p->n.x=m[3]-m[1];
     p->n.y=m[7]-m[5];
     p->n.z=m[11]-m[9];
     p->d=m[15]-m[13];
   
     p=&frustumPlane[PFAR];
     p->n.x=m[3]-m[2];
     p->n.y=m[7]-m[6];
     p->n.z=m[11]-m[10];
     p->d=m[15]-m[14];
   
     p=&frustumPlane[PNEAR];
     p->n.x=m[3]+m[2];
     p->n.y=m[7]+m[6];
     p->n.z=m[11]+m[10];
     p->d=m[15]+m[14];*/	
}




void Frustum::buildFromPolygon(const Vector3f *points,const int numPoints,const Matrix4f &view)
{
	planes->clear();

	Plane plane;
	Matrix4f matr;

	matr.loadIdentity();
	matr=view;
	
	Matrix4f matri=matr/*.invert()*/;
	Vector4f tempv(0.0f,0.0f,0.0f,1.0f);
	tempv=Vector4f(matri*tempv);
	Vector3f viewpos(tempv.x(),tempv.y(),tempv.z());
//	Kernel::logger() << "x " << tempv.x() << " y " << tempv.y() << " z " << tempv.z() << "\n";
	
	for (int point=0;point<numPoints;++point) {
		Vector3f point1=points[point];
		Vector3f point2=points[(point+1)%numPoints];
		
		Vector3f vec1=point2-viewpos;
		Vector3f vec2=point2-point1;
		Vector3f normal=vec1^vec2;
		normal.normalize();
		plane.normal(normal);
		plane.d(normal*viewpos);
//		Kernel::logger() << "a " << plane.a() << " b " << plane.b() << " c " << plane.c() << " d " << plane.d() << "\n";
		
		planes->push_back(plane);
	}
}

}
}
