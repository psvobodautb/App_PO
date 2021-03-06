#include "include/models/SubjectModel.h"

SubjectModel::SubjectModel()
{
}

SubjectModel::~SubjectModel()
{
}

void SubjectModel::assign(const SubjectModel m)
{
    id = m.id;
    shortcut = m.shortcut;
    name = m.name;
    lecturesNum = m.lecturesNum;
    seminarsNum = m.seminarsNum;
    excercisesNum = m.excercisesNum;
    weeksNum = m.weeksNum;
    isEnglish = m.isEnglish;
    isCombined = m.isCombined;
    studyYear = m.studyYear;
    isWinterSemester = m.isWinterSemester;
    studyForm = m.studyForm;
    ending = m.ending;
    groupSize = m.groupSize;
    credits = m.credits;
}

void SubjectModel::operator=(const SubjectModel m)
{
	assign(m);
}
