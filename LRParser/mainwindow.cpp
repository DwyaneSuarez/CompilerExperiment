#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lrparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->AnalyseProcess->horizontalHeader()->setStretchLastSection(true);
    ui->AnalyseProcess->setColumnCount(4);
    //ui->AnalyseProcess->setHorizontalHeaderItem(0, new QTableWidgetItem("Step"));
    ui->AnalyseProcess->setHorizontalHeaderItem(0, new QTableWidgetItem("State"));
    ui->AnalyseProcess->setHorizontalHeaderItem(1, new QTableWidgetItem("Symbol"));
    ui->AnalyseProcess->setHorizontalHeaderItem(2, new QTableWidgetItem("String"));
    ui->AnalyseProcess->setHorizontalHeaderItem(3, new QTableWidgetItem("Action"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAnalyse_triggered()
{
    QString rules = ui->InputRules->toPlainText();
    QStringList list = rules.split('\n');
    QString input = ui->InputString->toPlainText();
    LRParser *pars = new LRParser();
    QTableWidget *table = ui->AnalyseProcess;
    pars->setRules(list);
    pars->setDFA();

    pars->startAnalyse(input, table);
}
