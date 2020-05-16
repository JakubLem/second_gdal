#include "MultiPolygon.h"



MultiPolygon::MultiPolygon()
{
}

void MultiPolygon::addGeometry(OGRGeometry* _geometry) {
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
	OGRMultiPolygon * poPolygon = _geometry->toMultiPolygon();
#else
	OGRPoint* poPoint = (OGRPoint*)_geometry;
#endif
	polygons.push_back(poPolygon);
}


MultiPolygon::~MultiPolygon()
{
}
