#include <algorithm>
#include <QWidget>
#include <QMainWindow>
#include <QThread>
#include <QListWidgetItem>
#include "noteswindow.h"
#include "ui_noteswindow.h"
#include "mainwindow.h"

using json = nlohmann::json;

NotesWindow::NotesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);

    auto *networkWorker = new NetworkWorker;
    networkWorker->moveToThread(&networkThread);

    // get notes
    connect(this, &NotesWindow::getNotes, networkWorker, &NetworkWorker::on_getNotes);
    connect(networkWorker, &NetworkWorker::getNotesReady, this, &NotesWindow::attachNotes);
    // save notes
    connect(this, &NotesWindow::saveNote, networkWorker, &NetworkWorker::on_saveNote);
    connect(networkWorker, &NetworkWorker::saveNoteReady, this, &NotesWindow::on_noteSaved);

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
    if (notes.empty() || index == -1) {
        ui->saveNoteButton->setDisabled(true);
        selectedNote = nullptr;
        return;
    }
    selectedNote = notes[index];
    noteHasBeenSelected = true;
    ui->noteTextEdit->setText(QString::fromStdString(selectedNote->content));
}

void NotesWindow::on_noteTextEdit_textChanged() {
    if (selectedNote == nullptr) {
        return;
    }
    if (noteHasBeenSelected) {
        noteHasBeenSelected = false;
        return;
    }
    ui->saveNoteButton->setDisabled(false);
}

void NotesWindow::on_saveNoteButton_clicked() {
    if (selectedNote == nullptr) {
        return;
    }
    selectedNote->content = ui->noteTextEdit->toPlainText().toStdString();
    ui->saveNoteButton->setDisabled(true);
    emit saveNote(selectedNote);
}

void NotesWindow::on_noteSaved(Note* note) {
    ui->saveNoteButton->setDisabled(false);
    if (note == nullptr) {
        std::cout << "error saving note" << '\n';
        return;
    }
    auto noteIndex = std::distance(notes.begin(), std::find(notes.begin(), notes.end(), note));
    ui->notesList->item(noteIndex)->setText(QString::fromStdString(note->title));
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

void NetworkWorker::on_saveNote(Note* note) {
    auto updatedNote = notesClient->saveNote(note);
    emit saveNoteReady(updatedNote);
}
