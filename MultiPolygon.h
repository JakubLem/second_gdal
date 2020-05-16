#pragma once
#include "Geometry.h"
class MultiPolygon : public Geometry
{
public:
	MultiPolygon();
	void addGeometry(OGRGeometry* _geometry);
	~MultiPolygon();
};

