#ifndef CONNECTINGWIDGET_H
#define CONNECTINGWIDGET_H

#include <QListWidget>
#include <QSqlQuery>
#include <QUuid>

class ConnectingWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ConnectingWidget(QSqlQuery *query, QWidget *parent = nullptr);

    void ShowWidget(QUuid groupId);

signals:

private:
    void OnItemChange(QListWidgetItem* item);
    void RemoveConnectionFromDb(QUuid subjectId);
    void AddConnectionToDb(QUuid subjectId);

    QSqlQuery* _query;
    QUuid _groupId;
};

#endif // CONNECTINGWIDGET_H
