#include "include/models/LabelModel.h"

LabelModel::LabelModel()
{
}

LabelModel::~LabelModel()
{
}

void LabelModel::assign(const LabelModel other)
{
    id = other.id;
    name = other.name;
    subjectId = other.subjectId;
    employeeId = other.employeeId;
    groupId = other.groupId;
    labelType = other.labelType;
    studentsNum = other.studentsNum;
    lessonsNum = other.lessonsNum;
    weeksNum = other.weeksNum;
    language = other.language;
    points = other.points;
    isValid = other.isValid;
}

void LabelModel::operator=(const LabelModel other)
{
    assign(other);
}
