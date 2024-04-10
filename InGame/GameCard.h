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
#include <qevent.h>
#include <qeventpoint.h>

#include "InGame/styles.h"
#include "InGame/globalOptions.h"

class CardInfo{
public:
	QString id;
	QString name;
	QString description;
	QString path;
	int targetType;
	int energy;
public:
	CardInfo() = default;
	CardInfo(const QString id, const QString name, const QString description, const QString path, int targetType, int energy);
	bool operator==(const CardInfo& rhs) const {return this->id == rhs.id;}
	friend size_t qHash(CardInfo k, size_t seed);
	friend size_t qHash(const CardInfo& k, size_t seed);
};
size_t qHash(CardInfo k, size_t seed);
size_t qHash(const CardInfo& k, size_t seed);

class CardSystem : public QObject
{
	Q_OBJECT
public:
	static QHash<QString, CardInfo> cards;
	static QHash<QString, CardInfo> InitCards();
private:
	static CardSystem CARDS;
	CardSystem();
};

const QVector<QString> PlayerOwnedCards = {"01ef", "13jd", "1i92"};
class CardStack : public QObject
{
	Q_OBJECT
public:
	CardStack() = default;
	CardStack(const QVector<QString>& cards);
	QString getPopOne();
	QVector<QString> getPopN(int n);
	QVector<QString> getPopAll();
	void push(const QVector<QString>& cards);
	bool empty() const;

private:
	QVector<int> utilGetRandomNum(int n);
	std::list<QString> _list;
};

class CardView : public QWidget
{
	Q_OBJECT
public:
	CardView(const QString id, QWidget* parent = nullptr);
private:
	QString _id;
	bool _pressed;
	QPoint lastPoint;
	QPoint initPoint;
protected:
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
};

