#ifndef LABELSWIDGET_H
#define LABELSWIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSqlQuery>

#include "include/models/EmployeeModel.h"
#include "include/models/SubjectModel.h"
#include "include/models/GroupModel.h"

class LabelsWidget : public QWidget
{
	Q_OBJECT

public:
    explicit LabelsWidget(QList<EmployeeModel>* e, QList<SubjectModel>* s, QList<GroupModel>* g, QSqlQuery* query, QWidget* parent = nullptr);
	~LabelsWidget();

    void SetupWidget();

private:
    void ClearGroupBox();

    void GenerateBtnClicked();
    void GenerateLabels(SubjectModel, QList<GroupModel>);

    QGroupBox* groupBox;
    QVBoxLayout* widgetLayout;

    QHBoxLayout* firstRowLayout;
    QHBoxLayout* secondRowLayout;

    QVBoxLayout* subjectsLayout;
    QListWidget* unassigned;
    QListWidget* assigned;

    QPushButton* btnGenerate;
    QButtonGroup* buttonGroup;

    QList<EmployeeModel>* employees;
    QList<SubjectModel>* subjects;
    QList<GroupModel>* groups;

    QSqlQuery* _query;
};

#endif // LABELSWIDGET_H
