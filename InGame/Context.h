#ifndef CONTEXT_H
#define CONTEXT_H

#include <QString>
#include <QVector>

class Entity;
class BasicBuff;

/// Context(from, vector(to), damageDone, armorGained, hpHealed, effectGiven)
struct Context
{
    Entity* from;
    QVector<Entity*> to;
    int damageDone;
    int armorGained;
    int hpHealed;
    int actAltered;
    QString buffGiven; // Using buffID
};

#endif // CONTEXT_H
