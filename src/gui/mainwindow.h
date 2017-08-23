#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_loginButton_clicked();

private:
    Ui::MainWindow* ui;
    QSettings* settings;
    QWidget* parentWidget;
};

#endif // MAINWINDOW_H
