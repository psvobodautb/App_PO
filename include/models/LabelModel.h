#pragma once

#include "include/models/SubjectModel.h"
#include "include/models/EmployeeModel.h"

#include "include/Enums.h"

class LabelModel
{
public:
    LabelModel();
    ~LabelModel();

    void assign(const LabelModel);
    void operator=(const LabelModel);

    QString name;
    QUuid subjectId;
    QUuid employeeId;

    LabelType labelType;
    int studentsNum;
    int lessonsNum;
    int weeksNum;
    Language language;
    int points;
};

