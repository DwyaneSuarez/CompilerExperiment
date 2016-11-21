#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QTextEdit>
#include <QVector>
#include "parser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionQuit_triggered();

    void on_actionAnalyse_triggered();

private:
    Ui::MainWindow *ui;
    Parser *pars;

};

#endif // MAINWINDOW_H
