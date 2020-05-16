#pragma once
#include <vector>
#include "Geometry.h"
class Map
{
	std::vector<Geometry*> geom;
public:
	Map();
	~Map();
};

