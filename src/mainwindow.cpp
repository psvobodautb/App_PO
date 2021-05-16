#include "include/mainwindow.h"

#include <QTabWidget>
#include <QCoreApplication>

#include "include/widgets/subjectswidget.h"
#include "include/widgets/employeeswidget.h"
#include "include/widgets/groupswidget.h"
#include "include/widgets/labelswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabWidget = new QTabWidget(this);

    this->setCentralWidget(tabWidget);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(qApp->applicationDirPath() + "/podb");

    db.open();

    query = QSqlQuery(db);
    //query.exec("Select * from album");

    employees = new EmployeesWidget();
    subjects = new SubjectsWidget();
    groups = new GroupsWidget();
    labels = new LabelsWidget(employees->GetEmployees(),subjects->GetSubjects(),groups->GetGroups(), &query);

    tabWidget->addTab(subjects,"Předměty");
    tabWidget->addTab(employees,"Zaměstnanci");
    tabWidget->addTab(groups,"Skupiny");
    tabWidget->addTab(labels,"Štítky");

    connect(tabWidget, &QTabWidget::tabBarClicked, [this](){
        labels->SetupWidget();
    });

    if(db.isOpen()){
        subjects->LoadDb(&query);
        groups->LoadDb(&query);
        employees->LoadDb(&query);
    }

    setMinimumSize(QSize(500,400));
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete tabWidget;
}

