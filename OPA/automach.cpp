#include "automach.h"

Automach::Automach(QObject *parent) : QObject(parent)
{

}

void Automach::Prepare()
{


    keyword.clear();
    symbol.clear();

    AddKeyword("if", 1);
    AddKeyword("while", 2);
    AddKeyword("do", 3);
    AddKeyword("else", 4);
    AddKeyword("int", 5);
    AddKeyword("for", 6);
    AddKeyword("cin", 7);
    AddKeyword("cout", 8);
    AddKeyword("include", 9);
    AddKeyword("main", 10);
    AddKeyword("stdio.h", 11);
    AddKeyword("iostream", 12);
    AddKeyword("algorithm", 13);
    AddKeyword("return", 14);
    AddKeyword("endl", 15);
    AddKeyword("using", 16);
    AddKeyword("namespace", 17);
    AddKeyword("std", 18);

    AddSymbol("+", 1);
    AddSymbol("-", 2);
    AddSymbol("*", 3);
    AddSymbol("/", 4);
    AddSymbol("%", 5);
    AddSymbol("=", 6);
    AddSymbol("(", 7);
    AddSymbol(")", 8);
    AddSymbol("+=", 9);
    AddSymbol("-=", 10);
    AddSymbol("*=", 11);
    AddSymbol("/=", 12);
    AddSymbol("%=", 13);
    AddSymbol("!=", 14);
    AddSymbol("==", 15);
    AddSymbol("&&", 16);
    AddSymbol("||", 17);
    AddSymbol("!", 18);
    AddSymbol("++", 19);
    AddSymbol("--", 20);
    AddSymbol(">", 21);
    AddSymbol("<", 22);
    AddSymbol(">=", 23);
    AddSymbol("<=", 24);
    AddSymbol(";", 25);
    AddSymbol(">>", 26);
    AddSymbol("<<", 27);
    AddSymbol("#", 28);
    AddSymbol("[", 29);
    AddSymbol("]", 30);
    AddSymbol("{", 31);
    AddSymbol("}", 32);
    AddSymbol(",", 33);

    AddSeparator("+");
    AddSeparator("-");
    AddSeparator("*");
    AddSeparator("/");
    AddSeparator("%");
    AddSeparator("=");
    AddSeparator("(");
    AddSeparator(")");
    AddSeparator("[");
    AddSeparator("]");
    AddSeparator("{");
    AddSeparator("}");
    AddSeparator("!");
    AddSeparator(";");
    AddSeparator("+=");
    AddSeparator("-=");
    AddSeparator("*=");
    AddSeparator("/=");
    AddSeparator("%=");
    AddSeparator(" ");
    AddSeparator("!=");
    AddSeparator("&&");
    AddSeparator("||");
    AddSeparator("++");
    AddSeparator("--");
    AddSeparator(">");
    AddSeparator("<");
    AddSeparator(">=");
    AddSeparator("<=");
    AddSeparator(">>");
    AddSeparator("<<");
    AddSeparator("#");
    AddSeparator(",");
}

bool Automach::isKeyword(QString str)
{
    if (keyword[str]>0) return true;
        else return false;
}

bool Automach::isSymbol(QString str)
{
    if (symbol[str] > 0) return true;
        else return false;
}

void Automach::AddKeyword(QString str, int number)
{
    keyword[str] = number;
}

void Automach::AddSymbol(QString str, int number)
{
    symbol[str] = number;
}

void Automach::HandleSpace(QString &str)
{
    QString temp;
    int len=str.size();
    int cnt=0,i;
    for (i=0;i<len;i++)
        if (str[i]=='\t') str[i]=' ';
    i=0;
    while (str[i]==' ') i++;
    for (;i<len;i++)
    {
        if (str[i]!=' ') temp[cnt++]=str[i];
            else if (str[i+1]!=' ') temp[cnt++]=' ';
    }
    temp[cnt]='\0';
    str=temp;
}

void Automach::RemoveNote(QString &str,QString &content)
{
    QString temp;
    int cnt=0;
    for (int i=0;i<str.size();i++)
    {
        if (str[i]=='/'&&str[i+1]=='/')
        {
            content=str.mid(i+2,str.size()-i-1);
            while (i<str.size()) i++;
        }
        else temp[cnt++]=str[i];
    }
    temp[cnt]='\0';
    str=temp;
}

void Automach::SplitToWords(QString &str,QVector<QString> &words)
{
    QString word,cur;
    int i=0;
    while (str[i]!='\0')
    {
        cur.clear();
        cur+=str[i];
        if (cur==" ")
        {
            if (!word.isEmpty())
                words.push_back(word);
            word.clear();
        }
        else if (separator.contains(cur))
        {
            if (!word.isEmpty())
                words.push_back(word);
            word.clear();
            if (str[i+1]!='\0') cur+=str[i+1];
            if (separator.contains(cur))
            {
                words.push_back(cur);
                i++;
            }
            else
            {
                if (cur.size()==2)
                   cur=cur.left(cur.size()-1);
                words.push_back(cur);
            }
        }
        else word+=str[i];
        i++;
    }
}

void Automach::AddSeparator(QString str)
{
    separator.insert(str);
}

bool Automach::isSeparator(QString str)
{
    if (separator.find(str)!=separator.end()) return true;
        else return false;
}

bool Automach::isIdentifier(QString str)
{
    if (isSymbol(str)) return false;
    if (isKeyword(str)) return false;
    int i=0;
    if (str[0].isDigit()) return false;
    if (str[0]=='_'||str[0].isLetter())
    {
        for (i=1;i<str.size();i++)
            if (str[i]!='_'&&!str[i].isDigit()&&!str[i].isLetter()) return false;
        return true;
    }
    else return false;
}

bool Automach::isNumber(QString str)
{
    for (int i=0;i<str.size();i++)
        if (str[i]<'0'||str[i]>'9') return false;
    return true;
}
