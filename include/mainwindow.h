#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class QTabWidget;
class SubjectsWidget;
class EmployeesWidget;
class GroupsWidget;
class LabelsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTabWidget* tabWidget;
    SubjectsWidget* subjects;
    EmployeesWidget* employees;
    GroupsWidget* groups;
    LabelsWidget* labels;

    QSqlDatabase db;
    QSqlQuery query;
};
#endif // MAINWINDOW_H
