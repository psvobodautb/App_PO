#include "include/widgets/labelswidget.h"

#include <QSpacerItem>
#include <QVariant>
#include <QMessageBox>
#include <QVector>
#include <QHeaderView>
#include <QListWidgetItem>
#include "include/Functions.h"

#include <QSqlError>

static const char* SUBJECT_POSITION = "subject";
static const int LABEL = 1025;

LabelsWidget::LabelsWidget(QList<EmployeeModel>* e, QList<SubjectModel>* s, QList<GroupModel>* g, QSqlQuery* query, QWidget* parent) :
    QWidget(parent), employees(e), subjects(s), groups(g), detailWidget(nullptr), _query(query)
{
    widgetLayout = new QVBoxLayout();
    subjectsLayout = nullptr;
    groupBox = new QGroupBox("Předměty");
    unassigned = new QListWidget();
    assigned = new QListWidget();

    connect(unassigned, &QListWidget::itemDoubleClicked, this, &LabelsWidget::LabelClicked );
    connect(assigned, &QListWidget::itemDoubleClicked, this, &LabelsWidget::LabelClicked );

    connect(unassigned, &QListWidget::itemClicked, [this](){assigned->clearSelection(); });
    connect(assigned, &QListWidget::itemClicked, [this](){unassigned->clearSelection(); });

    QVBoxLayout* unassignedLayout = new QVBoxLayout();
    QVBoxLayout* assignedLayout = new QVBoxLayout();

    unassignedLayout->addWidget(new QLabel("Nepřiřazené štítky:"),Qt::AlignCenter);
    assignedLayout->addWidget(new QLabel("Přiřazené štítky:"),Qt::AlignCenter);

    unassignedLayout->addWidget(unassigned);
    assignedLayout->addWidget(assigned);

    unassignedLayout->setStretch(0,0);
    unassignedLayout->setStretch(1,1);

    assignedLayout->setStretch(0,0);
    assignedLayout->setStretch(1,1);

    buttonGroup = new QButtonGroup(this);

    secondRowLayout = new QHBoxLayout();
    secondRowLayout->addWidget(groupBox);
    secondRowLayout->addLayout(unassignedLayout);
    secondRowLayout->addLayout(assignedLayout);
    secondRowLayout->setStretch(0,3);
    secondRowLayout->setStretch(1,2);
    secondRowLayout->setStretch(2,2);

    widgetLayout->addLayout(secondRowLayout);

    setLayout(widgetLayout);
}

LabelsWidget::~LabelsWidget()
{
    delete detailWidget;
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
        connect(rb, &QRadioButton::clicked, this, &LabelsWidget::LoadCurrentSubjectLabels);
        rb->setProperty(SUBJECT_POSITION,i);
        buttonGroup->addButton(rb,i);
        subjectsLayout->addWidget(rb);
    }

    if(subjects->count() > 0)
        buttonGroup->button(0)->setChecked(true);

    QSpacerItem* spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    subjectsLayout->addItem(spacerItem);
}

void LabelsWidget::GenerateLabelsSlot(QUuid groupId, QUuid subjectId)
{
    GroupModel group;
    SubjectModel subject;

    for(int i = 0; i < groups->count(); i++){
        if(groups->at(i).id == groupId)
             group = groups->at(i);
    }

    for(int j = 0; j < subjects->count(); j++){
        if(subjects->at(j).id == subjectId)
            subject = subjects->at(j);
    }

    if(!group.id.isNull() && !subject.id.isNull())
        GenerateLabels(subject,group);
}

void LabelsWidget::GroupSizeChangedSlot(QUuid groupId)
{
    GroupModel group;

    for(int k = 0; k < groups->count(); k++){
        if(groups->at(k).id == groupId)
            group = groups->at(k);
    }

    QList<QUuid> ids;

    _query->prepare("Select Distinct subjectId from Labels where groupId=:groupId");
    _query->bindValue(0,ConvertUuidToString(groupId));
    _query->exec();

    while(_query->next()){
        ids.append(_query->value("subjectId").toUuid());
    }

    for(int i = 0; i < ids.count(); i++)
    {
        SubjectModel subject;

        for(int j = 0; j < subjects->count();j++){
            if(subjects->at(j).id == ids.at(i))
                subject = subjects->at(j);
        }

        UpdateLabels(subject, group);
    }
}

void LabelsWidget::SubjectSizeChangedSlot(QUuid subjectId)
{
    qDebug() << "subject size changed" << subjectId;

    SubjectModel subject;

    for(int k = 0; k < subjects->count(); k++){
        if(subjects->at(k).id == subjectId)
            subject = subjects->at(k);
    }

    QList<QUuid> ids;

    _query->prepare("Select Distinct groupId from Labels where subjectId=:subjectId");
    _query->bindValue(0,ConvertUuidToString(subjectId));
    _query->exec();

    while(_query->next()){
        ids.append(_query->value("groupId").toUuid());
    }

    for(int i = 0; i < ids.count(); i++)
    {
        GroupModel group;

        for(int j = 0; j < groups->count();j++){
            if(groups->at(j).id == ids.at(i))
               group = groups->at(j);
        }

        UpdateLabels(subject, group);
    }
}

void LabelsWidget::LoadCurrentSubjectLabels()
{
    if(buttonGroup->buttons().count() > 0)
        LoadLabelsFromDb(buttonGroup->checkedId());
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

void LabelsWidget::LoadLabelsFromDb(int id)
{
    currentLabels.clear();
    assigned->clear();
    unassigned->clear();

    int pos = buttonGroup->button(id)->property(SUBJECT_POSITION).toInt();

    _query->prepare("Select * from Labels Where subjectId=:subjectId");
    _query->bindValue(0, ConvertUuidToString(subjects->at(pos).id));
    _query->exec();

    while(_query->next()){
        LabelModel label;

        label.id = _query->value("id").toUuid();
        label.name = _query->value("name").toString();
        label.subjectId = _query->value("subjectId").toUuid();
        label.employeeId = _query->value("employeeId").toUuid();
        label.groupId = _query->value("groupId").toUuid();
        label.labelType = LabelType(_query->value("labelType").toInt());
        label.studentsNum = _query->value("studentsNum").toInt();
        label.lessonsNum = _query->value("lessonsNum").toInt();
        label.weeksNum = _query->value("weeksNum").toInt();
        label.language = Language(_query->value("language").toInt());
        label.points = _query->value("points").toDouble();
        label.isValid = _query->value("isValid").toBool();

        currentLabels.append(label);

        QListWidgetItem* item = new QListWidgetItem(label.name);
        item->setData(LABEL,label.id);

        if(!label.isValid){
            item->setBackground(QBrush("red"));
            item->setForeground(QBrush("black"));
        }
        else{
            if(label.labelType == LabelType::Lecture)
                item->setForeground(QBrush("yellow"));
            else if(label.labelType == LabelType::Excercise)
                item->setForeground(QBrush("orange"));
            else
                item->setForeground(QBrush("pink"));
        }

        if(label.employeeId.isNull())
            unassigned->addItem(item);
        else
            assigned->addItem(item);

    }
}

void LabelsWidget::RemoveLabelFromDb(QUuid labelId)
{
    _query->prepare("DELETE FROM Labels WHERE id=:labelId");
    _query->bindValue(0, ConvertUuidToString(labelId));
    _query->exec();

    LoadCurrentSubjectLabels();
}

void LabelsWidget::GenerateLabels(SubjectModel subject, GroupModel group)
{
    LabelModel lecture;

    // vygeneruje přednášku
    if(subject.lecturesNum > 0){
        lecture = CreateLectureLabel(subject,group);
    }

    QList<LabelModel> excercises;
    QList<LabelModel> seminars;

    if(subject.excercisesNum > 0)
        excercises = GenerateExcercises(subject,group);

    if(subject.seminarsNum > 0)
        seminars = GenerateSeminars(subject,group);

    if(lecture.isValid)
        InsertLectureToDd(lecture);

    for(int i = 0; i < excercises.count(); i++){
        if(excercises.at(i).isValid)
            InsertExcerciseToDb(excercises.at(i));
    }

    for(int j = 0; j < seminars.count(); j++){
        if(seminars.at(j).isValid)
            InsertSeminarToDb(seminars.at(j));
    }

    if(buttonGroup->buttons().count() > 0)
        LoadLabelsFromDb(buttonGroup->checkedId());
}

void LabelsWidget::UpdateLabels(SubjectModel subject, GroupModel group)
{
    QList<LabelModel> seminars;
    QList<LabelModel> excercises;
    _query->prepare("Select * from Labels Where subjectId=:subjectId");
    _query->bindValue(0, ConvertUuidToString(subject.id));
    _query->exec();

    while(_query->next()){
        LabelModel label;

        label.id = _query->value("id").toUuid();
        label.name = _query->value("name").toString();
        label.subjectId = _query->value("subjectId").toUuid();
        label.groupId = _query->value("groupId").toUuid();
        label.employeeId = _query->value("employeeId").toUuid();
        label.labelType = LabelType(_query->value("labelType").toInt());
        label.studentsNum = _query->value("studentsNum").toInt();
        label.lessonsNum = _query->value("lessonsNum").toInt();
        label.weeksNum = _query->value("weeksNum").toInt();
        label.language = Language(_query->value("language").toInt());
        label.points = _query->value("points").toDouble();
        label.isValid = _query->value("isValid").toBool();

        if(label.labelType == Excercise && label.isValid)
            excercises.append(label);
        else if(label.labelType == Seminar && label.isValid)
            seminars.append(label);

    }

    for(int sPos = 0; sPos < seminars.count(); sPos++){
        _query->prepare("Delete from Labels Where id=:id");
        _query->bindValue(0, ConvertUuidToString(seminars.at(sPos).id));
        _query->exec();
    }

    for(int ePos = 0; ePos < excercises.count(); ePos++){
        _query->prepare("Delete from Labels Where id=:id");
        _query->bindValue(0,ConvertUuidToString(excercises.at(ePos).id));
        _query->exec();
    }

    _query->prepare("Select groupSize from Subjects where id=:subjectId");
    _query->bindValue(0,ConvertUuidToString(subject.id));
    _query->exec();

    if(_query->next()){
        int groupSize = _query->value("groupSize").toInt();

        _query->prepare("Select studentsNum from Groups where id=:groupId");
        _query->bindValue(0,ConvertUuidToString(group.id));
        _query->exec();

        if(_query->next()){
            int studentsNum = _query->value("studentsNum").toInt();

            // spočítat kolik má být štítků

            double num = ((double)studentsNum / (double)groupSize);

            int parts = ceil(num);
            // přidat nebo označit některé za neplatné

            if(subject.excercisesNum > 0){
                if(parts > excercises.count()){
                    int numOfExcercises = parts;
                    while(excercises.count() < numOfExcercises){
                        LabelModel lm = excercises.last();
                        lm.employeeId = QUuid();
                        lm.id = QUuid::createUuid();
                        lm.name = QString("%1-%2-Cvičení-%3").arg(subject.shortcut).arg(group.shortcut).arg(excercises.count()+1);

                        excercises.append(lm);
                    }
                }
                else if(parts < excercises.count()){
                    int offset = excercises.count() - parts;
                    for(int n = 0; n < offset; n++){
                        excercises[parts + n].isValid = false;
                    }
                }

                // změnit počet studentů
                if(excercises.count() > 0){
                    if (studentsNum % parts == 0)
                    {
                        for(int ePos = 0; ePos < parts; ePos++){
                           LabelModel label = excercises.at(ePos);
                           label.studentsNum = studentsNum / parts;
                           excercises[ePos] = label;
                        }
                    }
                    else
                    {
                        int zp = parts - (studentsNum % parts);
                        int pp = studentsNum / parts;
                        for(int ePos = 0; ePos < parts ; ePos++)
                        {
                            if(ePos>= zp){
                                LabelModel label = excercises.at(ePos);
                                label.studentsNum = pp + 1;
                                excercises[ePos] = label;
                            }
                            else{
                                LabelModel label = excercises.at(ePos);
                                label.studentsNum = pp;
                                excercises[ePos] = label;
                            }
                        }
                    }
                }
            }

            if(subject.seminarsNum > 0){
                if(parts > seminars.count()){
                    int numOfSeminars = parts;
                    while(seminars.count() < numOfSeminars){

                        LabelModel lm = seminars.last();
                        lm.employeeId = QUuid();
                        lm.id = QUuid::createUuid();
                        lm.name = QString("%1-%2-Seminář-%3").arg(subject.shortcut).arg(group.shortcut).arg(seminars.count()+1);

                        seminars.append(lm);

                    }
                }
                else if(parts < seminars.count()){
                    int offset = seminars.count() - parts;
                    for(int n = 0; n < offset; n++){
                        seminars[parts + n].isValid = false;
                    }
                }

                if(seminars.count() > 0){
                    if (studentsNum % parts == 0)
                    {
                        for(int sPos = 0; sPos < parts; sPos++){
                           LabelModel label = seminars.at(sPos);
                           label.studentsNum = studentsNum / parts;
                           seminars[sPos] = label;
                        }
                    }
                    else
                    {
                        int zp = parts - (studentsNum % parts);
                        int pp = studentsNum / parts;
                        for(int sPos = 0; sPos < parts ; sPos++)
                        {
                            if(sPos>= zp){
                                LabelModel label = seminars.at(sPos);
                                label.studentsNum = pp + 1;
                                seminars[sPos] = label;
                            }
                            else{
                                LabelModel label = seminars.at(sPos);
                                label.studentsNum = pp;
                                seminars[sPos] = label;
                            }
                        }
                    }
                }
            }

            // update db

            qDebug() << "updating: "<< excercises.count() << seminars.count();

            for(int ePos = 0; ePos < excercises.count(); ePos++){
                if(excercises.at(ePos).isValid || !excercises.at(ePos).employeeId.isNull())
                    InsertExcerciseToDb(excercises.at(ePos));
            }

            for(int sPos = 0; sPos < seminars.count(); sPos++){
                if(seminars.at(sPos).isValid || !seminars.at(sPos).employeeId.isNull())
                    InsertSeminarToDb(seminars.at(sPos));
            }
        }
    }
}

QList<LabelModel> LabelsWidget::GenerateExcercises(SubjectModel subject, GroupModel group)
{
    QList<LabelModel> excercises;

    int studentsNum = group.studentsNum;
    int groupNum = 1;

    double num = (studentsNum / subject.groupSize) + 0.5;

    int parts = (int)round(num);

    if (studentsNum % parts == 0)
    {
        for(int i = 0; i < parts; i++){
            excercises.append(CreateExcerciseLabel(subject,group,groupNum,studentsNum / parts));
            groupNum++;
        }

    }
    else
    {
        int zp = parts - (studentsNum % parts);
        int pp = studentsNum / parts;
        for(int i = 0; i < parts ; i++)
        {
            if(i>= zp)
                excercises.append(CreateExcerciseLabel(subject,group,groupNum,pp + 1));
            else
                excercises.append(CreateExcerciseLabel(subject,group,groupNum,pp));

            groupNum++;
        }
    }

    return excercises;
}

QList<LabelModel> LabelsWidget::GenerateSeminars(SubjectModel subject, GroupModel group)
{
    QList<LabelModel> seminars;

    int studentsNum = group.studentsNum;
    int groupNum = 1;

    double num = (studentsNum / subject.groupSize) + 0.5;

    int parts = (int)round(num);

    if (studentsNum % parts == 0)
    {
        for(int i = 0; i < parts; i++){
            seminars.append(CreateSeminarLabel(subject,group,groupNum,studentsNum / parts));
            groupNum++;
        }

    }
    else
    {
        int zp = parts - (studentsNum % parts);
        int pp = studentsNum / parts;
        for(int i = 0; i < parts ; i++)
        {

            if(i>= zp)
                seminars.append(CreateSeminarLabel(subject,group,groupNum,pp + 1));
            else
                seminars.append(CreateSeminarLabel(subject,group,groupNum,pp));

            groupNum++;
        }
    }

    return seminars;
}

LabelModel LabelsWidget::CreateLectureLabel(SubjectModel subject, GroupModel group)
{
    QString labelName = QString("%1-%2-Přednáška").arg(subject.shortcut).arg(group.shortcut);

    LabelModel label;
    label.id = QUuid::createUuid();
    label.name = labelName;
    label.subjectId = subject.id;
    label.groupId = group.id;
    label.labelType = LabelType::Lecture;
    label.studentsNum = group.studentsNum;
    label.lessonsNum = subject.lecturesNum;
    label.weeksNum = subject.weeksNum;

    if(subject.isEnglish){
        label.language = Language::English;
        label.points = 2.4;
    }
    else{
        label.language = Language::Czech;
        label.points = 1.8;
    }

    label.isValid = true;

    return label;
}

LabelModel LabelsWidget::CreateSeminarLabel(SubjectModel subject, GroupModel group, int groupNum, int groupSize)
{
    QString labelName = QString("%1-%2-Seminář-%3").arg(subject.shortcut).arg(group.shortcut).arg(groupNum);

    LabelModel label;
    label.id = QUuid::createUuid();
    label.name = labelName;
    label.subjectId = subject.id;
    label.groupId = group.id;
    label.labelType = LabelType::Seminar;
    label.studentsNum = groupSize;
    label.lessonsNum = subject.excercisesNum;
    label.weeksNum = subject.weeksNum;

    if(subject.isEnglish){
        label.language = Language::English;
        label.points = 1.8;
    }
    else{
        label.language = Language::Czech;
        label.points = 1.2;
    }

    label.isValid = true;

    return label;
}

LabelModel LabelsWidget::CreateExcerciseLabel(SubjectModel subject, GroupModel group, int groupNum, int groupSize)
{
    QString labelName = QString("%1-%2-Cvičení-%3").arg(subject.shortcut).arg(group.shortcut).arg(groupNum);

    LabelModel label;
    label.id = QUuid::createUuid();
    label.name = labelName;
    label.subjectId = subject.id;
    label.groupId = group.id;
    label.labelType = LabelType::Excercise;
    label.studentsNum = groupSize;
    label.lessonsNum = subject.excercisesNum;
    label.weeksNum = subject.weeksNum;

    if(subject.isEnglish){
        label.language = Language::English;
        label.points = 1.8;
    }
    else{
        label.language = Language::Czech;
        label.points = 1.2;
    }

    label.isValid = true;

    return label;
}

void LabelsWidget::InsertLectureToDd(LabelModel lecture)
{
    _query->prepare("INSERT INTO Labels VALUES(:id, :name, :subjectId, :employeeId, :groupId, :labelType, :studentsNum, :lessonsNum, :weeksNum, :language, :points, :isValid)");
    _query->bindValue(0, ConvertUuidToString(lecture.id));
    _query->bindValue(1, lecture.name);
    _query->bindValue(2, ConvertUuidToString(lecture.subjectId));

    if(lecture.employeeId.isNull())
        _query->bindValue(3, "NULL");
    else
        _query->bindValue(3, ConvertUuidToString(lecture.employeeId));

    _query->bindValue(4, ConvertUuidToString(lecture.groupId));
    _query->bindValue(5, lecture.labelType);
    _query->bindValue(6, lecture.studentsNum);
    _query->bindValue(7, lecture.lessonsNum);
    _query->bindValue(8, lecture.weeksNum);
    _query->bindValue(9, lecture.language);
    _query->bindValue(10, lecture.points);
    _query->bindValue(11, lecture.isValid);
    _query->exec();
}

void LabelsWidget::InsertSeminarToDb(LabelModel seminar)
{
    _query->prepare("INSERT INTO Labels VALUES(:id, :name, :subjectId, :employeeId, :groupId, :labelType, :studentsNum, :lessonsNum, :weeksNum, :language, :points, :isValid)");
    _query->bindValue(0, ConvertUuidToString(seminar.id));
    _query->bindValue(1, seminar.name);
    _query->bindValue(2, ConvertUuidToString(seminar.subjectId));

    if(seminar.employeeId.isNull())
        _query->bindValue(3, "NULL");
    else
        _query->bindValue(3, ConvertUuidToString(seminar.employeeId));

    _query->bindValue(4, ConvertUuidToString(seminar.groupId));
    _query->bindValue(5, seminar.labelType);
    _query->bindValue(6, seminar.studentsNum);
    _query->bindValue(7, seminar.lessonsNum);
    _query->bindValue(8, seminar.weeksNum);
    _query->bindValue(9, seminar.language);
    _query->bindValue(10, seminar.points);
    _query->bindValue(11, seminar.isValid);
    _query->exec();
}

void LabelsWidget::InsertExcerciseToDb(LabelModel excercise)
{
    _query->prepare("INSERT INTO Labels VALUES(:id, :name, :subjectId, :employeeId, :groupId, :labelType, :studentsNum, :lessonsNum, :weeksNum, :language, :points, :isValid)");
    _query->bindValue(0, ConvertUuidToString(excercise.id));
    _query->bindValue(1, excercise.name);
    _query->bindValue(2, ConvertUuidToString(excercise.subjectId));

    if(excercise.employeeId.isNull())
        _query->bindValue(3, "NULL");
    else
        _query->bindValue(3, ConvertUuidToString(excercise.employeeId));

    _query->bindValue(4, ConvertUuidToString(excercise.groupId));
    _query->bindValue(5, excercise.labelType);
    _query->bindValue(6, excercise.studentsNum);
    _query->bindValue(7, excercise.lessonsNum);
    _query->bindValue(8, excercise.weeksNum);
    _query->bindValue(9, excercise.language);
    _query->bindValue(10, excercise.points);
    _query->bindValue(11, excercise.isValid);
    _query->exec();
}

void LabelsWidget::UpdateEmployee(QString labelId, QString employeeId)
{
    _query->prepare("UPDATE Labels SET employeeId=:employeeId WHERE id=:id");
    _query->bindValue(0,employeeId);
    _query->bindValue(1,labelId);
    _query->exec();

    LoadCurrentSubjectLabels();
}

void LabelsWidget::LabelClicked(QListWidgetItem *item)
{
    QUuid id = item->data(LABEL).toUuid();

    if(detailWidget == nullptr){
        detailWidget = new LabelDetailWidget(employees,subjects);
        connect(this, &LabelsWidget::UpdateDetail, detailWidget, &LabelDetailWidget::Update);
        connect(detailWidget, &LabelDetailWidget::UpdateEmployeeInDb, this, &LabelsWidget::UpdateEmployee);
        connect(detailWidget, &LabelDetailWidget::DeleteLabel, this,&LabelsWidget::RemoveLabelFromDb);
    }

    LabelModel m;

    for(int i = 0; i < currentLabels.count(); i++){
        if(currentLabels.at(i).id == id)
            m = currentLabels.at(i);
    }

    if(!m.id.isNull())
        detailWidget->ShowWidget(m);
}
