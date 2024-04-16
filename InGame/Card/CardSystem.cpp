#include "CardSystem.h"


QHash<QString, CardInfo> CardSystem::_cards;

QHash<QString, CardInfo> CardSystem::initCard()
{
    QFile file(opt::cardJsonFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file " << opt::cardJsonFilePath;
        return {};
    }
    QByteArray data = file.readAll();
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

    QHash<QString, CardInfo> temp;
    QJsonArray array = doc.array();
    for (auto i : array)
    {
        if (!i.isObject())
        {
            qWarning() << "Warning: invalid json format";
            return {};
        }
        QJsonObject obj = i.toObject();
        if (!obj.contains("uuid") ||
            !obj.contains("name") ||
            !obj.contains("description") ||
            !obj.contains("path") ||
            !obj.contains("act_type") ||
            !obj.contains("target_type") ||
            !obj.contains("energy") ||
            !obj.contains("action"))
        {
            qWarning() << "Warning: invalid json format - " << "a card doesn't have attribute";
        }
        CardInfo tempInfo;
        tempInfo.id = obj["uuid"].toString();
        tempInfo.name = obj["name"].toString();
        tempInfo.description = obj["description"].toString();
        tempInfo.path = obj["path"].toString();
        tempInfo.energy = obj["energy"].toInt();
        const QString actStr = obj["action"].toString();
        auto res = ActionParser::parse(actStr);
        if(res.size() == 0)
        {
            qFatal("Error when parsing the actionID: %s", tempInfo.id.toLatin1().data());
        }


        auto actType = Action::strToActType(obj["act_type"].toString());
        if(actType == Action::NOT_FOUND)
        {
            qFatal("Act type %s doesn\'t exist.", obj["act_type"].toString().toLatin1().data());
        }
        tempInfo.actType = actType;

        // specified target_type
        auto targetType = CardSystem::strToTargetType(obj["target_type"].toString());
        if(targetType == CardInfo::NOT_FOUND)
        {
            qFatal("Target type %s doesn\'t exist.", obj["target_type"].toString().toLatin1().data());
        }
        tempInfo.targetType = targetType;

        qDebug() << "Successfully load a card " << tempInfo.id << " -> " << tempInfo.path;
        temp[tempInfo.id] = tempInfo;
    }
    CardSystem::_cards = temp;
    return temp;
}

const QHash<QString, CardInfo::targetType_t> CardSystem::_map = {
    {"ALL", CardInfo::ALL},
    {"ONE", CardInfo::ONE},
    {"RANDOM", CardInfo::RANDOM},
    {"SELF", CardInfo::SELF},
};

CardInfo::targetType_t CardSystem::strToTargetType(const QString &type)
{
    if(!_map.contains(type)) return CardInfo::NOT_FOUND;
    return CardSystem::_map[type];
}

CardInfo CardSystem::getCardInfo(const QString &cardID)
{
    if(!CardSystem::_cards.contains(cardID))
    {
        qFatal("The cardID %s doesn't exist");
        return CardInfo{"-1"};
    }
    return CardSystem::_cards[cardID];
}

