#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QThread>
#include <QSettings>
#include <iostream>
#include "../common/NextCloudNotes.h"

namespace Ui {
class NotesWindow;
}

class NetworkWorker : public QObject
{
Q_OBJECT

signals:
    void getNotesReady(std::vector<Note*>);

private slots:
    void on_getNotes();

public:
    explicit NetworkWorker();
    ~NetworkWorker() override;

private:
    NextCloudNotes* notesClient;
};

class NotesWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow() override;

signals:
    void getNotes();

private slots:
    void on_settingsButton_clicked();
    void attachNotes(std::vector<Note*>);
    void on_notesList_currentRowChanged(int);

private:
    QThread networkThread;
    Ui::NotesWindow *ui;
    std::vector<Note*> notes;
    Note* selectedNote;
};

#endif // NOTESWINDOW_H
