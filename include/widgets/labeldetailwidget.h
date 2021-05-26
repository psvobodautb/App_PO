#pragma once

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include "include/models/LabelModel.h"
#include "include/models/EmployeeModel.h"
#include "include/models/SubjectModel.h"
#include "include/Enums.h"

class LabelDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LabelDetailWidget(QList<EmployeeModel>* employees, QList<SubjectModel>* subjects, QWidget *parent = nullptr);

    void ShowWidget(LabelModel m);
    void Update();
signals:
    void UpdateEmployeeInDb(QString labelId, QString employeeId);
    void DeleteLabel(QUuid labelId);

private:
    void SetType(LabelType);
    void EmployeeChanged(int index);

    void RemoveLabelFromDb();

    QLabel* id;
    QLabel* name;
    QLabel* subject;
    QComboBox* employee;
    QLabel* type;
    QLabel* studentsNum;
    QLabel* lessonsNum;
    QLabel* weeksNum;
    QLabel* language;
    QLabel* points;

    QPushButton* deleteBtn;

    LabelModel currentLabel;

    QList<EmployeeModel>* employees;
    QList<SubjectModel>* subjects;
};
