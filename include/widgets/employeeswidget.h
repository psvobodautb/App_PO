#pragma once

#include "include/models/EmployeeModel.h"
#include "include/Enums.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSqlQuery>
#include <QUuid>
#include <QString>

class EmployeesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeesWidget(QWidget* parent = nullptr);
    ~EmployeesWidget();

    QList<EmployeeModel>* GetEmployees(){ return &employees; }
    void LoadDb(QSqlQuery* query);
    void LoadEmployeesFromDb();
signals:
    void EmployeesChanged();
    // DB
private:
    void InsertEmployeeToDb(EmployeeModel model);
    void DeleteEmployeeFromDb(QUuid id);
private:
    void SetupAddingLayout();
    void SetupEmployees();

    void SetupEmployeesLayout();
    void ClearEmployeesLayout();

    void AddEmployee();
    void DeleteEmployee();
    bool ValidateEmployee();

    bool ValidateIsDoctoral();

    QString CalculatePoints(QUuid employeeId, Language);

    QVBoxLayout* widgetLayout;
    QVBoxLayout* addingLayout;
    QVBoxLayout* employeesLayout;

    QScrollArea* scrollArea;
    QWidget* innerWidget;

    QLineEdit* name;
    QLineEdit* surname;
    QLineEdit* emailWork;
    QLineEdit* emailHome;
    QLineEdit* office;
    QComboBox* isDoctoral;
    QDoubleSpinBox* harnessType;
    QPushButton* btnAdd;

    QList<EmployeeModel> employees;

    QSqlQuery* _query;
};
