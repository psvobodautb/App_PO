#include "include/widgets/employeeswidget.h"

#include <QLabel>
#include <QMessageBox>
//#include <QRegExp>
#include <QDebug>
#include <QSqlError>

#include "include/Functions.h"

EmployeesWidget::EmployeesWidget(QWidget* parent) :
    QWidget(parent), _query(nullptr)
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

void EmployeesWidget::LoadDb(QSqlQuery *query)
{
    if(query != nullptr)
    {
        _query = query;
        LoadEmployeesFromDb();
    }
}

void EmployeesWidget::LoadEmployeesFromDb()
{
    if(_query != nullptr){
        employees.clear();

        _query->prepare("Select * from Employees");
        _query->exec();

        while(_query->next()){
            EmployeeModel m;

            m.id = _query->value("id").toUuid();
            m.name = _query->value("name").toString();
            m.surname = _query->value("surname").toString();
            m.emailWork = _query->value("emailWork").toString();
            m.emailHome = _query->value("emailHome").toString();
            m.office = _query->value("office").toString();
            m.isDoctoral = _query->value("isDoctoral").toBool();
            m.harnessType = _query->value("harnessType").toDouble();
            m.workPointsNum = _query->value("workPointsNum").toInt();

            employees.append(m);
        }

        SetupEmployeesLayout();
    }
}

void EmployeesWidget::InsertEmployeeToDb(EmployeeModel model)
{
    if(_query != nullptr){

        _query->prepare("INSERT INTO Employees VALUES(:id, :name, :surname, :emailWork, :emailHome,"
        " :office, :isDoctoral, :harnessType, :workPointsNum)");
        _query->bindValue(0, ConvertUuidToString(model.id));
        _query->bindValue(1, model.name);
        _query->bindValue(2, model.surname);
        _query->bindValue(3, model.emailWork);
        _query->bindValue(4, model.emailHome);
        _query->bindValue(5, model.office);
        _query->bindValue(6, model.isDoctoral);
        _query->bindValue(7, model.harnessType);
        _query->bindValue(8, 0);

        _query->exec();
    }
}

void EmployeesWidget::DeleteEmployeeFromDb(QUuid id)
{
    if(_query != nullptr){
        _query->prepare("DELETE FROM Employees WHERE id=:id");
        _query->bindValue(0, ConvertUuidToString(id));
        _query->exec();

        _query->prepare("UPDATE Labels SET employeeId = NULL WHERE employeeId=:employeeId");
        _query->bindValue(0, ConvertUuidToString(id));
        _query->exec();
    }
}

void EmployeesWidget::SetupAddingLayout()
{
    addingLayout->addWidget(new QLabel("Nový:"));

    QHBoxLayout* lrow = new QHBoxLayout();
    lrow->addWidget(new QLabel("Jméno"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Příjmení"), Qt::AlignCenter);
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
    harnessType->setValue(1.);
    lrow->addWidget(harnessType, Qt::AlignLeft | Qt::AlignTop);

    QLabel* en = new QLabel("Body EN");
    en->setFixedWidth(100);
    lrow->addWidget(en);

    QLabel* cz = new QLabel("Body CZ");
    cz->setFixedWidth(100);
    lrow->addWidget(cz);

    btnAdd = new QPushButton("Přidat");
    btnAdd->setFixedWidth(100);
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
        lrow->addWidget(new QLabel(employees.at(i).emailHome));
        lrow->addWidget(new QLabel(employees.at(i).emailWork));
        lrow->addWidget(new QLabel(employees.at(i).office));

        if(employees.at(i).isDoctoral)
            lrow->addWidget(new QLabel("ano"));
        else
            lrow->addWidget(new QLabel("ne"));

        lrow->addWidget(new QLabel(QString::number(employees.at(i).harnessType, 'f', 1)));

        QLabel* pointsEN = new QLabel();
        pointsEN->setFixedWidth(100);
        pointsEN->setText(CalculatePoints(employees.at(i).id,Language::English));
        lrow->addWidget(pointsEN);

        QLabel* pointsCZ = new QLabel();
        pointsCZ->setFixedWidth(100);
        pointsCZ->setText(CalculatePoints(employees.at(i).id,Language::Czech));
        lrow->addWidget(pointsCZ);

        QPushButton* deleteBtn = new QPushButton("Smazat");
        deleteBtn->setFixedWidth(100);
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
        m.emailWork = emailWork->text();
        m.emailHome = emailHome->text();
        m.office = office->text();
        m.isDoctoral = ValidateIsDoctoral();
        m.harnessType = harnessType->value();

        employees.append(m);

        InsertEmployeeToDb(m);
    }

    SetupEmployeesLayout();

    emit EmployeesChanged();
}

void EmployeesWidget::DeleteEmployee()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(QObject::sender());

    QUuid id = employees.at(btn->property("row").toInt()).id;
    DeleteEmployeeFromDb(id);

    employeesLayout->removeItem(employeesLayout->itemAt(btn->property("row").toInt()));
    employees.removeAt(btn->property("row").toInt());

    SetupEmployeesLayout();

    emit EmployeesChanged();
}

bool EmployeesWidget::ValidateEmployee()
{
    bool isValid = true;

    if (name->text().isEmpty())
        isValid = false;

    if (surname->text().isEmpty())
        isValid = false;

    if (office->text().isEmpty())
        isValid = false;

    return isValid;
}

bool EmployeesWidget::ValidateIsDoctoral()
{
    if (isDoctoral->currentIndex() == 0)
        return false;

    return true;
}

QString EmployeesWidget::CalculatePoints(QUuid employeeId, Language lan)
{
    _query->prepare("SELECT lessonsNum, weeksNum, language, points, isValid FROM Labels WHERE employeeId=:employeeId");
    _query->bindValue(0,ConvertUuidToString(employeeId));
    _query->exec();

    double points = 0;

    while(_query->next()){

        Language labelLan = Language(_query->value("language").toInt());
        bool isValid = _query->value("isValid").toBool();

        if(lan == labelLan && isValid)
        {
            double lessons =  _query->value("lessonsNum").toDouble();
            double weeks = _query->value("weeksNum").toDouble();
            double labelPoints = _query->value("points").toDouble();

            double sum = lessons * weeks * labelPoints;
            points += sum;
        }
    }
    return QString::number(points, 'f', 2);
}
