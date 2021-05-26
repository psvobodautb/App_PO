#include "include/models/GroupModel.h"

GroupModel::GroupModel()
{
}

GroupModel::~GroupModel()
{
}

void GroupModel::assign(const GroupModel m)
{
    id = m.id;
	shortcut = m.shortcut;
	name = m.name;
	isCombined = m.isCombined;
	year = m.year;
    isWinterSemester = m.isWinterSemester;
	studentsNum = m.studentsNum;
	studyForm = m.studyForm;
}

void GroupModel::operator=(const GroupModel m)
{
    assign(m);
}

