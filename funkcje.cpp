#include "funkcje.h"
#include "Points.h"

std::vector<OGRGeometry*> read(const  char* name, const  char* layer, const char* field)
{
	std::vector<OGRGeometry*> result;
	GDALAllRegister();
	GDALDataset* poDS = static_cast<GDALDataset*>(
		GDALOpenEx(name, GDAL_OF_VECTOR, NULL, NULL, NULL));
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}
	OGRLayer* poLayer = poDS->GetLayerByName(layer);
	OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
	poLayer->ResetReading();
	OGRFeature* poFeature; 
	OGRFieldDefn oField1(field, OFTInteger64); //typ pola int64 //int string etc -> 32 linia

	result.clear();
	while ((poFeature = poLayer->GetNextFeature()) != NULL)
	{
		for (int iField = 0; iField < poFDefn->GetFieldCount(); iField++)
		{
			OGRFieldDefn* poFieldDefn = poFDefn->GetFieldDefn(iField);
			switch (poFieldDefn->GetType())
			{
			case OFTInteger:
				printf("%d,", poFeature->GetFieldAsInteger(iField));
				break;
			case OFTInteger64:
				printf(CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64(iField));
				break;
			case OFTReal:
				printf("%.3f,", poFeature->GetFieldAsDouble(iField));
				break;
			case OFTString:
				printf("%s,", poFeature->GetFieldAsString(iField));
				break;
			default:
				printf("%s,", poFeature->GetFieldAsString(iField));
				break;
			}
		}

		OGRGeometry* poGeometry = poFeature->GetGeometryRef();

		if (poGeometry != NULL)
			result.push_back(poGeometry);
		else
			printf("NULL\n");
	}
	GDALClose(poDS);
	return result;
}






//ZOSTAWIÆ!!!!!!!!!!!!!

//funkcja odczytuj¹ca punkt
std::vector<double> readPointShapeFile(OGRPoint* poPoint)
{
	std::vector<double> punkt;

	double x = poPoint->getX();
	double y = poPoint->getY();
	
	punkt.push_back(x);
	punkt.push_back(y);

	return punkt;
}



//funkcja odczytuj¹ca multipunkt
std::vector<std::vector<double>> readMultiPointShapeFile(OGRGeometry* poGeometry, OGRFeature* poFeature)
{
	std::vector<std::vector<double>> punkty;

	printf("%d", poGeometry->getGeometryType());
	OGRMultiPoint* poMultipoint = (OGRMultiPoint*)poGeometry;
	int NumberOfGeometries = poMultipoint->getNumGeometries();
	for (int j = 0; j < NumberOfGeometries; j++)
	{
		OGRGeometry* poPointGeometry = poMultipoint->getGeometryRef(j);
		OGRPoint* poPoint = (OGRPoint*)poPointGeometry;
		punkty.push_back(readPointShapeFile(poPoint));
	}
	return punkty;
}


//funkcja odczytuj¹ca linie
std::vector<std::vector<double>> readLineShapeFile(OGRLineString* poLineString)
{
	std::vector<std::vector<double>> punkty;
	std::vector<double> punkt;
	int NumPoints = poLineString->getNumPoints();
	for (int i = 0; i < NumPoints; i++)
	{
		double x = poLineString->getX(i);
		double y = poLineString->getY(i);
		punkt.push_back(x);
		punkt.push_back(y);
		punkty.push_back(punkt);
		punkt.clear();
	}
	return punkty;
}



//funkcja odczytuj¹ca multilinie
std::vector<std::vector<double>> readMultiLineShapeFile(OGRGeometry* poGeometry, OGRFeature* poFeature)
{
	std::vector<std::vector<double>> punkty, punkty_linia;

	printf("%d", poGeometry->getGeometryType());
	OGRMultiLineString* poMultiLineString = (OGRMultiLineString*)poGeometry;
	int NumberOfGeometries = poMultiLineString->getNumGeometries();
	for (int j = 0; j < NumberOfGeometries; j++)
	{
		OGRGeometry* poLineGeometry = poMultiLineString->getGeometryRef(j);
		OGRLineString* poLineString = (OGRLineString*)poLineGeometry;
		punkty_linia = readLineShapeFile(poLineString);
		for (auto row : punkty_linia)
			punkty.insert(punkty.end(), row);
	}
	return punkty;
}



//funkcja rozdzialaj¹ca wszystkie typy do zapisu
void write(const char* name, const char* layer, const char* field, std::vector<std::vector<double>> points, const char* type) {
	const char* pszDriverName = "ESRI Shapefile";
	GDALDriver* poDriver;
	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL) {
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}
	GDALDataset* poDS;
	poDS = poDriver->Create(name, 0, 0, 0, GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}
	OGRLayer* poLayer;
	poLayer = poDS->CreateLayer(layer, NULL, wkbMultiPoint, NULL);
	if (poLayer == NULL) {
		printf("Layer creation failed.\n");
		exit(1);
	}
	OGRFieldDefn oField(field, OFTInteger64);
	oField.SetWidth(10);
	if (poLayer->CreateField(&oField) != OGRERR_NONE) {
		printf("Creating Name field failed.\n");
		exit(1);
	}

	if (type == "points")
		writePointShapeFile(field, poLayer, points);
	else if (type == "multipoints")
		writeMultiPointShapeFile(field, poLayer, points);
	else
		printf("Nie okreœlony typ.\n");
	GDALClose(poDS);
}



//funkcja zapisuj¹ca punkty
void writePointShapeFile(const char* field, OGRLayer* poLayer, std::vector<std::vector<double>> points)
{
	int i = 0;
	while (!feof(stdin) && i < points.size())
	{
		OGRFeature* poFeature;

		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
		poFeature->SetField(field, i);

		OGRPoint pt;

		pt.setX(points.at(i).at(0));
		pt.setY(points.at(i).at(1));

		poFeature->SetGeometry(&pt);

		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}

		OGRFeature::DestroyFeature(poFeature);
		i++;
	}
}



//funkcja zapisuj¹ca multipunkty
void writeMultiPointShapeFile(const char* field, OGRLayer* poLayer, std::vector<std::vector<double>> points)
{
	int i = 0;
	while (!feof(stdin) && i < points.size()) //tu zrobiæ for'a
	{
		OGRFeature* poFeature;

		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
		poFeature->SetField(field, i);

		OGRMultiPoint multi_pt;
		OGRPoint pt;

		pt.setX(points.at(i).at(0));
		pt.setY(points.at(i).at(1));

		multi_pt.addGeometry(&pt);
		poFeature->SetGeometry(&multi_pt);

		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}
		OGRFeature::DestroyFeature(poFeature);
		i++;
	}
}