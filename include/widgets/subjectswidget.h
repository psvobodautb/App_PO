#ifndef SUBJECTSWIDGET_H
#define SUBJECTSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>

#include "include/models/SubjectModel.h"

class SubjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubjectsWidget(QWidget *parent = nullptr);
    ~SubjectsWidget();

private:
    void SetupWidget();
    void SetupNewLayout();
    void SetupSubjectsLayout();
    void ClearSubjectsLayout();

    void AddSubject();
    void DeleteSubject();
    bool ValidateSubject();

    bool ValidateLanguage(int);
    bool ValidateStudyType(int);
    bool ValidateSemester(int);

    QGridLayout* newLayout;
    QGridLayout* subjectsLayout;
    QVBoxLayout* widgetLayout;

    QSpacerItem* spacerItem;

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
    QPushButton* btnAdd;

    QList<SubjectModel> subjects;
};

#endif // SUBJECTSWIDGET_H
