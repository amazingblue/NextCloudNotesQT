#include <iostream>
#include <QSettings>
#include <QWidget>
#include <QMainWindow>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), settings(new QSettings)
{
    ui->setupUi(this);
    ui->serverLineEdit->setText(settings->value("credentials/server").toString());
    ui->userLineEdit->setText(settings->value("credentials/user").toString());
    ui->passwordLineEdit->setText(settings->value("credentials/password").toString());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
}

void MainWindow::on_loginButton_clicked()
{
    settings->setValue("credentials/server", ui->serverLineEdit->text());
    settings->setValue("credentials/user", ui->userLineEdit->text());
    settings->setValue("credentials/password", ui->passwordLineEdit->text());
    std::cout << ui->serverLineEdit->text().toStdString() << std::endl;
}
