#include "include/widgets/labelswidget.h"

#include <QSpacerItem>
#include <QVariant>
#include <QMessageBox>

static const char* SUBJECT_POSITION = "subject";

LabelsWidget::LabelsWidget(QList<EmployeeModel>* e, QList<SubjectModel>* s, QList<GroupModel>* g, QWidget* parent) :
    QWidget(parent), employees(e), subjects(s), groups(g)
{

    widgetLayout = new QVBoxLayout();
    subjectsLayout = nullptr;
    groupBox = new QGroupBox("Předměty");
    unassigned = new QListWidget();
    assigned = new QListWidget();

    buttonGroup = new QButtonGroup(this);

    firstRowLayout = new QHBoxLayout();
    firstRowLayout->setAlignment(Qt::AlignRight);

    btnGenerate = new QPushButton("Generovat");
    connect(btnGenerate, &QPushButton::pressed, this, &LabelsWidget::GenerateBtnClicked);
    firstRowLayout->addWidget(btnGenerate);

    secondRowLayout = new QHBoxLayout();
    secondRowLayout->addWidget(groupBox);
    secondRowLayout->addWidget(unassigned);
    secondRowLayout->addWidget(assigned);

    widgetLayout->addLayout(firstRowLayout);
    widgetLayout->addLayout(secondRowLayout);

    setLayout(widgetLayout);
}

LabelsWidget::~LabelsWidget()
{
}

void LabelsWidget::SetupWidget()
{
    if(subjectsLayout != nullptr)
        ClearGroupBox();

    subjectsLayout = new QVBoxLayout();
    groupBox->setLayout(subjectsLayout);

    buttonGroup = new QButtonGroup(this);

    for(int i = 0; i < subjects->count(); i++){
        QRadioButton* rb = new QRadioButton(subjects->at(i).name,this);
        rb->setProperty(SUBJECT_POSITION,i);
        buttonGroup->addButton(rb,i);
        subjectsLayout->addWidget(rb);
    }

    if(subjects->count() > 0)
        buttonGroup->button(0)->setChecked(true);

    QSpacerItem* spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    subjectsLayout->addItem(spacerItem);
}

void LabelsWidget::ClearGroupBox()
{
    if (subjectsLayout != NULL) {
        delete buttonGroup;

        QLayoutItem* item;
        while ((item = subjectsLayout->takeAt(0)) != NULL) {
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

        delete subjectsLayout;
    }
}

void LabelsWidget::GenerateBtnClicked()
{
    unassigned->clear();
    assigned->clear();

    qDebug() << "generovat";

    if(buttonGroup->checkedButton() != nullptr){
        int pos = buttonGroup->checkedButton()->property(SUBJECT_POSITION).toInt();

        qDebug() << "pozice " << pos;
        if(pos >= 0 && subjects->count() > 0 && pos < subjects->count()){
            SubjectModel subject = subjects->at(pos);
            QList<GroupModel> groupsWithSubject;

            for(int i = 0; i < groups->count(); i++){
                if(groups->at(i).year == subject.studyYear
                        && groups->at(i).isWinterSemester == subject.isWinterSemester
                        && groups->at(i).studyForm == subject.studyForm){
                     groupsWithSubject.append(groups->at(i));
                     GenerateLabels(subject,groupsWithSubject);
                }
            }

            if(groupsWithSubject.count() == 0)
                QMessageBox::warning(this,"Chyba!","K předmětu nelze dohledat žádné skupiny!");
        }
    }
}

void LabelsWidget::GenerateLabels(SubjectModel model, QList<GroupModel> list)
{
    for(int i = 0; i < list.size(); i++){
        int studentsNum = list.at(i).studentsNum;
        int groupNum = 1;

        qDebug() << studentsNum << model.groupSize;

        while(studentsNum > model.groupSize){
            studentsNum -= model.groupSize;
            QString groupName = QString("%1%2").arg(model.shortcut).arg(groupNum);
            groupNum += 1;
            unassigned->addItem(groupName);
        }

        if(studentsNum > 0){
            QString groupName = QString("%1%2").arg(model.shortcut).arg(groupNum);
            unassigned->addItem(groupName);
        }
    }
}
