#ifndef WOBJ_GROUP
#define WOBJ_GROUP

#include "Nullable.h"
#include "Enums.h"
#include <string>
#include <vector>


class WOBJ_Group
{
public:
	std::string GroupName;
	int BeginIndex;
	int EndIndex;
	std::vector<int*>* Indicies;
	Nullable<PolygonType> PolygonType;

	WOBJ_Group()
	{
		BeginIndex = 0;
		EndIndex = 0;
		PolygonType = PolygonType::PT_Triangles;
	}
};

#endif