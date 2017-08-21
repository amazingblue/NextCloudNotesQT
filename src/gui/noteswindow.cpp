#include <QWidget>
#include <QMainWindow>
#include <QThread>
#include "noteswindow.h"
#include "ui_noteswindow.h"
#include "mainwindow.h"
#include <cpr/cpr.h>
#include <json.hpp>

using json = nlohmann::json;

NotesWindow::NotesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);
    auto *workerThread = new WorkerThread;
    connect(workerThread, SIGNAL(resultReady(const std::vector<Note*>)), this, SLOT(attachNotes(const std::vector<Note*>)));
    workerThread->start();
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

void NotesWindow::on_settingsButton_clicked() {
    auto w = new MainWindow;
    w->show();
}

void NotesWindow::attachNotes(const std::vector<Note*> notes) {
    for (auto &note : notes) {
        ui->verticalLayout->addWidget(new QLabel(QString::fromStdString(note->title)));
        std::cout << note->title << '\n';
    }
}

void WorkerThread::run() {
    QSettings settings;
    auto server = settings.value("credentials/server").toString().toStdString();
    auto user = settings.value("credentials/user").toString().toStdString();
    auto password = settings.value("credentials/password").toString().toStdString();
    NextCloudNotes nextCloudNotes(server, user, password);
    auto notes = nextCloudNotes.getNotes();
    emit resultReady(notes);
}
