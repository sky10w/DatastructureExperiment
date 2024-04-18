#ifndef BUFFSYSTEM_H
#define BUFFSYSTEM_H

#include "BuffParser.h"

class BuffSystem
{
public:
    static BuffInfo getBuffInfo(const QString &buffID);
    static QHash<QString, BuffInfo> initBuff();
    static BuffInfo::BuffType strToTargetType(const QString& type);
private:
    BuffSystem() = delete;
    static const QHash<QString, BuffInfo::BuffType> _map;

    static QHash<QString, BuffInfo> _buff;
};

#endif // BUFFSYSTEM_H
