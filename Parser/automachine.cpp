#include "automachine.h"

Automachine::Automachine()
{

}

Automachine::Prepare()
{
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
}
