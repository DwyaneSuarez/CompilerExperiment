#ifndef AUTOMACH_H
#define AUTOMACH_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QVector>

class Automach : public QObject
{
    Q_OBJECT
public:
    explicit Automach(QObject *parent = 0);
    bool isKeyword(QString);
    bool isSymbol(QString);
    bool isSeparator(QString);
    bool isIdentifier(QString);
    bool isNumber(QString);
    void Prepare();
    void AddKeyword(QString , int );
    void AddSymbol(QString , int );
    void AddSeparator(QString);
    void HandleSpace(QString &);
    void RemoveNote(QString &,QString &);
    void SplitToWords(QString &,QVector<QString> &);

private:
    QMap<QString, int> keyword,symbol;
    QSet<QString> separator;

signals:

public slots:
};

#endif // AUTOMACH_H
