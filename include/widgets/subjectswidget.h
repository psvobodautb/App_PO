#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QScrollArea>

#include "include/models/SubjectModel.h"

class SubjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubjectsWidget(QWidget *parent = nullptr);
    ~SubjectsWidget();

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
    QComboBox* ending;
    QSpinBox* groupSize;
    QSpinBox* creditsNum;
    QPushButton* btnAdd;

    QList<SubjectModel> subjects;
};
