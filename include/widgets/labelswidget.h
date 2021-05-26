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
#include "include/models/LabelModel.h"

#include "include/widgets/labeldetailwidget.h"

class LabelsWidget : public QWidget
{
	Q_OBJECT

public:
    explicit LabelsWidget(QList<EmployeeModel>* e, QList<SubjectModel>* s, QList<GroupModel>* g, QSqlQuery* query, QWidget* parent = nullptr);
	~LabelsWidget();

    void SetupWidget();

    void GenerateLabelsSlot(QUuid groupId, QUuid subjectId);
    void GroupSizeChangedSlot(QUuid groupId);
    void SubjectSizeChangedSlot(QUuid subjectId);

    void LoadCurrentSubjectLabels();

signals:
    void UpdateDetail();

private:
    void ClearGroupBox();
    void LoadLabelsFromDb(int id);
    void RemoveLabelFromDb(QUuid labelId);
    void GenerateLabels(SubjectModel subject, GroupModel group);
    void UpdateLabels(SubjectModel subject, GroupModel group);

    QList<LabelModel> GenerateExcercises(SubjectModel subject, GroupModel group);
    QList<LabelModel> GenerateSeminars(SubjectModel subject, GroupModel group);

    LabelModel CreateLectureLabel(SubjectModel subject, GroupModel group);
    LabelModel CreateSeminarLabel(SubjectModel subject, GroupModel group, int groupNum, int groupSize);
    LabelModel CreateExcerciseLabel(SubjectModel subject, GroupModel group, int groupNum, int groupSize);

    void InsertLectureToDd(LabelModel lecture);
    void InsertSeminarToDb(LabelModel seminars);
    void InsertExcerciseToDb(LabelModel excercises);

    void UpdateEmployee(QString labelId, QString employeeId);

    void LabelClicked(QListWidgetItem* item);

    QGroupBox* groupBox;
    QVBoxLayout* widgetLayout;

    QHBoxLayout* secondRowLayout;

    QVBoxLayout* subjectsLayout;
    QListWidget* unassigned;
    QListWidget* assigned;

    QPushButton* btnGenerate;
    QButtonGroup* buttonGroup;

    QList<EmployeeModel>* employees;
    QList<SubjectModel>* subjects;
    QList<GroupModel>* groups;

    QList<LabelModel> currentLabels;

    LabelDetailWidget* detailWidget;

    QSqlQuery* _query;
};

#endif // LABELSWIDGET_H
