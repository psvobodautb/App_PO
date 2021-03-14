#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

#include "include/models/GroupModel.h"

class GroupsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupsWidget(QWidget *parent = nullptr);
    ~GroupsWidget();

private:
    void SetupAddingLayout();
    void SetupGroups();
    void SetupGroupsLayout();
    void ClearGroupsLayout();

    void AddGroup();
    void DeleteGroup();
    bool ValidateGroup();

    int ValidateStudyType(int);

    QVBoxLayout* widgetLayout;
    QVBoxLayout* addingLayout;
    QVBoxLayout* groupsLayout;

    QScrollArea* scrollArea;
    QWidget* innerWidget;

    QLineEdit* shortcut;
    QLineEdit* name;
    QComboBox* studyType;
    QComboBox* studyYear;
    QComboBox* semester;
    QSpinBox* studentsNum;
    QComboBox* studyForm;
    QPushButton* btnAdd;

    QList<GroupModel> groups;
};