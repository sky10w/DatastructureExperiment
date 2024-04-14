#ifndef BUFFH_H
#define BUFFH_H

#include <QHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include "globalOptions.h"
#include "Basic.h"

class BuffInfo
{
public:
    QString className;
    QString buffName;
    QString description;
    QString iconPath;
};

namespace BuffSystem
{
    QHash<QString, BuffInfo> InitBuff();
    const QHash<QString, BuffInfo> buff = InitBuff();
};

#endif // BUFFH_H
