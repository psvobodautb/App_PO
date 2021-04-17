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

class EmployeesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeesWidget(QWidget* parent = nullptr);
    ~EmployeesWidget();

private:
    void SetupAddingLayout();
    void SetupEmployees();

    void SetupEmployeesLayout();
    void ClearEmployeesLayout();

    void AddEmployee();
    void DeleteEmployee();
    bool ValidateEmployee();

    bool ValidateIsDoctoral();
    bool ValidateEmail(QString email);
    bool ValidatePhoneNumber(QString number);

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
    QDoubleSpinBox* harnessType; // typ úvazku
    QPushButton* btnAdd;

    QList<EmployeeModel> employees;
};