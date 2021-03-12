#include "include/models/GroupModel.h"

GroupModel::GroupModel()
{
}

GroupModel::~GroupModel()
{
}

void GroupModel::assign(const GroupModel m)
{
	shortcut = m.shortcut;
	name = m.name;
	isCombined = m.isCombined;
	year = m.year;
	semester = m.semester;
	studentsNum = m.studentsNum;
}

void GroupModel::operator=(const GroupModel m)
{
	assign(m);
}
