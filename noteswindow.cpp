#include <QWidget>
#include <QMainWindow>
#include <QString>
#include "noteswindow.h"
#include "ui_noteswindow.h"
#include <cpr/cpr.h>
#include <json.hpp>

using json = nlohmann::json;

NotesWindow::NotesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);
}

NotesWindow::~NotesWindow()
{
    delete ui;
}

void NotesWindow::on_pushButton_clicked()
{
    std::thread([&]{
        auto response = cpr::Get(cpr::Url{"https://arenavision-unofficial.herokuapp.com/json"});
        auto jsonResponse = json::parse(response.text);
        ui->label->setText(QString::fromStdString(jsonResponse.dump(2)));
    }).detach();
}
