#pragma once
#include "Geometry.h"
class MultiLine : public Geometry
{
public:
	MultiLine(){}
	void addGeometry(OGRGeometry* _geometry);
	~MultiLine();
};

