#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opparser.h"
#include "automach.h"
#include <QFileDialog>
#include <QTextStream>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->AnalyseProcess->horizontalHeader()->setStretchLastSection(true);

    ui->AnalyseProcess->setColumnCount(6);
    ui->AnalyseProcess->setHorizontalHeaderItem(0, new QTableWidgetItem("Step"));
    ui->AnalyseProcess->setHorizontalHeaderItem(1, new QTableWidgetItem("Symbol Stack"));
    ui->AnalyseProcess->setHorizontalHeaderItem(2, new QTableWidgetItem("Terminal"));
    ui->AnalyseProcess->setHorizontalHeaderItem(3, new QTableWidgetItem("Relation"));
    ui->AnalyseProcess->setHorizontalHeaderItem(4, new QTableWidgetItem("Input"));
    ui->AnalyseProcess->setHorizontalHeaderItem(5, new QTableWidgetItem("Action"));
    //ui->AnalyseProcess->verticalHeader()->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "open file", " ",  "codefile(*.cpp *.c *.txt)");
    if (path.isEmpty())
        return;
    QFile openfile(path);
    openfile.open(QIODevice::ReadOnly);
    QTextStream ts(&openfile);
    ui->ExpInput->clear();
    ui->ExpInput->appendPlainText(ts.readAll());
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
    ts << ui->ExpInput->toPlainText();
}

void MainWindow::on_actionAnalyse_triggered()
{
    QString src = ui->ExpInput->toPlainText();
    int p=0,q;
    QVector<QString> words;
    words.clear();
    Automach *autom = new Automach;
    autom->Prepare();
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
    QTableWidget *table = ui->AnalyseProcess;
    OPParser *parser = new OPParser();
    parser->startAnalyse(src, table);
}
