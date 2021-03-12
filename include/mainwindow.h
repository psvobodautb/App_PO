#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

};
#endif // MAINWINDOW_H
