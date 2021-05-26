#pragma once

#include "include/models/EmployeeModel.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSqlQuery>

class EmployeesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeesWidget(QWidget* parent = nullptr);
    ~EmployeesWidget();

    QList<EmployeeModel>* GetEmployees(){ return &employees; }
    void LoadDb(QSqlQuery* query);
signals:
    void EmployeesChanged();
    // DB
private:
    void LoadEmployeesFromDb();
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
    //bool ValidateEmail(QString email);
    //bool ValidatePhoneNumber(QString number);

    QVBoxLayout* widgetLayout;
    QVBoxLayout* addingLayout;
    QVBoxLayout* employeesLayout;

    QScrollArea* scrollArea;
    QWidget* innerWidget;

    QLineEdit* name;
    QLineEdit* surname;
    QLineEdit* phoneNumHome;
    QLineEdit* phoneNumWork;
    QLineEdit* emailWork;
    QLineEdit* emailHome;
    QLineEdit* office;
    QComboBox* isDoctoral;
    QDoubleSpinBox* harnessType; // typ �vazku
    QPushButton* btnAdd;

    QList<EmployeeModel> employees;

    QSqlQuery* _query;
};
