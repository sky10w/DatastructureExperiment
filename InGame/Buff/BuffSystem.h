#ifndef BUFFSYSTEM_H
#define BUFFSYSTEM_H

#include "BuffParser.h"

struct BuffSystem
{
    static BuffInfo getBuffInfo(const QString &buffID);
    static QHash<QString, BuffInfo> initBuff();
private:
    BuffSystem() = delete;
    static const QHash<QString, BuffInfo::BuffType> _map;
    static BuffInfo::BuffType strToTargetType(const QString& type);
    static QHash<QString, BuffInfo> _buff;
};

#endif // BUFFSYSTEM_H
