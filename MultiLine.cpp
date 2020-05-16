#include "MultiLine.h"

void MultiLine::addGeometry(OGRGeometry* _geometry) {
	#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
	OGRMultiLineString * poLineString = _geometry->toMultiLineString();
	#else
	OGRPoint* poPoint = (OGRPoint*)_geometry;
	#endif
	multiLines.push_back(poLineString);
}



MultiLine::~MultiLine()
{
}
