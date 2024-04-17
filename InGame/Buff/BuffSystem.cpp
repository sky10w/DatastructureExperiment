#include "BuffSystem.h"


QHash<QString, BuffInfo> BuffSystem::_buff;

BuffInfo BuffSystem::getBuffInfo(const QString &buffID)
{
    qDebug() << "Getting buffInfo" << buffID;
    for(auto& i : BuffSystem::_buff)
    {
        qDebug() << i.id << i.buffName;
    }
    if(!BuffSystem::_buff.contains(buffID))
    {
        qFatal("Error: no existing buff - id: %s", buffID.toLatin1().data());
        return BuffInfo{"-1"};
    }
    return _buff[buffID];
}

QHash<QString, BuffInfo> BuffSystem::initBuff()
{
    QFile file(opt::buffJsonFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file " << opt::buffJsonFilePath;
        return {};
    }
    QByteArray data = file.readAll();
    // qDebug() << data;
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() << "Error when parse to jsonDocument " << err.errorString();
        return {};
    }

    if (!doc.isArray())
    {
        qWarning() << "Warning: Failed to parse the json into an json arry";
        return {};
    }

    QHash<QString, BuffInfo> temp;
    QJsonArray array = doc.array();
    for (auto i : array)
    {
        if (!i.isObject())
        {
            qWarning() << "Warning: invalid json format";
            return {};
        }
        QJsonObject obj = i.toObject();
        if (!obj.contains("id") ||
            !obj.contains("class_name") ||
            !obj.contains("buff_name") ||
            !obj.contains("description") ||
            !obj.contains("type") ||
            !obj.contains("icon_path"))
        {
            qWarning() << "Warning: invalid json format";
        }
        const QString id = obj["id"].toString();
        const QString className = obj["class_name"].toString();
        const QString buffName = obj["buff_name"].toString();
        const QString description = obj["description"].toString();
        const QString iconPath = obj["iconPath"].toString();
        const QString typeStr = obj["type"].toString();
        const auto type = BuffSystem::strToTargetType(typeStr);
        if(type == BuffInfo::NOT_FOUND)
        {
            qFatal("Error when parsing the buffID: %s -> type error", id.toLatin1().data());
            return {};
        }

        qDebug() << "Parsing buffID" << id << "class_name" << className;
        auto res = BuffParser::parse(className);
        if(res == nullptr)
        {
            qFatal("Error when parsing the buffID: %s -> parse error", id.toLatin1().data());
            return {};
        }
        res->setID(id);

        qDebug() << "Successfully load a buff" << className << "->" << iconPath << '\n';
        temp[id] = BuffInfo{id, className, buffName, description, type, iconPath, res};
    }
    BuffSystem::_buff = temp;
    for(auto &i : _buff)
    {
        qDebug() << i.id << i.buffName;
    }

    return temp;
}

const QHash<QString, BuffInfo::BuffType> BuffSystem::_map = {
    {"ON_ROUNDBEGIN",BuffInfo::ON_ROUNDBEGIN},
    {"ON_ROUNDEND",BuffInfo::ON_ROUNDEND},
    {"ON_GIVEBUFF",BuffInfo::ON_GIVEBUFF},
    {"ON_GETBUFFED",BuffInfo::ON_GETBUFFED},
    {"ON_ATTACK",BuffInfo::ON_ATTACK},
    {"ON_GAINARMOR",BuffInfo::ON_GAINARMOR},
    {"ON_HURT",BuffInfo::ON_HURT}
};

BuffInfo::BuffType BuffSystem::strToTargetType(const QString &type)
{
    if(!_map.contains(type)) return BuffInfo::NOT_FOUND;
    return BuffSystem::_map[type];
}
