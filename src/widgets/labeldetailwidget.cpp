#include "include/widgets/labeldetailwidget.h"

#include <QGridLayout>
#include "include/Functions.h"

LabelDetailWidget::LabelDetailWidget(QList<EmployeeModel>* e, QList<SubjectModel>* s, QWidget *parent) : QWidget(parent)
{
    id = new QLabel();
    name = new QLabel();
    subject = new QLabel();
    employee = new QComboBox();
    connect(employee, &QComboBox::currentIndexChanged,this,&LabelDetailWidget::EmployeeChanged);
    type = new QLabel();
    studentsNum = new QLabel();
    lessonsNum = new QLabel();
    weeksNum = new QLabel();
    language = new QLabel();
    points = new QLabel();

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel("id: "),0,0);
    layout->addWidget(id,0,1);

    layout->addWidget(new QLabel("název: "),1,0);
    layout->addWidget(name,1,1);

    layout->addWidget(new QLabel("předmět: "),2,0);
    layout->addWidget(subject,2,1);

    layout->addWidget(new QLabel("vyučující: "),3,0);
    layout->addWidget(employee,3,1);

    layout->addWidget(new QLabel("typ: "),4,0);
    layout->addWidget(type,4,1);

    layout->addWidget(new QLabel("počet studentů: "),5,0);
    layout->addWidget(studentsNum,5,1);

    layout->addWidget(new QLabel("počet hodin: "),6,0);
    layout->addWidget(lessonsNum,6,1);

    layout->addWidget(new QLabel("počet týdnů: "),7,0);
    layout->addWidget(weeksNum,7,1);

    layout->addWidget(new QLabel("jazyk: "),8,0);
    layout->addWidget(language,8,1);

    layout->addWidget(new QLabel("úvazkové body: "),9,0);
    layout->addWidget(points,9,1);

    deleteBtn = new QPushButton("Smazat");
    connect(deleteBtn, &QPushButton::pressed, this, &LabelDetailWidget::RemoveLabelFromDb);
    layout->addWidget(deleteBtn,10,0,1,2);

    setLayout(layout);
    setMinimumSize(300,400);

    employees = e;
    subjects = s;
}

void LabelDetailWidget::ShowWidget(LabelModel m)
{
    currentLabel = m;

    id->setText(ConvertUuidToString(m.id));
    name->setText(m.name);

    subject->clear();

    for(int i = 0; i < subjects->count(); i++){
        if(m.subjectId == subjects->at(i).id)
            subject->setText(subjects->at(i).name);
    }

    Update();

    SetType(m.labelType);
    studentsNum->setText(QString::number(m.studentsNum));
    lessonsNum->setText(QString::number(m.lessonsNum));
    weeksNum->setText(QString::number(m.weeksNum));

    if(m.language == Language::Czech)
        language->setText("Čeština");
    else
        language->setText("Angličtina");

    points->setText(QString::number(m.points));

    if(!m.isValid){
        employee->setDisabled(true);
        deleteBtn->setDisabled(false);
    }
    else{
        employee->setDisabled(false);
        deleteBtn->setDisabled(true);
    }

    show();
    raise();
    activateWindow();
}

void LabelDetailWidget::Update()
{
    employee->blockSignals(true);

    int index = -1;
    employee->clear();

    for(int i = 0; i < employees->count(); i++){
        employee->addItem(QString("%1 %2").arg(employees->at(i).name).arg(employees->at(i).surname),employees->at(i).id);
        if(ConvertUuidToString(employees->at(i).id) == ConvertUuidToString(currentLabel.employeeId))
            index = i;
    }

    employee->addItem("---");

    if(index == -1)
        employee->setCurrentIndex(employee->count() - 1);
    else
        employee->setCurrentIndex(index);

    employee->blockSignals(false);
}

void LabelDetailWidget::SetType(LabelType t)
{
    switch(t){
    case Credit:
        type->setText("Zápočet");
        break;
    case ClassifiedCredit:
        type->setText("Klasifikovaný zápočet");
        break;
    case Exam:
        type->setText("Zkouška");
        break;
    case Lecture:
        type->setText("Přednáška");
        break;
    case Seminar:
        type->setText("Seminář");
        break;
    case Excercise:
        type->setText("Cvičení");
        break;
    default:
        type->clear();
        break;
    }
}

void LabelDetailWidget::EmployeeChanged(int index)
{
    QString id;

    if(employee->count() > 1 && index != employee->count() - 1)
        id = ConvertUuidToString(employee->itemData(index).toUuid());
    else
        id = NULL;

    emit UpdateEmployeeInDb(ConvertUuidToString(currentLabel.id),id);
}

void LabelDetailWidget::RemoveLabelFromDb()
{
    emit DeleteLabel(currentLabel.id);
    hide();
}
