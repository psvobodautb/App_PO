#include "include/widgets/subjectswidget.h"

#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QUuid>
#include <QHBoxLayout>
#include <QString>
#include <QIntValidator>

#include "include/Enums.h"
#include "include/Functions.h"

static const char* GROUP_SIZE = "group";

SubjectsWidget::SubjectsWidget(QWidget *parent) :
    QWidget(parent), _query(nullptr)
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

void SubjectsWidget::LoadDb(QSqlQuery* query)
{
    if(query != nullptr){
        _query = query;
        LoadSubjectsFromDb();
    }
}

void SubjectsWidget::LoadSubjectsFromDb()
{
    _query->exec("Select * from Subjects");

    while(_query->next()){
        SubjectModel model;

        model.id = _query->value("id").toUuid();
        model.shortcut = _query->value("shortcut").toString();
        model.name = _query->value("name").toString();
        model.lecturesNum = _query->value("lecturesNum").toInt();
        model.seminarsNum = _query->value("seminarsNum").toInt();
        model.excercisesNum = _query->value("excercisesNum").toInt();
        model.weeksNum = _query->value("weeksNum").toInt();
        model.isEnglish = _query->value("isEnglish").toBool();
        model.isCombined = _query->value("isCombined").toBool();
        model.studyYear = _query->value("studyYear").toInt();
        model.isWinterSemester = _query->value("isWinterSemester").toBool();
        model.studyForm = _query->value("studyForm").toInt();
        model.ending = _query->value("ending").toInt();
        model.groupSize = _query->value("groupSize").toInt();
        model.credits = _query->value("credits").toInt();

        subjects.append(model);
        SetupSubjects();
    }
}

void SubjectsWidget::InsertSubjectToDb(SubjectModel model)
{
    if(_query != nullptr){

        _query->prepare("INSERT INTO Subjects VALUES(:id, :shortcut, :name, :lecturesNum, :seminarsNum, :excercisesNum, :weeksNum, :isEnglish,"
                        " :isCombined, :studyYear, :isWinterSemester, :studyForm, :ending, :groupSize, :credits)");
        _query->bindValue(0, ConvertUuidToString(model.id));
        _query->bindValue(1, model.shortcut);
        _query->bindValue(2, model.name);
        _query->bindValue(3, model.lecturesNum);
        _query->bindValue(4, model.seminarsNum);
        _query->bindValue(5, model.excercisesNum);
        _query->bindValue(6, model.weeksNum);
        _query->bindValue(7, model.isEnglish);
        _query->bindValue(8, model.isCombined);
        _query->bindValue(9, model.studyYear);
        _query->bindValue(10, model.isWinterSemester);
        _query->bindValue(11, model.studyForm);
        _query->bindValue(12, model.ending);
        _query->bindValue(13, model.groupSize);
        _query->bindValue(14, model.credits);

        _query->exec();
    }
}

void SubjectsWidget::DeleteSubjectFromDb(QUuid id)
{
    if(_query != nullptr){
        _query->prepare("DELETE FROM Subjects WHERE id=:id");
        _query->bindValue(0, ConvertUuidToString(id));
        _query->exec();

        _query->prepare("DELETE FROM Connections WHERE subjectId=:subjectId");
        _query->bindValue(0, ConvertUuidToString(id));
        _query->exec();
    }
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
    lrow->addWidget(new QLabel("Typ"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Ročník"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Semestr"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Studium"), Qt::AlignCenter);
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

    studyForm = new QComboBox;
    studyForm->addItem("Bakalářský", StudyForm::Bachelor);
    studyForm->addItem("Magisterský", StudyForm::Master);
    studyForm->addItem("Doktorský", StudyForm::Doctoral);
    lrow->addWidget(studyForm, Qt::AlignLeft | Qt::AlignTop);

    ending = new QComboBox;
    ending->addItem("Zápočet", LabelType::Credit);
    ending->addItem("Klasifikovaný zápočet", LabelType::ClassifiedCredit);
    ending->addItem("Zkouška", LabelType::Exam);
    lrow->addWidget(ending, Qt::AlignLeft | Qt::AlignTop);

    groupSize = new QLineEdit;
    groupSize->setValidator(new QIntValidator(1,1000));
    groupSize->setText(QString("%1").arg(1));
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

        QString formStr = "";
        switch (subjects.at(i).studyForm) {
        case StudyForm::Bachelor:
            formStr = "Bakalářský";
            break;
        case StudyForm::Master:
            formStr = "Magisterský";
            break;
        case StudyForm::Doctoral:
            formStr = "Doktorský";
            break;
        default:
            break;
        }

        lrow->addWidget(new QLabel(formStr));

        QString endingStr = "";
        switch (subjects.at(i).ending) {
        case LabelType::Credit:
            endingStr = "Zápočet";
            break;
        case LabelType::ClassifiedCredit:
            endingStr = "Klasifikovaný zápočet";
            break;
        case LabelType::Exam:
            endingStr = "Zkouška";
            break;
        default:
            break;
        }
        lrow->addWidget(new QLabel(endingStr));

        QLineEdit* groupSize = new QLineEdit();
        groupSize->setFixedWidth(120);
        groupSize->setText(QString("%1").arg(subjects.at(i).groupSize));
        groupSize->setValidator(new QIntValidator(1,1000));
        groupSize->setProperty(GROUP_SIZE,QVariant(i));
        connect(groupSize, &QLineEdit::textChanged, this, &SubjectsWidget::ValueChanged);
        lrow->addWidget(groupSize);

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
        m.groupSize = groupSize->text().toInt();
        m.credits = creditsNum->value();

        subjects.append(m);

        SetupSubjectsLayout();

        InsertSubjectToDb(m);
    }
}

void SubjectsWidget::DeleteSubject()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(QObject::sender());

    QUuid id = subjects.at(btn->property("row").toInt()).id;
    DeleteSubjectFromDb(id);

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

void SubjectsWidget::ValueChanged(const QString& text)
{
    QLineEdit* sender = qobject_cast<QLineEdit*>(QObject::sender());

    if(sender != nullptr){
        int position = sender->property(GROUP_SIZE).toInt();
        subjects[position].groupSize = text.toInt();

        if(_query != nullptr){
            _query->prepare("UPDATE Subjects SET groupSize=:groupSize WHERE id=:id");
            _query->bindValue(0,text.toInt());
            _query->bindValue(1,ConvertUuidToString(subjects[position].id));
            _query->exec();
        }
    }
}
