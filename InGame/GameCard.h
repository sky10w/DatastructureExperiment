#pragma once
#include <QtWidgets/qwidget.h>
#include <qfile.h>
#include <qstring.h>
#include <qstack.h>
#include <qhash.h>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <QEvent>

#include "InGame/globalOptions.h"

struct CardInfo{
public:
	QString id;
	QString name;
	QString description;
	QString path;
	int targetType;
	int energy;
    QString action;
public:
    CardInfo() = default;
	bool operator==(const CardInfo& rhs) const {return this->id == rhs.id;}
	friend size_t qHash(CardInfo k, size_t seed);
    friend size_t qHash(const CardInfo& k, size_t seed);
    void setPath(const QString &newPath);
};
size_t qHash(CardInfo k, size_t seed);
size_t qHash(const CardInfo& k, size_t seed);

namespace CardSystem
{
    QHash<QString, CardInfo> InitCards();
    const QHash<QString, CardInfo> cards = InitCards();
};

// const QVector<QString> PlayerOwnedCards = {"01ef", "13jd", "1i92"};
class CardStack : public QObject
{
	Q_OBJECT
public:
	CardStack() = default;
	CardStack(const QVector<QString>& cards);
	QString getPopOne();
	QVector<QString> getPopN(int n);
	QVector<QString> getPopAll();
    QVector<QString> getAll() const;
	void push(const QVector<QString>& cards);
	bool empty() const;

private:
	QVector<int> utilGetRandomNum(int n);
	std::list<QString> _list;
};
