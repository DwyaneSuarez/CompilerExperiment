#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pars = new Parser(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "open file", " ",  "codefile(*.cpp *.c *.txt)");
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

void MainWindow::on_actionAnalyse_triggered()
{
    pars->Prepare();
    ui->AnalyseRes->clear();
    QString src = ui->CodeInput->toPlainText();
    pars->toWords(src);
    pars->program();
    QVector<QString> Output = pars->getAns();
    for (int i = 0;i < Output.size();i++)
        ui->AnalyseRes->appendPlainText(Output[i]);
    ui->AnalyseRes->appendPlainText(pars->getSuccess()?"Successed":"Failed");
}


