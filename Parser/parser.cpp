#include "parser.h"

Parser::Parser(QObject *parent) : QObject(parent)
{
    autom = new Automach(this);

}

void Parser::Prepare()
{
    words.clear();
    autom->Prepare();
    index = 0;
    success = true;
}

void Parser::toWords(QString &src)
{
    int p=0,q;
    while (src[p]!='\0')
    {
        q=p;
        while (src[q]!='\n'&&q<src.size()) q++;
        QString line = src.mid(p,q-p);
        autom->HandleSpace(line);
        QString NoteContent;
        autom->RemoveNote(line,NoteContent);
        autom->SplitToWords(line, words);
        p=q+1;
    }
}

void Parser::Replace(QString a, QString b)
{
    int pos = Output.indexOf(a);
    Output.replace(pos, a.length(), b);
}

void Parser::program()
{
    answer.clear();
    Output.clear();
    Output += "program";
    answer.push_back(Output);
    Replace("program", "=>program");
    Replace("program", "block");
    answer.push_back(Output);
    block();
    if (index < words.size()) success = false;
}

void Parser::block()
{
    if (index >= words.size()) return;
    if (!success) return;
    if (words[index]=="{")
    {
        Replace("block", "{stmts}");
        answer.push_back(Output);
        index++;
        stmts();
        if (index >= words.size() || words[index] != "}")
        {
            success = false;
            return;
        }
        index++;
    }
    else success = false;
}

void Parser::stmts()
{
    if (!success) return;
    if (index >= words.size()) return;
    if (words[index] == "}")
    {
        Replace(" stmts", "");
        answer.push_back(Output);
        return;
    }
    Replace("stmts", "stmt stmts");
    answer.push_back(Output);
    stmt();
    stmts();
}

void Parser::stmt()
{
    if (!success) return;
    if (index >= words.size()) return;
    if (autom->isIdentifier(words[index]))
    {
        //answer.push_back("error");return;
        Replace("stmt", "id = expr");
        answer.push_back(Output);
        index++;
        if (index >= words.size())
        {
            success = false;
            return;
        }
        if (words[index] != "=")
        {
            success = false;
            return;
        }
        index++;
        expr();
        if (index >= words.size())
        {
            success = false;
            return;
        }
        if (words[index] != ";")
        {
            success = false;
            return;
        }
        index++;
        return;
    }
    else if (words[index] == "if")
    {
        //answer.push_back("error");return;
        Replace("stmt", "if (boolexp) stmt stmt1");
        answer.push_back(Output);
        index++;
        if (index >= words.size() || words[index] != "(")
        {
            success = false;
            return;
        }
        index++;
        boolexp();
        if (index >= words.size() || words[index] != ")")
        {
            success = false;
            return;
        }
        index++;
        stmt();
        if (index >= words.size()) return;
        if (words[index] == "else")
        {
            Replace("stmt1", "else stmt");
            answer.push_back(Output);
            index++;
            stmt();
            return;
        }
        else
        {
            Replace(" stmt1", "");
            answer.push_back(Output);
            return;
        }
    }
    else if (words[index] == "while")
    {
        //answer.push_back("error");return;
        Replace("stmt", "while (boolexp) stmt");
        answer.push_back(Output);
        index++;
        if (index >= words.size() || words[index] != "(")
        {
            success = false;
            return;
        }
        index++;
        boolexp();
        if (index >= words.size() || words[index] != ")")
        {
            success = false;
            return;
        }
        index++;
        stmt();
        return;
    }
    else if (words[index] == "break")
    {
        Replace("stmt", "break");
        answer.push_back(Output);
        index++;
        return;
    }
    else
    {
        Replace("stmt", "block");
        answer.push_back(Output);
        block();
        return;
    }
}

void Parser::boolexp()
{
    if (!success || index >= words.size()) return;
    Replace("boolexp", "expr operator expr");
    answer.push_back(Output);
    expr();
    if (index >= words.size()) return;
    if (words[index] == "<")
    {
        //Replace("boolexp", "expr<expr");
        Replace("operator expr", "< expr");
        answer.push_back(Output);
        index++;
        expr();
        return;
    }
    else if (words[index] == "<=")
    {
        //Replace("boolexp", "expr<=expr");
        Replace("operator expr", "<= expr");
        answer.push_back(Output);
        index++;
        expr();
        return;
    }
    else if (words[index] == ">")
    {
        //Replace("boolexp", "expr>expr");
        Replace("operator expr", "> expr");
        answer.push_back(Output);
        index++;
        expr();
        return;
    }
    else if (words[index] == ">=")
    {
       // Replace("boolexp", "expr>=expr");
        Replace("operator expr", ">= expr");
        answer.push_back(Output);
        index++;
        expr();
        return;
    }
}

void Parser::expr()
{
    if (!success) return;
    Replace("expr", "term expr1");
    answer.push_back(Output);
    term();
    expr1();
}

void Parser::expr1()
{
    if (!success) return;
    if (index >= words.size()) return;
    if (words[index] == "+")
    {
        Replace("expr1", "+ term expr1");
        answer.push_back(Output);
        index++;
        term();
        expr1();
        return;
    }
    else if (words[index] == "-")
    {
        Replace("expr1", "- term expr1");
        answer.push_back(Output);
        index++;
        term();
        expr1();
        return;
    }
    else
    {
        Replace(" expr1", "");
        answer.push_back(Output);
        return;
    }
}

void Parser::term()
{
    if (!success) return;
    Replace("term", "factor term1");
    answer.push_back(Output);
    factor();
    term1();
}

void Parser::term1()
{
    if (!success) return;
    if (index >= words.size()) return;
    if (words[index] == "*")
    {
        Replace("term1", "* factor term1");
        answer.push_back(Output);
        index++;
        factor();
        term1();
        return;
    }
    else if (words[index] == "/")
    {
        Replace("term1", "/ factor term1");
        answer.push_back(Output);
        index++;
        factor();
        term1();
        return;
    }
    else
    {
        Replace(" term1", "");
        answer.push_back(Output);
        return;
    }
}

void Parser::factor()
{
    if (!success) return;
    if (index >= words.size()) return;
    if (words[index] == "(")
    {
        Replace("factor", "(expr)");
        answer.push_back(Output);
        index++;
        expr();
        if (index >= words.size() || words[index] != ")")
        {
            success = false;
            return;
        }
        index++;
        return;
    }
    else if (autom->isIdentifier(words[index]))
    {
        Replace("factor", "id");
        answer.push_back(Output);
        index++;
        return;
    }
    else if (autom->isNumber(words[index]))
    {
        Replace("factor", "num");
        answer.push_back(Output);
        index++;
        return;
    }
    else
    {
        success = false;
        return;
    }
}
