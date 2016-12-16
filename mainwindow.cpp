#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include <QDebug>

#define qd qDebug()<<

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fn;
    fn=ui->lineEdit->text();
    fn=QFileDialog::getOpenFileName(this,tr("请选择文件"),tr("."),tr("file(*.cpp)"));
    ui->lineEdit->setText(fn);
}

void MainWindow::on_pushButton_2_clicked()
{

    QFile file(ui->lineEdit->text());
    if(!file.open(QFile::ReadOnly)){
        QMessageBox::critical(this,tr("错误"),tr("文件读取失败！"),QMessageBox::Cancel);
        return;
    }

    QTextStream in(&file);
    QString nextLine;
    QString myFile;
    //防止打开空文件
    if(in.atEnd()){
        QMessageBox::critical(this,tr("错误"),tr("这是一个空文件！"),QMessageBox::Cancel);
        return;
    }

    int i=1;
    while(!in.atEnd()){
        nextLine=in.readLine();
        if(nextLine.startsWith("{"))
            nextLine=nextLine+tr("\n//! %1").arg(i);

        if(nextLine.startsWith(tr("}")))
            nextLine=tr("//! %1\n").arg(i++)+nextLine;
        myFile+=nextLine+tr("\n");
    }
    file.close();

    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        QMessageBox::critical(this,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
        return;
    }
    file.write(myFile.toLocal8Bit());
    file.close();
    QMessageBox::information(this,tr("格式化完成!"),tr("完成"),QMessageBox::Ok);
}
