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
    auto *workerThread = new WorkerThread;
    connect(workerThread, SIGNAL(resultReady(const std::vector<Note*>)), this, SLOT(attachNotes(const std::vector<Note*>)));
    workerThread->start();
    new QListWidgetItem("Loading notes...", ui->notesList);
    notes = std::vector<Note*>();
}

NotesWindow::~NotesWindow()
{
    delete ui;
}

void NotesWindow::on_settingsButton_clicked() {
    auto w = new MainWindow;
    w->show();
}

void NotesWindow::attachNotes(const std::vector<Note*> notes) {
    ui->notesList->clear();
    this->notes = notes;
    for (auto &note : notes) {
        new QListWidgetItem(QString::fromStdString(note->title), ui->notesList);
        std::cout << note->title << '\n';
    }
}

void NotesWindow::on_notesList_currentRowChanged(int index) {
    std::cout << index << '\n';
    if (notes.empty()) {
        return;
    }
    if (index == -1) {
        return;
    }
    selectedNote = notes[index];
    ui->noteTextEdit->setText(QString::fromStdString(selectedNote->content));
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
