#pragma once
#include <QObject>
#include <QFile>
#include <QString>
#include <QStack>
#include <QHash>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QEvent>

#include "ActionParser.h"

#include "InGame/globalOptions.h"

/// CardInfo(id, name, description, path, targetType, energy, QList<Action*> action)
struct CardInfo
{
    enum targetType_t : int {
        ONE,
        ALL,
        RANDOM,
        SELF,

        NOT_FOUND
    };
	QString id;
	QString name;
	QString description;
	QString path;
    targetType_t targetType;
    Action::Act_t actType;
	int energy;
    QList<Action*> action;

    bool operator==(const CardInfo& rhs) const {return this->id == rhs.id;}
};


// const QVector<QString> PlayerOwnedCards = {"01ef", "13jd", "1i92"};
class CardStack
{
public:
    CardStack() = default;
    CardStack(const QVector<QString>& cards);
	QString getPopOne();
	QVector<QString> getPopN(int n);
	QVector<QString> getPopAll();
    QVector<QString> getAll() const;
	void push(const QVector<QString>& cards);
    void clear();
	bool empty() const;
    int size() const;

private:
	QVector<int> utilGetRandomNum(int n);
	std::list<QString> _list;
};
