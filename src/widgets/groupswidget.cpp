#include "include/widgets/groupswidget.h"

#include <QLabel>
#include <QMessageBox>

#include "include/Enums.h"

GroupsWidget::GroupsWidget(QWidget *parent) :
    QWidget(parent)
{
    widgetLayout = new QVBoxLayout();
    scrollArea = new QScrollArea();
    addingLayout = new QVBoxLayout();
    groupsLayout = NULL;

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

GroupsWidget::~GroupsWidget()
{
    delete widgetLayout;
}

void GroupsWidget::SetupAddingLayout()
{
    addingLayout->addWidget(new QLabel("Nov�:"));

    QHBoxLayout* lrow = new QHBoxLayout();
    lrow->addWidget(new QLabel("Zkratka"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("N�zev"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Typ"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Ro�n�k"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Semestr"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Po�et student�"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Studium"), Qt::AlignCenter);
    lrow->addWidget(new QLabel(""), Qt::AlignCenter);

    addingLayout->addLayout(lrow);
    lrow = new QHBoxLayout();

    shortcut = new QLineEdit;
    lrow->addWidget(shortcut, Qt::AlignLeft | Qt::AlignTop);

    name = new QLineEdit;
    lrow->addWidget(name, Qt::AlignLeft | Qt::AlignTop);

    studyType = new QComboBox;
    studyType->addItem("Prezen�n�", StudyType::Fulltime);
    studyType->addItem("Kombinovan�", StudyType::Combined);
    lrow->addWidget(studyType, Qt::AlignLeft | Qt::AlignTop);

    studyYear = new QComboBox;
    studyYear->addItem("1.", StudyYear::First);
    studyYear->addItem("2.", StudyYear::Second);
    studyYear->addItem("3.", StudyYear::Third);
    studyYear->addItem("4.", StudyYear::Fourth);
    studyYear->addItem("5.", StudyYear::Fifth);
    lrow->addWidget(studyYear, Qt::AlignLeft | Qt::AlignTop);

    semester = new QComboBox;
    semester->addItem("Letn�", Semester::Summer);
    semester->addItem("Zimn�", Semester::Winter);
    lrow->addWidget(semester, Qt::AlignLeft | Qt::AlignTop);

    studentsNum = new QSpinBox;
    studentsNum->setMinimum(1);
    studentsNum->setMaximum(1000);
    lrow->addWidget(studentsNum, Qt::AlignLeft | Qt::AlignTop);

    studyForm = new QComboBox;
    studyForm->addItem("Bakal��sk�", StudyForm::Bachelor);
    studyForm->addItem("Magistersk�", StudyForm::Master);
    studyForm->addItem("Doktorsk�", StudyForm::Doctoral);
    lrow->addWidget(studyForm, Qt::AlignLeft | Qt::AlignTop);

    btnAdd = new QPushButton("P�idat");
    lrow->addWidget(btnAdd, Qt::AlignLeft | Qt::AlignTop);
    connect(btnAdd, &QPushButton::released, this, &GroupsWidget::AddGroup);

    addingLayout->addLayout(lrow);

    addingLayout->addWidget(new QLabel("V�echny skupiny:"));
}

void GroupsWidget::SetupGroups()
{
    innerWidget = new QWidget();
    innerWidget->setMinimumSize(450, 380);
    scrollArea->setWidget(innerWidget);

    groupsLayout = new QVBoxLayout();

    for (int i = 0; i < groups.count(); i++) {
        QHBoxLayout* lrow = new QHBoxLayout();

        lrow->addWidget(new QLabel(groups.at(i).shortcut));
        lrow->addWidget(new QLabel(groups.at(i).name));

        if (groups.at(i).isCombined)
            lrow->addWidget(new QLabel("Kombinovan�"));
        else
            lrow->addWidget(new QLabel("Prezen�n�"));

        QString yearStr = "";
        switch (groups.at(i).year) {
        case StudyYear::First:
            yearStr = "1.";
            break;
        case StudyYear::Second:
            yearStr = "2.";
            break;
        case StudyYear::Third:
            yearStr = "3.";
            break;
        case StudyYear::Fourth:
            yearStr = "4.";
            break;
        case StudyYear::Fifth:
            yearStr = "5.";
            break;
        default:
            break;
        }

        lrow->addWidget(new QLabel(yearStr));

        if (groups.at(i).semester == Semester::Summer)
            lrow->addWidget(new QLabel("Letn�"));
        else
            lrow->addWidget(new QLabel("Zimn�"));

        lrow->addWidget(new QLabel(QString::number(groups.at(i).studentsNum)));

        QString formStr = "";
        switch (groups.at(i).studyForm) {
        case StudyForm::Bachelor:
            formStr = "Bakal��sk�";
            break;
        case StudyForm::Master:
            formStr = "Magistersk�";
            break;
        case StudyForm::Doctoral:
            formStr = "Doktorsk�";
            break;
        default:
            break;
        }

        lrow->addWidget(new QLabel(formStr));

        QPushButton* deleteBtn = new QPushButton("Smazat");
        deleteBtn->setProperty("row", i);

        lrow->addWidget(deleteBtn);
        connect(deleteBtn, &QPushButton::released, this, &GroupsWidget::DeleteGroup);

        groupsLayout->addLayout(lrow);
    }

    QSpacerItem* spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    groupsLayout->addItem(spacerItem);

    innerWidget->setLayout(groupsLayout);
}

void GroupsWidget::SetupGroupsLayout()
{
    ClearGroupsLayout();
    SetupGroups();
}

void GroupsWidget::ClearGroupsLayout()
{
    if (groupsLayout != NULL) {
        QLayoutItem* item;
        while ((item = groupsLayout->takeAt(0)) != NULL) {
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

        delete groupsLayout;
        delete innerWidget;
    }
}

void GroupsWidget::AddGroup()
{
    bool canAdd = ValidateGroup();

    if(!canAdd)
        QMessageBox::warning(this, "Neplatn� skupina",
            "Nevyplnil jste v�echny �daje ke skupin�!\nZkontrolujte n�zev a zkratku!", QMessageBox::Close);
    else {
        GroupModel m;
        m.shortcut = shortcut->text();
        m.name = name->text();
        m.isCombined = ValidateStudyType(studyType->currentIndex());
        m.year = studyYear->currentIndex();
        m.semester = semester->currentIndex();
        m.studentsNum = studentsNum->value();
        m.studyForm = studyForm->currentIndex();

        groups.append(m);
    }

    SetupGroupsLayout();
}

void GroupsWidget::DeleteGroup()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(QObject::sender());

    groupsLayout->removeItem(groupsLayout->itemAt(btn->property("row").toInt()));
    groups.removeAt(btn->property("row").toInt());

    SetupGroupsLayout();
}

bool GroupsWidget::ValidateGroup()
{
    bool canAdd = true;

    if (shortcut->text().isEmpty())
        canAdd = false;

    if (name->text().isEmpty())
        canAdd = false;

    return canAdd;
}

int GroupsWidget::ValidateStudyType(int type)
{
    if (type == StudyType::Combined)
        return true;

    return false;
}