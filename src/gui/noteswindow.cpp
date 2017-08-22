#include <QWidget>
#include <QMainWindow>
#include <QThread>
#include <QListWidgetItem>
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
    auto *networkWorker = new NetworkWorker;
    networkWorker->moveToThread(&networkThread);
    connect(networkWorker, SIGNAL(getNotesReady(const std::vector<Note*>)), this, SLOT(attachNotes(const std::vector<Note*>)));
    connect(this, SIGNAL(getNotes()), networkWorker, SLOT(on_getNotes()));
    networkThread.start();
    emit getNotes();
    new QListWidgetItem("Loading notes...", ui->notesList);
    notes = std::vector<Note*>();
}

NotesWindow::~NotesWindow() {
    networkThread.quit();
    networkThread.wait();
    delete ui;
    for (auto &note : notes) {
        delete note;
    }
}

void NotesWindow::on_settingsButton_clicked() {
    auto w = new MainWindow;
    w->show();
}

void NotesWindow::attachNotes(const std::vector<Note*> notes) {
    ui->notesList->clear();
    this->notes = notes;
    for (const auto &note : notes) {
        new QListWidgetItem(QString::fromStdString(note->title), ui->notesList);
    }
}

void NotesWindow::on_notesList_currentRowChanged(int index) {
    if (notes.empty()) {
        return;
    }
    if (index == -1) {
        return;
    }
    selectedNote = notes[index];
    ui->noteTextEdit->setText(QString::fromStdString(selectedNote->content));
}

NetworkWorker::NetworkWorker() {
    QSettings settings;
    auto server = settings.value("credentials/server").toString().toStdString();
    auto user = settings.value("credentials/user").toString().toStdString();
    auto password = settings.value("credentials/password").toString().toStdString();
    notesClient = new NextCloudNotes(server, user, password);
}

NetworkWorker::~NetworkWorker() {
    delete notesClient;
}

void NetworkWorker::on_getNotes() {
    auto notes = notesClient->getNotes();
    emit getNotesReady(notes);
}
