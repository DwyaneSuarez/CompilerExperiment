#ifndef PARSER_H
#define PARSER_H

#include "automach.h"

#include <QObject>
#include <QVector>
#include <QString>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);
    void Prepare();
    void program();
    void block();
    void stmts();
    void stmt();
    void boolexp();
    void bool1();
    void expr();
    void expr1();
    void term();
    void term1();
    void factor();
    void toWords(QString &);
    void Replace(QString, QString);
    QVector<QString> getAns()
    {
        return answer;
    }
    bool getSuccess() {return success;}

private:
    Automach *autom;
    QVector<QString> words;
    QString Output;
    QVector<QString> answer;
    int index;
    bool success;

signals:

public slots:
};

#endif // PARSER_H
