#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"fileinteraction.h"
#include <QMainWindow>
#include<QString>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    FileInteraction tem;
    bool openFlag=false;//资源文件是否打开
    void showData(FileInteraction &temporary);
    QString fileAddress;
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
