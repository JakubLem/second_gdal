#pragma once
#include "ogrsf_frmts.h"
#include <vector>
class Geometry
{
	
public:
	std::vector<OGRGeometry*> points;
	std::vector<OGRGeometry*> multiLines;
	std::vector<OGRGeometry*> polygons;
	
};