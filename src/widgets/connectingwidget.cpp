#include "include/widgets/connectingwidget.h"

#include <QHBoxLayout>

#include "include/Functions.h"

const int ID_ROLE = 1001;

ConnectingWidget::ConnectingWidget(QSqlQuery *query,QWidget *parent) : QListWidget(parent)
{
    setLayout(new QHBoxLayout);
    setBaseSize(200,200);

    _query = query;

    connect(this,&QListWidget::itemChanged,this,&ConnectingWidget::OnItemChange);
}

void ConnectingWidget::ShowWidget(QUuid groupId)
{
    clear();

    show();
    raise();

    _groupId = groupId;

    _query->prepare("Select id, name from Subjects");
    _query->exec();

    while(_query->next()){
        QUuid id = _query->value("id").toUuid();
        QString name = _query->value("name").toString();
        QListWidgetItem* item = new QListWidgetItem(name);
        item->setData(ID_ROLE,ConvertUuidToString(id));
        item->setCheckState(Qt::Unchecked);
        addItem(item);
    }

    _query->prepare("Select * from Connections where groupId=:groupId");
    _query->bindValue(0,ConvertUuidToString(groupId));
    _query->exec();

    blockSignals(true);

    while(_query->next()){
        for(int i = 0; i < count();i++){
            if(item(i)->data(ID_ROLE).toString() == _query->value("subjectId").toString()){
                item(i)->setCheckState(Qt::Checked);
            }
        }
    }

     blockSignals(false);
}

void ConnectingWidget::OnItemChange(QListWidgetItem *item)
{
    QUuid subjectId = item->data(ID_ROLE).toUuid();

    if(item->checkState() == Qt::Checked){
         AddConnectionToDb(subjectId);
    }
    else{
        RemoveConnectionFromDb(subjectId);
    }
}

void ConnectingWidget::RemoveConnectionFromDb(QUuid subjectId)
{
    _query->prepare("Delete from Connections Where subjectId=:subjectId And groupId=:groupId");
    _query->bindValue(0,ConvertUuidToString(subjectId));
    _query->bindValue(1,ConvertUuidToString(_groupId));
    _query->exec();
}

void ConnectingWidget::AddConnectionToDb(QUuid subjectId)
{
    _query->prepare("Insert into Connections Values(:groupId,:subjectId)");
    _query->bindValue(0,ConvertUuidToString(_groupId));
    _query->bindValue(1,ConvertUuidToString(subjectId));
    _query->exec();
}