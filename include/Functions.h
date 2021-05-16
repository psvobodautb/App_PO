#pragma once

#include <QString>
#include <QUuid>

static QString ConvertUuidToString(QUuid id)
{
    QString str = id.toString();
    str = str.left(str.count() - 1);
    str = str.right(str.count() - 1);

    return str;
}
