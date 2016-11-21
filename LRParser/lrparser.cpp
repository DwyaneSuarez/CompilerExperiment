#include "lrparser.h"
#include <QStack>
#include <QQueue>

LRParser::LRParser()
{
    actionmap.clear();
    gotomap.clear();
    rules.clear();
    DFA.clear();
    amapsize = gmapsize = 0;
    /*
    addRules('S', "S");
    addRules('S', "L,S");
    addRules('S', "L");
    addRules('L', "x");
    addRules('L', "y");

    setActionMap('x', 0);
    setActionMap('y', 1);
    setActionMap(',', 2);
    setActionMap('#', 3);

    setGotoMap('S', 0);
    setGotoMap('L', 1);

    setActionTable(0, 'x', "S", 3);
    setActionTable(0, 'y', "S", 4);
    setActionTable(1, '#', "acc", -1);
    setActionTable(2, ',', "S", 5);
    setActionTable(2, '#', "r", 2);
    setActionTable(3, ',', "r", 3);
    setActionTable(3, '#', "r", 3);
    setActionTable(4, ',', "r", 4);
    setActionTable(4, '#', "r", 4);
    setActionTable(5, 'x', "S", 3);
    setActionTable(5, 'y', "S", 4);
    setActionTable(6, '#', "r", 1);

    setGotoTable(0, 'S', 1);
    setGotoTable(0, 'L', 2);
    setGotoTable(5, 'S', 6);
    setGotoTable(5, 'L', 2);
    */
}

void LRParser::setRules(QStringList list)
{
    addRules('$', list[0].mid(0, 1));
    firstT = list[0][0];
    for (int i = 0; i < list.size(); i++)
    {
        QChar ch = list[i][0];
        if (ch >= 'A' && ch <= 'Z')
        {
            if (!gotomap.contains(ch))
            {
                gotomap[ch] = gmapsize;
                gmapsize++;
            }
        }
        for (int j = 3; j < list[i].length(); j++)
            if (list[i][j] >= 'a' && list[i][j] <= 'z' && !actionmap.contains(list[i][j]))
            {
                actionmap[list[i][j]] = amapsize;
                amapsize++;
            }
        actionmap['#'] = amapsize;
        QString str = list[i].mid(3);
        addRules(ch, str);
    }
}

void LRParser::addRules(QChar ch, QString str)
{
    Rules rule(ch, str);
    rules.push_back(rule);
}

void LRParser::setActionMap(QChar ch, int num)
{
    actionmap[ch] = num;
}

void LRParser::setGotoMap(QChar ch, int num)
{
    gotomap[ch] = num;
}

void LRParser::setActionTable(int state, QChar ch, QString type, int num)
{
    Action[state][actionmap[ch]].type = type;
    Action[state][actionmap[ch]].num = num;
}

void LRParser::setGotoTable(int state, QChar ch, int num)
{
    Goto[state][gotomap[ch]] = num;
}

int LRParser::findIndex(Item item)
{
    for (int i = 0; i < rules.size(); i++)
        if (rules[i].left == item.left && rules[i].right == item.right) return i;
}

void LRParser::setDFA()
{
    int order = 0;
    QQueue<Item> que;
    que.clear();
    que.push_back({rules[0].left, rules[0].right, 0});
    ItemSet its;
    Item temp0(rules[0].left, rules[0].right, 0);
    its.mp[temp0]++;
    while (!que.empty())
    {
        Item temp = que.front();
        que.pop_front();
        if (gotomap.contains(temp.right[temp.index]))
            for (int i = 0; i < rules.size(); i++)
            {
                Item temp0(rules[i].left, rules[i].right, 0);
                if (rules[i].left == temp.right[temp.index] && !its.mp.contains(temp0))
                {
                    its.mp[temp0]++;
                    que.push_back(temp0);
                }
            }
    }
    DFA[its] = order;
    QQueue<ItemSet> q;
    q.clear();
    q.push_back(its);
    while (!q.empty())
    {
        ItemSet its = q.front();
        q.pop_front();
        int state = DFA[its];
        for (QMap<Item, int>::iterator it = its.mp.begin(); it != its.mp.end(); it++)
        {
            Item cur = it.key();
            if (cur.index == cur.right.length())
            {
                QChar acc = firstT;
                if (cur.left == '$' && cur.right[cur.index - 1] == acc)
                {
                    setActionTable(state, '#', "acc", -1);
                    continue;
                }
                int ind = findIndex(cur);
                for (QMap<QChar, int>::const_iterator it1 = actionmap.constBegin(); it1 != actionmap.constEnd(); it1++)
                    setActionTable(state, it1.key(), "r", ind);
                continue;
            }
            ItemSet newset;
            Item newcur(cur.left, cur.right, cur.index + 1);
            newset.mp[newcur]++;
            QQueue<Item> que;
            que.clear();
            que.push_back(newcur);
            while (!que.empty())
            {
                Item temp = que.front();
                que.pop_front();
                if (temp.index < temp.right.length() && gotomap.contains(temp.right[temp.index]))
                    for (int i = 0; i < rules.size(); i++)
                    {
                        Item temp0(rules[i].left, rules[i].right, 0);
                        if (rules[i].left == temp.right[temp.index] && !newset.mp.contains(temp0))
                        {
                            newset.mp[temp0]++;
                            que.push_back(temp0);
                        }
                    }
            }
            bool ok = false;
            int repeatState;
            for (auto it = DFA.begin(); it != DFA.end(); it++)
                if (it.key() == newset)
                {
                    ok = true;
                    repeatState = it.value();
                    break;
                }
            //bool ok = DFA.contains(newset);
            if (!ok)
            {
                order++;
                DFA[newset] = order;
                if (actionmap.contains(cur.right[cur.index]))
                {
                    setActionTable(state, cur.right[cur.index], "S", order);
                    q.push_back(newset);
                }
                if (gotomap.contains(cur.right[cur.index]))
                {
                    setGotoTable(state, cur.right[cur.index], order);
                    q.push_back(newset);
                }
            }
            else
            {
                if (actionmap.contains(cur.right[cur.index]))
                {
                    setActionTable(state, cur.right[cur.index], "S", repeatState);
                    //q.push_back(newset);
                }
                if (gotomap.contains(cur.right[cur.index]))
                {
                    setGotoTable(state, cur.right[cur.index], repeatState);
                    //q.push_back(newset);
                }
            }
        }
    }
}

void LRParser::startAnalyse(QString str, QTableWidget *table)
{
    QString output;
    str += '#';
    QStack<int> state;
    QStack<QChar> symbol;
    int index = 0, step = 1;
    table->setRowCount(1);
    //table->setItem(step - 1, 0, new QTableWidgetItem(QString::number(step)));
    state.push(0);
    table->setItem(step - 1, 0, new QTableWidgetItem("0"));
    symbol.push('#');
    table->setItem(step - 1, 1, new QTableWidgetItem("#"));
    table->setItem(step - 1, 2, new QTableWidgetItem(str.mid(index)));
    if (Action[0][actionmap[str[index]]].type == "r")
    {
        output.clear();
        output += "reduce with the production " + QString::number(Action[state.top()][actionmap[str[index]]].num) +
                  ": " + rules[Action[state.top()][actionmap[str[index]]].num].left + " -> " +
                  rules[Action[state.top()][actionmap[str[index]]].num].right;
        table->setItem(step - 1, 3, new QTableWidgetItem(output));
        QChar newch = rules[Action[state.top()][actionmap[str[index]]].num].left;
        int cur = state.top();
        for (int i = 0; i < rules[Action[cur][actionmap[str[index]]].num].right.length(); i++)
        {
            symbol.pop();
            state.pop();
        }
        symbol.push(newch);
        state.push(Goto[state.top()][gotomap[symbol.top()]]);
    }
    else
    {
        output.clear();
        output += "push '" + str[index] + "', state goto " + QString::number(Action[0][actionmap[str[index]]].num);
        table->setItem(step - 1, 3, new QTableWidgetItem(output));
        state.push(Action[0][actionmap[str[index]]].num);
        symbol.push(str[index]);
        index++;
    }
    while (true)
    {
        step++;
        table->setRowCount(step);
        //table->setItem(step - 1, 0, new QTableWidgetItem(QString::number(step)));
        output.clear();
        for (QStack<int>::iterator it = state.begin(); it != state.end(); it++)
            output += QString::number(*it);
        table->setItem(step - 1, 0, new QTableWidgetItem(output));
        output.clear();
        for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
            output += *it;
        table->setItem(step - 1, 1, new QTableWidgetItem(output));
        table->setItem(step - 1, 2, new QTableWidgetItem(str.mid(index)));
        if (Action[state.top()][actionmap[str[index]]].type == "S")
        {
            output.clear();
            output += "push '" + str[index] + "', state goto " + QString::number(Action[state.top()][actionmap[str[index]]].num);
            table->setItem(step - 1, 3, new QTableWidgetItem(output));
            state.push(Action[state.top()][actionmap[str[index]]].num);
            symbol.push(str[index]);
            index++;
        }
        else if (Action[state.top()][actionmap[str[index]]].type == "r")
        {
            output.clear();
            output += "reduce with the production " + QString::number(Action[state.top()][actionmap[str[index]]].num) +
                      ": " + rules[Action[state.top()][actionmap[str[index]]].num].left + " -> " +
                      rules[Action[state.top()][actionmap[str[index]]].num].right;
            table->setItem(step - 1, 3, new QTableWidgetItem(output));
            QChar newch = rules[Action[state.top()][actionmap[str[index]]].num].left;
            int cur = state.top();
            for (int i = 0; i < rules[Action[cur][actionmap[str[index]]].num].right.length(); i++)
            {
                symbol.pop();
                state.pop();
            }
            symbol.push(newch);
            state.push(Goto[state.top()][gotomap[symbol.top()]]);
        }
        else if (Action[state.top()][actionmap[str[index]]].type == "acc")
        {
            table->setItem(step - 1, 3, new QTableWidgetItem("acc"));
            return;
        }
        else
        {
            table->setItem(step - 1, 3, new QTableWidgetItem("error"));
            return;
        }
    }
}
