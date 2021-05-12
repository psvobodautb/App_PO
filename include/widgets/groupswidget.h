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
#include <QSqlQuery>

#include "include/models/GroupModel.h"

class GroupsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupsWidget(QWidget* parent = nullptr);
    ~GroupsWidget();

    QList<GroupModel>* GetGroups(){ return &groups; }
    void LoadDb(QSqlQuery* query);

    // DB
private:
    void LoadGroupsFromDb();
    void InsertGroupToDb(GroupModel model);
    void DeleteGroupFromDb(QUuid id);
    QString ConvertUuidToString(QUuid id);

private:
    void SetupAddingLayout();
    void SetupGroups();
    void SetupGroupsLayout();
    void ClearGroupsLayout();

    void AddGroup();
    void DeleteGroup();
    bool ValidateGroup();

    int ValidateStudyType(int);
    bool ValidateSemester(int);

    void ValueChanged(const QString&);

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
    QLineEdit* studentsNum;
    QComboBox* studyForm;
    QPushButton* btnAdd;

    QList<GroupModel> groups;

    QSqlQuery* _query;
};
