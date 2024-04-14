#include "Buff.h"

QHash<QString, BuffInfo> BuffSystem::InitBuff()
{
    QFile file(opt::buffJsonFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file " << opt::buffJsonFilePath;
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
        if (!obj.contains("className") ||
            !obj.contains("buffName") ||
            !obj.contains("description") ||
            !obj.contains("iconPath"))
        {
            qWarning() << "Warning: invalid json format - " << "a card doesn't have attribute - \"className\"";
        }
        const QString className = obj["className"].toString();
        const QString buffName = obj["buffName"].toString();
        const QString description = obj["description"].toString();
        const QString iconPath = obj["iconPath"].toString();

        qDebug() << "Successfully load a buff " << className << " -> " << iconPath;
        temp[className] = {className, buffName, description, iconPath};
    }
    return temp;
}
