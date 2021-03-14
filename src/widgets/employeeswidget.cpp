#include "include/widgets/employeeswidget.h"

#include <QLabel>
#include <QMessageBox>

EmployeesWidget::EmployeesWidget(QWidget* parent) :
    QWidget(parent)
{
    widgetLayout = new QVBoxLayout();
    scrollArea = new QScrollArea();
    addingLayout = new QVBoxLayout();
    employeesLayout = NULL;

    innerWidget = new QWidget();

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(innerWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    widgetLayout->addLayout(addingLayout);
    widgetLayout->addWidget(scrollArea);

    setLayout(widgetLayout);

    SetupAddingLayout();
}

EmployeesWidget::~EmployeesWidget()
{
    delete widgetLayout;
}

void EmployeesWidget::SetupAddingLayout()
{
    addingLayout->addWidget(new QLabel("Nový:"));

    QHBoxLayout* lrow = new QHBoxLayout();
    lrow->addWidget(new QLabel("Jméno"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Příjmení"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Telefon"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Telefon do práce"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Osobní email"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Pracovní email"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Kancelář"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Doktorand"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Typ úvazku"), Qt::AlignCenter);
    lrow->addWidget(new QLabel(""), Qt::AlignCenter);

    addingLayout->addLayout(lrow);
    lrow = new QHBoxLayout();

    name = new QLineEdit;
    lrow->addWidget(name, Qt::AlignLeft | Qt::AlignTop);

    surname = new QLineEdit;
    lrow->addWidget(surname, Qt::AlignLeft | Qt::AlignTop);

    phoneNumHome = new QLineEdit;
    lrow->addWidget(phoneNumHome, Qt::AlignLeft | Qt::AlignTop);

    phoneNumWork = new QLineEdit;
    lrow->addWidget(phoneNumWork, Qt::AlignLeft | Qt::AlignTop);

    emailHome = new QLineEdit;
    lrow->addWidget(emailHome, Qt::AlignLeft | Qt::AlignTop);

    emailWork = new QLineEdit;
    lrow->addWidget(emailWork, Qt::AlignLeft | Qt::AlignTop);

    office = new QLineEdit;
    lrow->addWidget(office, Qt::AlignLeft | Qt::AlignTop);

    isDoctoral = new QComboBox;
    isDoctoral->addItem("ne");
    isDoctoral->addItem("ano");
    lrow->addWidget(isDoctoral, Qt::AlignLeft | Qt::AlignTop);

    harnessType = new QDoubleSpinBox;
    harnessType->setMinimum(0.);
    harnessType->setMaximum(1.);
    harnessType->setSingleStep(0.1);
    lrow->addWidget(harnessType, Qt::AlignLeft | Qt::AlignTop);

    btnAdd = new QPushButton("Přidat");
    lrow->addWidget(btnAdd, Qt::AlignLeft | Qt::AlignTop);
    connect(btnAdd, &QPushButton::released, this, &EmployeesWidget::AddEmployee);

    addingLayout->addLayout(lrow);

    addingLayout->addWidget(new QLabel("Všichni zaměstnanci:"));
}

void EmployeesWidget::SetupEmployees()
{
    innerWidget = new QWidget();
    innerWidget->setMinimumSize(450, 380);
    scrollArea->setWidget(innerWidget);

    employeesLayout = new QVBoxLayout();

    for (int i = 0; i < employees.count(); i++) {
        QHBoxLayout* lrow = new QHBoxLayout();

        lrow->addWidget(new QLabel(employees.at(i).name));
        lrow->addWidget(new QLabel(employees.at(i).surname));
        lrow->addWidget(new QLabel(employees.at(i).phoneNumHome));
        lrow->addWidget(new QLabel(employees.at(i).phoneNumWork));
        lrow->addWidget(new QLabel(employees.at(i).emailHome));
        lrow->addWidget(new QLabel(employees.at(i).emailWork));
        lrow->addWidget(new QLabel(employees.at(i).office));

        if(employees.at(i).isDoctoral)
            lrow->addWidget(new QLabel("ano"));
        else
            lrow->addWidget(new QLabel("ne"));

        lrow->addWidget(new QLabel(QString::number(employees.at(i).harnessType, 'f', 1)));

        QPushButton* deleteBtn = new QPushButton("Smazat");
        deleteBtn->setProperty("row", i);

        lrow->addWidget(deleteBtn);
        connect(deleteBtn, &QPushButton::released, this, &EmployeesWidget::DeleteEmployee);

        employeesLayout->addLayout(lrow);
    }

    QSpacerItem* spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    employeesLayout->addItem(spacerItem);

    innerWidget->setLayout(employeesLayout);
}

void EmployeesWidget::SetupEmployeesLayout()
{
    ClearEmployeesLayout();
    SetupEmployees();
}

void EmployeesWidget::ClearEmployeesLayout()
{
    if (employeesLayout != NULL) {
        QLayoutItem* item;
        while ((item = employeesLayout->takeAt(0)) != NULL) {
            if (item->spacerItem())
                delete item->spacerItem();
            else if (item->widget()) {
                delete item->widget();
                delete item;
            }
            else if (item->layout()) {
                delete item->layout();
            }
        }

        delete employeesLayout;
        delete innerWidget;
    }
}

void EmployeesWidget::AddEmployee()
{
    bool canAdd = ValidateEmployee();

    if (!canAdd)
        QMessageBox::warning(this, "Neplatný zaměstnanec",
            "Nevyplnil jste všechny údaje o zaměstnanci!", QMessageBox::Close);
    else {
        EmployeeModel m;
        m.id = QUuid::createUuid();
        m.name = name->text();
        m.surname = surname->text();
        m.phoneNumHome = phoneNumHome->text();
        m.phoneNumWork = phoneNumWork->text();
        m.emailWork = emailWork->text();
        m.emailHome = emailHome->text();
        m.office = office->text();
        m.isDoctoral = ValidateIsDoctoral();
        m.harnessType = harnessType->value();

        employees.append(m);
    }

    SetupEmployeesLayout();
}

void EmployeesWidget::DeleteEmployee()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(QObject::sender());

    employeesLayout->removeItem(employeesLayout->itemAt(btn->property("row").toInt()));
    employees.removeAt(btn->property("row").toInt());

    SetupEmployeesLayout();
}

bool EmployeesWidget::ValidateEmployee()
{
    bool isValid = true;

    return isValid;
}

bool EmployeesWidget::ValidateIsDoctoral()
{
    if (isDoctoral->currentIndex() == 0)
        return false;

    return true;
}
