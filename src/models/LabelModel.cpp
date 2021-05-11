#include "include/models/LabelModel.h"

LabelModel::LabelModel()
{
}

LabelModel::~LabelModel()
{
}

void LabelModel::assign(const LabelModel other)
{
    name = other.name;
    subjectId = other.subjectId;
    employeeId = other.employeeId;

    labelType = other.labelType;
    studentsNum = other.studentsNum;
    lessonsNum = other.lessonsNum;
    weeksNum = other.weeksNum;
    language = other.language;
    points = other.points;
}

void LabelModel::operator=(const LabelModel other)
{
    assign(other);
}
