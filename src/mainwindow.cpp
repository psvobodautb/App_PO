#include "include/mainwindow.h"

#include <QTabWidget>
#include "include/widgets/subjectswidget.h"
#include "include/widgets/employeeswidget.h"
#include "include/widgets/groupswidget.h"
#include "include/widgets/labelswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);

    this->setCentralWidget(tabWidget);

    employees = new EmployeesWidget();
    subjects = new SubjectsWidget();
    groups = new GroupsWidget();
    labels = new LabelsWidget();

    tabWidget->addTab(subjects,"Předměty");
    tabWidget->addTab(employees,"Zaměstnanci");
    tabWidget->addTab(groups,"Skupiny");
    tabWidget->addTab(labels,"Štítky");

    setMinimumSize(QSize(500,400));
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete tabWidget;
}

