#include <iostream>
#include "funkcje.h"
#include "Geometry.h"
#include "Points.h"
#include "MultiLine.h"
#include "MultiPolygon.h"

int main()
{
	std::vector<OGRGeometry*> geometry = read("dane/punkty/point_test.shp", "point_test", "id"); //tu by by³y punkty
	std::vector<OGRGeometry*> geometry2 = read("dane/punkty/point_test.shp", "point_test", "id"); //tu by by³y multilinie
	// i tak dalej, tutaj odczyty wszystkich plików


	std::vector<std::vector<OGRGeometry*>> result; //tutaj dodawanie wszystkich odczytów do result 
	//na dobr¹ sprawê mo¿na zrobiæto od razu wk³adaj¹c funckjê read() do result.push_back() --> zrobimy tak jak bêdzie ju¿ dzia³aæ
	result.push_back(geometry);
	result.push_back(geometry2);
	/*
	Geometry* geom;

	if (wkbFlatten(geometry.at(0)->getGeometryType()) == wkbPoint)
	{
		geom = new Points();
		for (auto row : geometry)
			geom->addGeometry(row);
			std::cout << geom[0];
	}	
	else
		geom = new Points();

	std::vector<OGRPoint*> points = geom->getAllPoints();
	std::cout << std::endl;
	for (auto row : points)
		std::cout << row->getX() << " | " << row->getY() << std::endl;
	geometry.clear();
	*/
	Points* punkty = new Points();
	MultiLine* multiLines = new MultiLine();
	MultiPolygon* multiPolygons = new MultiPolygon();
	for (auto obj : result) {
		if (wkbFlatten(obj.at(0)->getGeometryType()) == wkbPoint) {
			for (auto point : obj) {
				punkty->addGeometry(point);
			}
		} else if (wkbFlatten(obj.at(0)->getGeometryType()) == wkbLineString) {
			for (auto line : obj) {
				multiLines->addGeometry(line);
			}
		} else if (wkbFlatten(obj.at(0)->getGeometryType()) == wkbMultiPolygon) {
			for (auto polygon : obj) {
				multiPolygons->addGeometry(polygon);
			}
		}
	}





	//write("dane/multipunkty/write/multipunkty_write.shp", "multipunkty_write", "id", points, "multipoints");

	return 0;
}	