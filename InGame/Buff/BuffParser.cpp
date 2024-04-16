#include "BuffParser.h"

const QHash<QString, BuffInfo::BuffType> BuffParser::_map = {
    {"ON_ROUNDBEGIN", BuffInfo::ON_ROUNDBEGIN},
    {"ON_ROUNDEND", BuffInfo::ON_ROUNDEND},
    {"ON_GIVEBUFF", BuffInfo::ON_GIVEBUFF},
    {"ON_GETBUFFED", BuffInfo::ON_GETBUFFED},
    {"ON_ATTACK", BuffInfo::ON_ATTACK},
    {"ON_GAINARMOR", BuffInfo::ON_GAINARMOR},
    {"ON_HURT", BuffInfo::ON_HURT}
};

const QHash<QString, std::function<BuffParser::res_t(BuffParser::iter_t, BuffParser::end_t)>> BuffParser::Functions = {
    {"ModifyDamageByNumberBuff", [](iter_t iter, end_t end) -> res_t {
            ++iter;
            if(iter == end) return {false, nullptr};
            const QString typeName = *iter;
            const BuffInfo::BuffType type = BuffParser::strToType(typeName);
            if (type == BuffInfo::NOT_FOUND) return {false, nullptr};

            ++iter;
            if(iter == end) return {false, nullptr};
            const QString damageStr = *iter;
            bool ok;
            const int damage = damageStr.toInt(&ok);
            if(!ok) return {false, nullptr};

            ++iter;
            if(iter == end) return {false, nullptr};
            if(*iter != "end") return {false, nullptr};

            auto buff = new ModifyDamageByNumberBuff(type, damage);
            return {true, buff};
     }
    },
};

BasicBuff* BuffParser::parse(const QString &str)
{
    QList<BasicBuff*> resList;
    auto temp = str.split(" ");
    qDebug() << "The string list:" << temp;

    auto iter = temp.begin();
    const QString cur = *iter;
    if(!Functions.contains(cur))
    {
        qWarning() << "Invalid expression: keyword missing \"" << cur << "\"";
        return nullptr;
    }
    const auto res = BuffParser::Functions[cur](iter, temp.end());
    if(res.first == false)
    {
        qWarning() << "Invalid expression: parsing error";
        return nullptr;
    }

    return res.second;
}

BuffInfo::BuffType BuffParser::strToType(const QString &str)
{
    if(!_map.contains(str)) return BuffInfo::NOT_FOUND;
    return _map[str];
}





