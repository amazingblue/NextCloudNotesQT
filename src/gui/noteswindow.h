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

class WorkerThread : public QThread
{
Q_OBJECT
    void run() override;
signals:
    void resultReady(std::vector<Note*>);
};

class NotesWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow() override;

private slots:
    void on_settingsButton_clicked();
    void attachNotes(std::vector<Note*>);
    void on_notesList_currentRowChanged(int);

private:
    Ui::NotesWindow *ui;
    std::vector<Note*> notes;
    Note* selectedNote;
};

#endif // NOTESWINDOW_H
