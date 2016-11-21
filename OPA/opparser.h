#ifndef OPPARSER_H
#define OPPARSER_H

#include <QObject>
#include <QTableWidget>

class OPParser
{
public:
    OPParser();
    void startAnalyse(QString, QTableWidget *);

private:
    QChar precedence[10][10];
    QMap<QChar, int> mp;
};

#endif // OPPARSER_H
