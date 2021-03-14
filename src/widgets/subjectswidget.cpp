#include "include/widgets/subjectswidget.h"

#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QUuid>
#include <QHBoxLayout>

#include "include/Enums.h"

SubjectsWidget::SubjectsWidget(QWidget *parent) :
    QWidget(parent)
{
    addingLayout = new QVBoxLayout();
    widgetLayout = new QVBoxLayout();
    subjectsLayout = NULL;

    innerWidget = new QWidget();
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(innerWidget);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setLayout(widgetLayout);

    widgetLayout->addLayout(addingLayout);
    widgetLayout->addWidget(scrollArea);

    SetupAddingLayout();
}

SubjectsWidget::~SubjectsWidget()
{
    delete subjectsLayout;
}

void SubjectsWidget::SetupSubjectsLayout()
{
    ClearSubjectsLayout();
    SetupSubjects();
}

void SubjectsWidget::SetupAddingLayout()
{
    addingLayout->addWidget(new QLabel(("Nový:")));

    QHBoxLayout* lrow = new QHBoxLayout();
    lrow->addWidget(new QLabel(QString::fromUtf8("Zkratka")), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Název"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Přednášky"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Semináře"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Cvičení"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Týdny"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Jazyk"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Studium"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Ročník"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Semestr"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Ukončení"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Velikost skupiny"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Počet kreditů"), Qt::AlignCenter);
    lrow->addWidget(new QLabel(""), Qt::AlignCenter);

    addingLayout->addLayout(lrow);
    lrow = new QHBoxLayout();

    shortcut = new QLineEdit;
    lrow->addWidget(shortcut, Qt::AlignLeft | Qt::AlignTop);

    name = new QLineEdit;
    lrow->addWidget(name, Qt::AlignLeft | Qt::AlignTop);

    lecturesNum = new QSpinBox;
    lecturesNum->setMinimum(0);
    lecturesNum->setMaximum(50);
    lrow->addWidget(lecturesNum, Qt::AlignLeft | Qt::AlignTop);

    seminarsNum = new QSpinBox;
    seminarsNum->setMinimum(0);
    seminarsNum->setMaximum(50);
    lrow->addWidget(seminarsNum, Qt::AlignLeft | Qt::AlignTop);

    excercisesNum = new QSpinBox;
    excercisesNum->setMinimum(0);
    excercisesNum->setMaximum(50);
    lrow->addWidget(excercisesNum, Qt::AlignLeft | Qt::AlignTop);

    weeksNum = new QSpinBox;
    weeksNum->setMinimum(1);
    weeksNum->setMaximum(20);
    lrow->addWidget(weeksNum, Qt::AlignLeft | Qt::AlignTop);

    language = new QComboBox;
    language->addItem("Čeština", Language::Czech);
    language->addItem("Angličtina", Language::English);
    lrow->addWidget(language, Qt::AlignLeft | Qt::AlignTop);

    studyType = new QComboBox;
    studyType->addItem("Prezenční", StudyType::Fulltime);
    studyType->addItem("Kombinovaná", StudyType::Combined);
    lrow->addWidget(studyType, Qt::AlignLeft | Qt::AlignTop);

    studyYear = new QComboBox;
    studyYear->addItem("1.", StudyYear::First);
    studyYear->addItem("2.", StudyYear::Second);
    studyYear->addItem("3.", StudyYear::Third);
    studyYear->addItem("4.", StudyYear::Fourth);
    studyYear->addItem("5.", StudyYear::Fifth);
    lrow->addWidget(studyYear, Qt::AlignLeft | Qt::AlignTop);

    semester = new QComboBox;
    semester->addItem("Letní", Semester::Summer);
    semester->addItem("Zimní", Semester::Winter);
    lrow->addWidget(semester, Qt::AlignLeft | Qt::AlignTop);

    ending = new QComboBox;
    ending->addItem("Zápočet", Ending::Credit);
    ending->addItem("Klasifikovaný zápočet", Ending::ClassifiedCredit);
    ending->addItem("Zkouška", Ending::Exam);
    lrow->addWidget(ending, Qt::AlignLeft | Qt::AlignTop);

    groupSize = new QSpinBox;
    groupSize->setMinimum(1);
    groupSize->setMaximum(1000);
    lrow->addWidget(groupSize, Qt::AlignLeft | Qt::AlignTop);

    creditsNum = new QSpinBox;
    creditsNum->setMinimum(1);
    creditsNum->setMaximum(30);
    lrow->addWidget(creditsNum, Qt::AlignLeft | Qt::AlignTop);

    btnAdd = new QPushButton("Přidat");
    lrow->addWidget(btnAdd, Qt::AlignLeft | Qt::AlignTop);
    connect(btnAdd, &QPushButton::released, this, &SubjectsWidget::AddSubject);

    addingLayout->addLayout(lrow);

    addingLayout->addWidget(new QLabel("Všechny předměty:"));
}

void SubjectsWidget::SetupSubjects()
{
    innerWidget = new QWidget();
    innerWidget->setMinimumSize(450,380);
    scrollArea->setWidget(innerWidget);

    subjectsLayout = new QVBoxLayout();

    for (int i = 0; i < subjects.count(); i++) {
        QHBoxLayout* lrow = new QHBoxLayout();

        lrow->addWidget(new QLabel(subjects.at(i).shortcut));
        lrow->addWidget(new QLabel(subjects.at(i).name));
        lrow->addWidget(new QLabel(QString::number(subjects.at(i).lecturesNum)));
        lrow->addWidget(new QLabel(QString::number(subjects.at(i).seminarsNum)));
        lrow->addWidget(new QLabel(QString::number(subjects.at(i).excercisesNum)));
        lrow->addWidget(new QLabel(QString::number(subjects.at(i).weeksNum)));

        if(subjects.at(i).isEnglish)
            lrow->addWidget(new QLabel("Angličtina"));
        else
            lrow->addWidget(new QLabel("Čeština"));

        if(subjects.at(i).isCombined)
            lrow->addWidget(new QLabel("Kombinovaná"));
        else
            lrow->addWidget(new QLabel("Prezenční"));

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
        lrow->addWidget(new QLabel(studyYearStr));

        if(subjects.at(i).isWinterSemester)
            lrow->addWidget(new QLabel("Zimní"));
        else
            lrow->addWidget(new QLabel("Letní"));

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
        lrow->addWidget(new QLabel(endingStr));

        lrow->addWidget(new QLabel(QString::number(subjects.at(i).groupSize)));

        lrow->addWidget(new QLabel(QString::number(subjects.at(i).credits)));

        QPushButton* deleteBtn = new QPushButton("Smazat");
        deleteBtn->setProperty("row", i);
        
        lrow->addWidget(deleteBtn);
        connect(deleteBtn, &QPushButton::released, this, &SubjectsWidget::DeleteSubject);

        subjectsLayout->addLayout(lrow);
    }

    QSpacerItem* spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    subjectsLayout->addItem(spacerItem);

    innerWidget->setLayout(subjectsLayout);
}

void SubjectsWidget::ClearSubjectsLayout()
{
    if (subjectsLayout != NULL) {
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
        delete innerWidget;
    }
}

void SubjectsWidget::AddSubject()
{
    bool canAdd = ValidateSubject();

    if (!canAdd) 
        QMessageBox::warning(this,"Neplatný předmět", 
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
        m.credits = creditsNum->value();

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
