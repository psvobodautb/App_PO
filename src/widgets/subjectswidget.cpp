#include "include/widgets/subjectswidget.h"

#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QUuid>

enum Language {
    Czech,
    English
};

enum StudyType {
    Fulltime,
    Combined
};

enum StudyYear {
    First,
    Second,
    Third,
    Fourth,
    Fifth
};

enum Semester {
    Summer,
    Winter
};

enum Ending {
    Credit,
    ClassifiedCredit,
    Exam
};

// předělat na jeden gridlayout

SubjectsWidget::SubjectsWidget(QWidget *parent) :
    QWidget(parent)
{
    newLayout = new QGridLayout();
    subjectsLayout = new QGridLayout();
    widgetLayout = new QVBoxLayout(this);

    widgetLayout->addLayout(newLayout);
    widgetLayout->addLayout(subjectsLayout);
    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetLayout->addItem(spacerItem);

    SetupNewLayout();
    SetupSubjectsLayout();
}

SubjectsWidget::~SubjectsWidget()
{
    delete widgetLayout;
}

void SubjectsWidget::SetupWidget()
{
}

void SubjectsWidget::SetupNewLayout()
{
    newLayout->addWidget(new QLabel("Nový:"), 0, 0);

    shortcut = new QLineEdit;
    newLayout->addWidget(new QLabel("Zkratka"), 1, 0, Qt::AlignCenter);
    newLayout->addWidget(shortcut, 2, 0);

    name = new QLineEdit;
    newLayout->addWidget(new QLabel("Název"), 1, 1, Qt::AlignCenter);
    newLayout->addWidget(name, 2, 1);

    lecturesNum = new QSpinBox;
    lecturesNum->setMinimum(0);
    lecturesNum->setMaximum(50);
    newLayout->addWidget(new QLabel("Přednášky"), 1, 2, Qt::AlignCenter);
    newLayout->addWidget(lecturesNum, 2, 2);

    seminarsNum = new QSpinBox;
    seminarsNum->setMinimum(0);
    seminarsNum->setMaximum(50);
    newLayout->addWidget(new QLabel("Semináře"), 1, 3, Qt::AlignCenter);
    newLayout->addWidget(seminarsNum, 2, 3);

    excercisesNum = new QSpinBox;
    excercisesNum->setMinimum(0);
    excercisesNum->setMaximum(50);
    newLayout->addWidget(new QLabel("Cvičení"), 1, 4, Qt::AlignCenter);
    newLayout->addWidget(excercisesNum, 2, 4);

    weeksNum = new QSpinBox;
    weeksNum->setMinimum(1);
    weeksNum->setMaximum(20);
    newLayout->addWidget(new QLabel("Týdny"), 1, 5, Qt::AlignCenter);
    newLayout->addWidget(weeksNum, 2, 5);

    language = new QComboBox;
    language->addItem("Čeština", Language::Czech);
    language->addItem("Angličtina", Language::English);
    newLayout->addWidget(new QLabel("Jazyk"), 1, 6, Qt::AlignCenter);
    newLayout->addWidget(language, 2, 6);

    studyType = new QComboBox;
    studyType->addItem("Prezenční", StudyType::Fulltime);
    studyType->addItem("Kombinovaná", StudyType::Combined);
    newLayout->addWidget(new QLabel("Studium"), 1, 7, Qt::AlignCenter);
    newLayout->addWidget(studyType, 2, 7);

    studyYear = new QComboBox;
    studyYear->addItem("1.", StudyYear::First);
    studyYear->addItem("2.", StudyYear::Second);
    studyYear->addItem("3.", StudyYear::Third);
    studyYear->addItem("4.", StudyYear::Fourth);
    studyYear->addItem("5.", StudyYear::Fifth);
    newLayout->addWidget(new QLabel("Ročník"), 1, 8, Qt::AlignCenter);
    newLayout->addWidget(studyYear, 2, 8);

    semester = new QComboBox;
    semester->addItem("Letní", Semester::Summer);
    semester->addItem("Zimní", Semester::Winter);
    newLayout->addWidget(new QLabel("Semestr"), 1, 9, Qt::AlignCenter);
    newLayout->addWidget(semester, 2, 9);

    ending = new QComboBox;
    ending->addItem("Zápočet", Ending::Credit);
    ending->addItem("Klasifikovaný zápočet", Ending::ClassifiedCredit);
    ending->addItem("Zkouška", Ending::Exam);
    newLayout->addWidget(new QLabel("Ukončení"), 1, 10, Qt::AlignCenter);
    newLayout->addWidget(ending, 2, 10);

    newLayout->addWidget(new QLabel("Velikost skupiny"), 1, 11, Qt::AlignCenter);
    groupSize = new QSpinBox;
    groupSize->setMinimum(1);
    groupSize->setMaximum(1000);
    newLayout->addWidget(groupSize, 2, 11);

    btnAdd = new QPushButton("Přidat");
    newLayout->addWidget(btnAdd, 2, 12);
    connect(btnAdd, &QPushButton::released, this, &SubjectsWidget::AddSubject);
}

void SubjectsWidget::SetupSubjectsLayout()
{
    ClearSubjectsLayout();

    for (int i = 0; i < subjects.count(); i++) {
        subjectsLayout->addWidget(new QLabel(subjects.at(i).shortcut), i, 0);
        subjectsLayout->addWidget(new QLabel(subjects.at(i).name), i, 1);
        subjectsLayout->addWidget(new QLabel(QString::number(subjects.at(i).lecturesNum)), i, 2);
        subjectsLayout->addWidget(new QLabel(QString::number(subjects.at(i).seminarsNum)), i, 3);
        subjectsLayout->addWidget(new QLabel(QString::number(subjects.at(i).excercisesNum)), i, 4);
        subjectsLayout->addWidget(new QLabel(QString::number(subjects.at(i).weeksNum)), i, 5);

        if(subjects.at(i).isEnglish)
            subjectsLayout->addWidget(new QLabel("Angličtina"), i, 6);
        else
            subjectsLayout->addWidget(new QLabel("Čeština"), i, 6);

        if(subjects.at(i).isCombined)
            subjectsLayout->addWidget(new QLabel("Kombinovaná"), i, 7);
        else
            subjectsLayout->addWidget(new QLabel("Prezenční"), i, 7);

        QString studyYearStr = "";
        switch (subjects.at(i).studyYear) {
        case StudyYear::First:
            studyYearStr = "1.";
            break;
        case StudyYear::Second:
            studyYearStr = "2.";
            break; 
        case StudyYear::Third:
            studyYearStr = "3.";
            break;
        case StudyYear::Fourth:
            studyYearStr = "4.";
            break;
        case StudyYear::Fifth:
            studyYearStr = "5.";
            break;
        default:
            break;
        }
        subjectsLayout->addWidget(new QLabel(studyYearStr), i, 8);

        if(subjects.at(i).isWinterSemester)
            subjectsLayout->addWidget(new QLabel("Zimní"), i, 9);
        else
            subjectsLayout->addWidget(new QLabel("Letní"), i, 9);

        QString endingStr = "";
        switch (subjects.at(i).ending) {
        case Ending::Credit:
            endingStr = "Zápočet";
            break;
        case Ending::ClassifiedCredit:
            endingStr = "Klasifikovaný zápočet";
            break;
        case Ending::Exam:
            endingStr = "Zkouška";
            break;
        default:
            break;
        }
        subjectsLayout->addWidget(new QLabel(endingStr), i, 10);

        subjectsLayout->addWidget(new QLabel(QString::number(subjects.at(i).groupSize)), i, 11);

        QPushButton* deleteBtn = new QPushButton("Smazat");
        deleteBtn->setProperty("row", i);
        
        subjectsLayout->addWidget(deleteBtn, i, 12);
        connect(deleteBtn, &QPushButton::released, this, &SubjectsWidget::DeleteSubject);
    }

    spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetLayout->addItem(spacerItem);
}

void SubjectsWidget::ClearSubjectsLayout()
{
    if (subjectsLayout != NULL) {
        QLayoutItem* item;
        while ((item = subjectsLayout->takeAt(0)) != NULL) {
            delete item->widget();
            delete item;
        }

        delete subjectsLayout;
        delete spacerItem;
    }
}

void SubjectsWidget::AddSubject()
{
    bool canAdd = ValidateSubject();

    if (!canAdd) 
        int result = QMessageBox::warning(this,"Neplatný předmět", 
            "Nevyplnil jste všechny údaje k předmětu!\nZkontrolujte název, zkratku a počty vyučovacích hodin!", QMessageBox::Close);
    else
    {
        SubjectModel m;
        m.id = QUuid::createUuid();
        m.shortcut = shortcut->text();
        m.name = name->text();
        m.lecturesNum = lecturesNum->value();
        m.seminarsNum = seminarsNum->value();
        m.excercisesNum = excercisesNum->value();
        m.weeksNum = weeksNum->value();
        m.isEnglish = ValidateLanguage(language->currentData().toInt());
        m.isCombined = ValidateStudyType(studyType->currentData().toInt());
        m.studyYear = studyYear->currentIndex();
        m.isWinterSemester = ValidateSemester(semester->currentIndex());
        m.ending = semester->currentIndex();
        m.groupSize = groupSize->value();

        subjects.append(m);
    }

    SetupSubjectsLayout();
}

void SubjectsWidget::DeleteSubject()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(QObject::sender());

    subjectsLayout->removeItem(subjectsLayout->itemAt(btn->property("row").toInt()));
    subjects.removeAt(btn->property("row").toInt());

    SetupSubjectsLayout();
}

bool SubjectsWidget::ValidateSubject()
{
    bool isValid = true;

    if (shortcut->text().isEmpty())
        isValid = false;

    if (name->text().isEmpty())
        isValid = false;

    if (lecturesNum->value() == 0 && seminarsNum->value() == 0 && excercisesNum->value() == 0)
        isValid = false;

    return isValid;
}

bool SubjectsWidget::ValidateLanguage(int lang)
{
    if (lang == Language::English)
        return true;
        
    return false;
}

bool SubjectsWidget::ValidateStudyType(int type)
{
    if (type == StudyType::Combined)
        return true;

    return false;
}

bool SubjectsWidget::ValidateSemester(int s)
{
    if (s == Semester::Winter)
        return true;

    return false;
}
