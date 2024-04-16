#include "ActionParser.h"


// const QSet<QString> ActionParser::KeyWords = {"attack", "defend", "giveBuff"};
const QHash<QString, std::function<ActionParser::res_t(ActionParser::iter_t, ActionParser::end_t)>> ActionParser::Functions = {
    {"attack", [](iter_t cur, end_t end) -> res_t {
         cur++;
         if(cur == end) return {false, nullptr };
         const QString temp = *cur;
         bool ok = false;
         const int res = temp.toInt(&ok);
         if(!ok) return {false, nullptr };
         return {true, new AttackAction(res)};
     }},
    {"defend", [](iter_t cur, end_t end) -> res_t {
         cur++;
         if(cur == end) return {false, nullptr };
         const QString temp = *cur;
         bool ok = false;
         const int res = temp.toInt(&ok);
         if(!ok) return {false, nullptr };
         return {true, new DefendAction(res)};
     }},
    {"give_buff",[](iter_t cur, end_t end) -> res_t {
         cur++;
         if(cur == end) return {false, nullptr };
         const QString temp = *cur;
         return {true, new GiveBuffAction(temp)};
     }},
    {"remove_buff",[](iter_t cur, end_t end) -> res_t {
         cur++;
         if(cur == end) return {false, nullptr};
         const QString temp = *cur;
         return {true, new RemoveBuffAction(temp)};
     }}
};
QList<Action *> ActionParser::parse(const QString &str)
{
    QList<Action*> resList;
    QVector<QString> temp = str.split(" ");
    qDebug() << "The string list:" << temp;
    int cnter = 0;
    for(auto iter = temp.begin(); iter != temp.end(); iter++)
    {
        const QString cur = *iter;
        if(!Functions.contains(cur))
        {
            qWarning() << "Invalid expression at" << cnter << ": keyword missing \"" << cur << "\"";
            return {};
        }
        const auto res = ActionParser::Functions[cur](iter, temp.end());
        if(res.first == false)
        {
            qWarning() << "Invalid expression at" << cnter << ": parsing error";
            return {};
        }
        resList.push_back(res.second);
        cnter++;
    }
    return resList;
}


AttackAction::AttackAction(int damage)
    : Action(Action::ATTACK)
{
    this->_ctx->damageDone = damage;
}

void AttackAction::act()
{
    qDebug() << "Acting attack" << this->_ctx->damageDone;
}

DefendAction::DefendAction(int armor)
    : Action(Action::DEFEND)
{
    this->_ctx->armorGained = armor;
}

void DefendAction::act()
{
    qDebug() << "Acting defend"  << this->_ctx->armorGained;
}

GiveBuffAction::GiveBuffAction(const QString &buffName)
    : Action(Action::BUFF)
{
    this->_ctx->buffGiven = buffName;
}

void GiveBuffAction::act()
{
    qDebug() << "Acting giveBuff" << this->_ctx->buffGiven;
}

RemoveBuffAction::RemoveBuffAction(const QString &buffID)
    : Action(Action::BUFF)
{}


const QHash<QString, Action::Act_t> Action::_act = {
    {"attack", Action::ATTACK},
    {"defend", Action::DEFEND},
    {"buff", Action::BUFF}
};

Action::Act_t Action::strToActType(const QString &str)
{
    if(!Action::_act.contains(str)) return Action::NOT_FOUND;
    return Action::_act[str];
}

Action::Act_t Action::getType() const
{
    return this->_type;
}

Context *Action::getContex() const
{
    return this->_ctx;
}

Action::Action(Act_t actType)
    : _type(actType)
    , _ctx(new Context())
{}


