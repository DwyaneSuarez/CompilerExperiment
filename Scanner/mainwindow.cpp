#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    autom = new Automach(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAnalyse_triggered()
{
    autom->Prepare();
    ui->AnalyseRes->clear();
    ui->ErrorInfo->clear();
    QString src = ui->CodeInput->toPlainText();
    int p=0,q;
    int label=0;
    while (src[p]!='\0')
    {
        label++;
        q=p;
        while (src[q]!='\n'&&q<src.size()) q++;
        QString line = src.mid(p,q-p);
        autom->HandleSpace(line);
        QString NoteContent;
        autom->RemoveNote(line,NoteContent);
        //ui->AnalyseRes->appendPlainText(line);
        QVector<QString> words;
        QString output=QString::number(label,10);
        output+=": ";
        ui->AnalyseRes->appendPlainText(output);
        autom->SplitToWords(line, words);
        //for (int i=0;i<words.size();i++) //output+=words[i];
        //    ui->AnalyseRes->appendPlainText(words[i]);

        bool flag=true;
        for (int i=0;i<words.size();i++)
        {
            //ui->AnalyseRes->appendPlainText(words[i]);
            if (words[i].isEmpty()) continue;
            output.clear();
            if (autom->isKeyword(words[i])>0)
            {
                output+="("+words[i]+", Keyword)";
                ui->AnalyseRes->appendPlainText(output);
            }
            else if (autom->isSymbol(words[i])>0)
            {
                output+="("+words[i]+", Symbol)";
                ui->AnalyseRes->appendPlainText(output);
            }
            else if (autom->isNumber(words[i]))
            {
                output+="("+words[i]+", Number)";
                ui->AnalyseRes->appendPlainText(output);
            }
            else if (autom->isIdentifier(words[i]))
            {
                output+="("+words[i]+", Identifier)";
                ui->AnalyseRes->appendPlainText(output);
            }
            else
            {
                flag=false;
                //ui->AnalyseRes->appendPlainText(i+words[i]+" Error");
            }
        }
        if (!NoteContent.isEmpty())
        {
            output.clear();
            output+="Contents of Note: "+NoteContent;
            ui->AnalyseRes->appendPlainText(output);
        }
        if (!flag)
        {
            output=QString::number(label,10);
            output+=": Error";
            ui->ErrorInfo->appendPlainText(output);
        }
        //for (int i=0;i<words.size();i++) output+=words[i]+' ';
        //ui->AnalyseRes->appendPlainText(output);

        p=q+1;
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "open file", " ",  "codefile(*.cpp *.c)");
    if (path.isEmpty())
        return;
    QFile openfile(path);
    openfile.open(QIODevice::ReadOnly);
    QTextStream ts(&openfile);
    ui->CodeInput->clear();
    ui->CodeInput->appendPlainText(ts.readAll());
    openfile.close();

}

void MainWindow::on_actionSave_As_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "save file", " ", "codefile(*.cpp *.c *.txt)");
    if (path.isEmpty())
        return;
    QFile savefile(path);
    savefile.open(QIODevice::WriteOnly);
    QTextStream ts(&savefile);
    ts << ui->CodeInput->toPlainText();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
