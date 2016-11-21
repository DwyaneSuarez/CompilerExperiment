 #include "opparser.h"
#include <QStack>
#include <QQueue>
#include <QTableWidget>

OPParser::OPParser()
{
    mp['+']=0;mp['-']=1;mp['*']=2;mp['i']=3;mp['#']=4;

    precedence[0][0]='>';
    precedence[0][1]='>';
    precedence[0][2]='<';
    precedence[0][3]='<';
    precedence[0][4]='>';

    precedence[1][0]='>';
    precedence[1][1]='>';
    precedence[1][2]='<';
    precedence[1][3]='<';
    precedence[1][4]='>';

    precedence[2][0]='>';
    precedence[2][1]='>';
    precedence[2][2]='>';
    precedence[2][3]='<';
    precedence[2][4]='>';

    precedence[3][0]='>';
    precedence[3][1]='>';
    precedence[3][2]='>';
    precedence[3][3]='$';
    precedence[3][4]='>';

    precedence[4][0]='<';
    precedence[4][1]='<';
    precedence[4][2]='<';
    precedence[4][3]='<';
    precedence[4][4]='=';
}

void OPParser::startAnalyse(QString src, QTableWidget *table)
{
    QStack<QChar> symbol;
    symbol.push('#');
    src += '#';
    QQueue<QChar> input;
    int step = 1, index = 0;
    table->setRowCount(1);
    table->setItem(0, 0, new QTableWidgetItem(QString::number(step)));
    table->setItem(0, 1, new QTableWidgetItem("#"));
    table->setItem(0, 2, new QTableWidgetItem("#"));
    table->setItem(0, 3, new QTableWidgetItem(QString(precedence[mp['#']][mp[src[index]]])));
    table->setItem(0, 4, new QTableWidgetItem(src));
    if (precedence[mp['#']][mp[src[index]]] == '>' && precedence[mp['#']][mp[src[index]]] == '$')
    {
        table->setItem(0, 5, new QTableWidgetItem("Error"));
        return;
    }
    else table->setItem(0, 5, new QTableWidgetItem("Push"));
    symbol.push(src[index]);
    index++;
    int prev = 0;
    while (symbol.top() != '#' || src[index] != '#')
    {
        step ++;
        table->setRowCount(table->rowCount() + 1);
        table->setItem(step - 1, 0, new QTableWidgetItem(QString::number(step)));
        if (prev == 1)
        {
            if (precedence[mp[symbol.top()]][mp[src[index]]] == '<')
            {
                QString output;
                for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
                    output += *it;
                table->setItem(step - 1, 1, new QTableWidgetItem(output));
                table->setItem(step - 1, 2, new QTableWidgetItem(symbol.top()));
                table->setItem(step - 1, 3, new QTableWidgetItem(precedence[mp[symbol.top()]][mp[src[index]]]));
                QString in = src.mid(index);
                table->setItem(step - 1, 4, new QTableWidgetItem(in));
                table->setItem(step - 1, 5, new QTableWidgetItem("Push"));
                prev = 0;
                symbol.push(src[index]);
                index ++;
                if (index > src.length())
                {
                    table->setItem(step - 1, 5, new QTableWidgetItem("Error"));
                    return;
                }
            }
            else if (precedence[mp[symbol.top()]][mp[src[index]]] == '$')
            {
                QString output;
                for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
                    output += *it;
                table->setItem(step - 1, 1, new QTableWidgetItem(output));
                table->setItem(step - 1, 2, new QTableWidgetItem(symbol.top()));
                table->setItem(step - 1, 3, new QTableWidgetItem(precedence[mp[symbol.top()]][mp[src[index]]]));
                QString in = src.mid(index);
                table->setItem(step - 1, 4, new QTableWidgetItem(in));
                table->setItem(step - 1, 5, new QTableWidgetItem("Error"));
                return;
            }
            else
            {
                QString output;
                for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
                    output += *it;
                table->setItem(step - 1, 1, new QTableWidgetItem(output));
                table->setItem(step - 1, 2, new QTableWidgetItem(symbol.top()));
                table->setItem(step - 1, 3, new QTableWidgetItem(precedence[mp[symbol.top()]][mp[src[index]]]));
                QString in = src.mid(index);
                table->setItem(step - 1, 4, new QTableWidgetItem(in));
                table->setItem(step - 1, 5, new QTableWidgetItem("Reduce"));
                prev = 1;
                symbol.pop();
                if (symbol.empty())
                {
                    table->setItem(step - 1, 5, new QTableWidgetItem("Error"));
                    return;
                }
            }
            continue;
        }
        if (precedence[mp[symbol.top()]][mp[src[index]]] == '<' || precedence[mp[symbol.top()]][mp[src[index]]] == '=')
        {
            QString output;
            for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
                output += *it;
            table->setItem(step - 1, 1, new QTableWidgetItem(output));
            table->setItem(step - 1, 2, new QTableWidgetItem(symbol.top()));
            table->setItem(step - 1, 3, new QTableWidgetItem(precedence[mp[symbol.top()]][mp[src[index]]]));
            QString in = src.mid(index);
            table->setItem(step - 1, 4, new QTableWidgetItem(in));
            table->setItem(step - 1, 5, new QTableWidgetItem("Push"));
            prev = 0;
            symbol.push(src[index]);
            index ++;
            if (index > src.length())
            {
                table->setItem(step - 1, 5, new QTableWidgetItem("Error"));
                return;
            }
        }
        else if (precedence[mp[symbol.top()]][mp[src[index]]] == '$')
        {
            QString output;
            for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
                output += *it;
            table->setItem(step - 1, 1, new QTableWidgetItem(output));
            table->setItem(step - 1, 2, new QTableWidgetItem(symbol.top()));
            table->setItem(step - 1, 3, new QTableWidgetItem(precedence[mp[symbol.top()]][mp[src[index]]]));
            QString in = src.mid(index);
            table->setItem(step - 1, 4, new QTableWidgetItem(in));
            table->setItem(step - 1, 5, new QTableWidgetItem("Error"));
            return;
        }
        else
        {
            QString output;
            for (QStack<QChar>::iterator it = symbol.begin(); it != symbol.end(); it++)
                output += *it;
            table->setItem(step - 1, 1, new QTableWidgetItem(output));
            table->setItem(step - 1, 2, new QTableWidgetItem(symbol.top()));
            table->setItem(step - 1, 3, new QTableWidgetItem(precedence[mp[symbol.top()]][mp[src[index]]]));
            QString in = src.mid(index);
            table->setItem(step - 1, 4, new QTableWidgetItem(in));
            table->setItem(step - 1, 5, new QTableWidgetItem("Reduce"));
            prev = 1;
            symbol.pop();
            if (symbol.empty())
            {
                table->setItem(step - 1, 5, new QTableWidgetItem("Error"));
                return;
            }
        }
    }
    step++;
    table->setRowCount(table->rowCount() + 1);
    table->setItem(step - 1, 0, new QTableWidgetItem(QString::number(step)));
    table->setItem(step - 1, 1, new QTableWidgetItem("#"));
    table->setItem(step - 1, 2, new QTableWidgetItem("#"));
    table->setItem(step - 1, 3, new QTableWidgetItem("="));
    table->setItem(step - 1, 4, new QTableWidgetItem("#"));
    table->setItem(step - 1, 5, new QTableWidgetItem("Accept"));
}


