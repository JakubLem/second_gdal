#pragma once
#include "ogrsf_frmts.h"
#include <vector>
#include "Geometry.h"
class Points : public Geometry
{

public:
	//konstruktory
	Points(){}

	Points(OGRPoint* _poPoint)
	{
		points.push_back(_poPoint);
	}

	//destruktor
	virtual ~Points() {};

	//settery
	void addGeometry(OGRGeometry* _geometry);

	//gettery
	std::vector<OGRGeometry*> getAllPoints();

	//operator prze�adowania indeksuj�cego[]
	OGRGeometry* operator[](int i);

};