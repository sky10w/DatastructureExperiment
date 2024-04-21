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
     }},
    {"restrict_action", [](iter_t cur, end_t end) -> res_t {
         cur++;
         if(cur == end) return {false, nullptr};
         const QString temp = *cur;
         return {true, new RestrictAction(Action::strToActType(temp))};
    }},
    {"unrestrict_action", [](iter_t cur, end_t end) -> res_t {
         cur++;
         if(cur == end) return {false, nullptr};
         const QString temp = *cur;
         return {true, new UnrestrictAction(Action::strToActType(temp))};
     }}
};
QList<Action *> ActionParser::parse(const QString &str)
{
    QList<Action*> resList;
    QVector<QString> temp = str.split(" ");
    // qDebug() << "The string list:" << temp;
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





const QHash<QString, Action::Act_t> Action::_act = {
    {"attack", Action::ATTACK},
    {"defend", Action::DEFEND},
    {"buff", Action::BUFF},
    {"action", Action::ACTION},
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

Action::Action(Act_t actType)
    : _type(actType)
{}

AttackAction::AttackAction(int damage)
    : Action(Action::ATTACK)
    , _damage(damage)
{}

void AttackAction::debug()
{
    qDebug() << "Acting attack" << this->_damage;
}

void AttackAction::act(Context *ctx)
{
    ctx->damageDone = this->_damage;
}

DefendAction::DefendAction(int armor)
    : Action(Action::DEFEND)
    , _armor(armor)
{}

void DefendAction::debug()
{
    qDebug() << "Acting defend"  << this->_armor;
}

void DefendAction::act(Context *ctx)
{
    ctx->armorGained = this->_armor;
}

GiveBuffAction::GiveBuffAction(const QString &buffID)
    : Action(Action::BUFF)
    , _buffID(buffID)
{}

void GiveBuffAction::debug()
{
    qDebug() << "Acting giveBuff" << this->_buffID;
}

void GiveBuffAction::act(Context *ctx)
{
    ctx->buffGiven += this->_buffID;
}

RemoveBuffAction::RemoveBuffAction(const QString &buffID)
    : Action(Action::BUFF)
    , _buffID(buffID)
{}

void RemoveBuffAction::debug()
{
    qDebug() << "Acting removeBuff" << this->_buffID;
}

void RemoveBuffAction::act(Context *ctx)
{
    ctx->buffRemoved += this->_buffID;
}

RestrictAction::RestrictAction(Act_t type)
    : Action(Action::ACTION)
    , _type(type)
{}

void RestrictAction::debug()
{
    qDebug() << "Acting restrict action";
}

void RestrictAction::act(Context *ctx)
{
    ctx->actAltered = this->_type;
}

UnrestrictAction::UnrestrictAction(Act_t type)
    : Action(Action::ACTION)
    , _type(type)
{}

void UnrestrictAction::debug()
{
    qDebug() << "Acting unrestrict action";
}

void UnrestrictAction::act(Context *ctx)
{
    ctx->actAltered = -1 * this->_type;
}
