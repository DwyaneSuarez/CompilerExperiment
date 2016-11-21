#ifndef LRPARSER_H
#define LRPARSER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QMap>
#include <QSet>
#include <QTableWidget>
#include <iterator>

struct AnalyseTable
{
    QString type;
    int num;
};

struct Rules
{
    QChar left;
    QString right;
    Rules(){}
    Rules(QChar l, QString r)
    {
        left = l;
        right = r;
    }
};

struct Item
{
    QChar left;
    QString right;
    int index;
    Item(){}
    Item(QChar l, QString r, int i=0)
    {
        left = l;
        right = r;
        index = i;
    }
    bool operator < (const Item &a) const
    {
        if (left != a.left) return left < a.left;
            else if (right != a.right) return right < a.right;
                else return index < a.index;
    }
    bool operator == (const Item &b) const
    {
        if (left == b.left && right == b.right && index == b.index) return true;
            else return false;
    }
};

struct ItemSet
{
    QMap<Item, int> mp;
    ItemSet()
    {
        mp.clear();
    }
    bool operator < (const ItemSet &b) const
    {
        if (mp.size() != b.mp.size()) return mp.size() < b.mp.size();
        for (auto it = b.mp.begin(), ite = mp.begin(); it != b.mp.end(); it++, ite++)
            if (!(it.key() == ite.key())) return it.key() < ite.key();
    }
    bool operator == (const ItemSet &b) const
    {
        for (auto it = b.mp.begin(); it != b.mp.end(); it++)
        {
            bool ok = false;
            for (auto ite = this->mp.begin(); ite != this->mp.end(); ite++)
                if (it.key() == ite.key())
                {
                    ok = true;
                    break;
                }
            if (!ok) return false;
        }
        return true;
    }
};

class LRParser
{
public:
    LRParser();
    void setActionMap(QChar, int);
    void setGotoMap(QChar, int);
    void setActionTable(int, QChar, QString, int);
    void setGotoTable(int, QChar, int);
    void startAnalyse(QString, QTableWidget *);
    void addRules(QChar, QString);
    void setRules(QStringList);
    void setDFA();
    int findIndex(Item);

private:
    QMap<QChar, int> actionmap,gotomap;
    AnalyseTable Action[100][100];
    int Goto[100][100];
    QVector<Rules> rules;
    QMap<ItemSet, int> DFA;
    int amapsize,gmapsize;
    QChar firstT;
};

#endif // LRPARSER_H
