#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow() override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::NotesWindow *ui;
};

#endif // NOTESWINDOW_H
