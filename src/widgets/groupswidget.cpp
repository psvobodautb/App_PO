#include "include/widgets/groupswidget.h"

#include <QLabel>
#include <QMessageBox>
#include <QIntValidator>

#include "include/Enums.h"
#include "include/Functions.h"

const char * NUM_PROPERTY = "num";

GroupsWidget::GroupsWidget(QWidget* parent) :
    QWidget(parent), _query(nullptr)
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
    delete connectingWidget;
}

void GroupsWidget::LoadDb(QSqlQuery *query)
{
    if(query != nullptr){
        _query = query;

        connectingWidget = new ConnectingWidget(_query);

        LoadGroupsFromDb();
    }
}

void GroupsWidget::LoadGroupsFromDb()
{
    _query->exec("Select * from Groups");

    while(_query->next()){
        GroupModel model;

        model.id = _query->value("id").toUuid();
        model.shortcut = _query->value("shortcut").toString();
        model.name = _query->value("name").toString();
        model.isCombined = _query->value("isCombined").toBool();
        model.year = _query->value("year").toInt();
        model.isWinterSemester = _query->value("isWinterSemester").toBool();
        model.studentsNum = _query->value("studentsNum").toInt();
        model.studyForm = _query->value("studyForm").toInt();

        groups.append(model);
        SetupGroups();
    }
}

void GroupsWidget::InsertGroupToDb(GroupModel model)
{
    if(_query != nullptr){

        _query->prepare("INSERT INTO Groups VALUES(:id, :shortcut, :name, :isCombined, :year, :isWinterSemester, :studentsNum, :studyFrom)");
        _query->bindValue(0, ConvertUuidToString(model.id));
        _query->bindValue(1, model.shortcut);
        _query->bindValue(2, model.name);
        _query->bindValue(3, model.isCombined);
        _query->bindValue(4, model.year);
        _query->bindValue(5, model.isWinterSemester);
        _query->bindValue(6, model.studentsNum);
        _query->bindValue(7, model.studyForm);

        _query->exec();
    }
}

void GroupsWidget::DeleteGroupFromDb(QUuid id)
{
    if(_query != nullptr){
        _query->prepare("DELETE FROM Groups WHERE id=:id");
        _query->bindValue(0, ConvertUuidToString(id));
        _query->exec();

        _query->prepare("SELETE FROM Connections WHERE groupId=:id");
        _query->bindValue(0, ConvertUuidToString(id));
        _query->exec();
    }
}

void GroupsWidget::SetupAddingLayout()
{
    addingLayout->addWidget(new QLabel("Nový:"));

    QHBoxLayout* lrow = new QHBoxLayout();
    lrow->addWidget(new QLabel("Zkratka"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Název"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Typ"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Ročník"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Semestr"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Studium"), Qt::AlignCenter);
    lrow->addWidget(new QLabel("Počet studentů"), Qt::AlignCenter);
    lrow->addWidget(new QLabel(""), Qt::AlignCenter);

    addingLayout->addLayout(lrow);
    lrow = new QHBoxLayout();

    shortcut = new QLineEdit;
    lrow->addWidget(shortcut, Qt::AlignLeft | Qt::AlignTop);

    name = new QLineEdit;
    lrow->addWidget(name, Qt::AlignLeft | Qt::AlignTop);

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

    studentsNum = new QLineEdit;
    studentsNum->setValidator(new QIntValidator(1,1000));
    studentsNum->setText(QString("%1").arg(1));
    lrow->addWidget(studentsNum, Qt::AlignLeft | Qt::AlignTop);

    btnAdd = new QPushButton("Přidat");
    lrow->addWidget(btnAdd, Qt::AlignLeft | Qt::AlignTop);
    connect(btnAdd, &QPushButton::released, this, &GroupsWidget::AddGroup);

    addingLayout->addLayout(lrow);

    addingLayout->addWidget(new QLabel("Všechny skupiny:"));
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
            lrow->addWidget(new QLabel("Kombinovaná"));
        else
            lrow->addWidget(new QLabel("Prezenční"));

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

        if (!groups.at(i).isWinterSemester)
            lrow->addWidget(new QLabel("Letní"));
        else
            lrow->addWidget(new QLabel("Zimní"));

        QString formStr = "";
        switch (groups.at(i).studyForm) {
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

        QLineEdit* studentsNumEdit = new QLineEdit();
        studentsNumEdit->setFixedWidth(200);
        studentsNumEdit->setText(QString("%1").arg(groups.at(i).studentsNum));
        studentsNumEdit->setValidator(new QIntValidator(1,1000));
        studentsNumEdit->setProperty(NUM_PROPERTY,QVariant(i));
        connect(studentsNumEdit, &QLineEdit::textChanged, this, &GroupsWidget::ValueChanged);
        lrow->addWidget(studentsNumEdit);

        QPushButton* addSubjectsBtn = new QPushButton("Propojit s předměty");
        addSubjectsBtn->setProperty("row",i);
        addSubjectsBtn->setFixedWidth(140);
        connect(addSubjectsBtn, &QPushButton::pressed, this, &GroupsWidget::AddSubjects);
        lrow->addWidget(addSubjectsBtn);

        QPushButton* deleteBtn = new QPushButton("Smazat");
        deleteBtn->setProperty("row", i);
        deleteBtn->setFixedWidth(100);
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

    if (!canAdd)
        QMessageBox::warning(this, "Neplatná skupina",
            "Nevyplnil jste všechny údaje ke skupině!\nZkontrolujte název a zkratku!", QMessageBox::Close);
    else {
        GroupModel m;
        m.id = QUuid::createUuid();
        m.shortcut = shortcut->text();
        m.name = name->text();
        m.isCombined = ValidateStudyType(studyType->currentIndex());
        m.year = studyYear->currentIndex();
        m.isWinterSemester = ValidateSemester(semester->currentIndex());
        m.studentsNum = studentsNum->text().toInt();
        m.studyForm = studyForm->currentIndex();

        groups.append(m);

        InsertGroupToDb(m);
    }

    SetupGroupsLayout();
}

void GroupsWidget::DeleteGroup()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(QObject::sender());

    QUuid id = groups.at(btn->property("row").toInt()).id;
    DeleteGroupFromDb(id);

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

bool GroupsWidget::ValidateSemester(int s)
{
    if (s == Semester::Winter)
        return true;

    return false;
}

void GroupsWidget::ValueChanged(const QString& text)
{
    QLineEdit* sender = qobject_cast<QLineEdit*>(QObject::sender());

    if(sender != nullptr){
        int position = sender->property(NUM_PROPERTY).toInt();
        groups[position].studentsNum = text.toInt();

        if(_query != nullptr){
            _query->prepare("UPDATE Groups SET studentsNum=:studentsNum WHERE id=:id");
            _query->bindValue(0,text.toInt());
            _query->bindValue(1,ConvertUuidToString(groups[position].id));
            _query->exec();
        }
    }
}

void GroupsWidget::AddSubjects()
{
    QPushButton* sender = qobject_cast<QPushButton*>(QObject::sender());

    if(sender != nullptr){
        int pos = sender->property("row").toInt();

        if(_query != nullptr)
            connectingWidget->ShowWidget(groups[pos].id);
        else
            QMessageBox::warning(this,"Chyba!","Nelze zapisovat do databáze!",QMessageBox::Ok);
    }
}
