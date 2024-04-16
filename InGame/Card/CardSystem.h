#ifndef CARDSYSTEM_H
#define CARDSYSTEM_H

#include "GameCard.h"

class CardSystem
{
public:
    static CardInfo getCardInfo(const QString& cardID);
    static QHash<QString, CardInfo> initCard();
private:
    static const QHash<QString, CardInfo::targetType_t> _map;
    static CardInfo::targetType_t strToTargetType(const QString& type);
    static QHash<QString, CardInfo> _cards;

};

#endif // CARDSYSTEM_H
