#ifndef BUFFPARSER_H
#define BUFFPARSER_H

#include <QDebug>
#include "InGame/Context.h"
#include "Buff.h"


class BuffParser
{
public:
    using res_t = QPair<bool, BasicBuff*>;
    using iter_t = QVector<QString>::iterator&;
    using end_t = const QVector<QString>::iterator;

    static BasicBuff* parse(const QString& str);
private:
    BuffParser() = delete;
    static const QHash<QString, std::function<res_t(iter_t, end_t)>> Functions;
    static const QHash<QString, BuffInfo::BuffType> _map;
    static BuffInfo::BuffType strToType(const QString &str);
};

#endif // BUFFPARSER_H
