#include "Points.h"
#include <iostream>
//settery
void Points::addGeometry(OGRGeometry* _geometry)
{
	#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
		OGRPoint* poPoint = _geometry->toPoint();
	#else
		OGRPoint* poPoint = (OGRPoint*)_geometry;
	#endif
	points.push_back(poPoint);
}

//gettery
std::vector<OGRGeometry*> Points::getAllPoints()
{
	return points;
}

OGRGeometry* Points::operator[](int i) {
	int x = 0;
	for (auto obj : this->points) {
		if (x == i) {
			return obj;
		}
		x++;
	}
}