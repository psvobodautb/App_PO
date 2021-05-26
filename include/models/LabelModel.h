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

    QUuid id;
    QString name;
    QUuid subjectId;
    QUuid employeeId;
    QUuid groupId;

    LabelType labelType;
    int studentsNum;
    int lessonsNum;
    int weeksNum;
    Language language;
    double points;

    bool isValid;
};

