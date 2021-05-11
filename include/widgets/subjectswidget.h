#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QScrollArea>
#include <QSqlQuery>

#include "include/models/SubjectModel.h"

class SubjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubjectsWidget(QWidget *parent = nullptr);
    ~SubjectsWidget();

    QList<SubjectModel>* GetSubjects(){ return &subjects; }

    void LoadDb(QSqlQuery* query);

    // DB
private:
    void LoadSubjectsFromDb();
    void InsertSubjectToDb(SubjectModel model);
    void DeleteSubjectFromDb(QUuid id);
    QString ConvertUuidToString(QUuid id);

private:
    void SetupSubjectsLayout();
    void SetupAddingLayout();
    void SetupSubjects();
    void ClearSubjectsLayout();

    void AddSubject();
    void DeleteSubject();
    bool ValidateSubject();

    bool ValidateLanguage(int);
    bool ValidateStudyType(int);
    bool ValidateSemester(int);

    void ValueChanged(int);

    QVBoxLayout* addingLayout;
    QVBoxLayout* widgetLayout;
    QVBoxLayout* subjectsLayout;

    QScrollArea* scrollArea;
    QWidget* innerWidget;

    QLineEdit* shortcut;
    QLineEdit* name;
    QComboBox* teacher;
    QSpinBox* lecturesNum;
    QSpinBox* seminarsNum;
    QSpinBox* excercisesNum;
    QSpinBox* weeksNum;
    QComboBox* language;
    QComboBox* studyType;
    QComboBox* studyYear;
    QComboBox* semester;
    QComboBox* studyForm;
    QComboBox* ending;
    QSpinBox* groupSize;
    QSpinBox* creditsNum;
    QPushButton* btnAdd;

    QList<SubjectModel> subjects;

    QSqlQuery* _query;
};
