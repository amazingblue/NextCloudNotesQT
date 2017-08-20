#include "mainwindow.h"
#include "noteswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Alberto Luna");
    a.setOrganizationDomain("albertoluna.es");
    a.setApplicationName("Nextcloud Notes");
    auto settings = new QSettings;
    auto server = settings->value("credentials/server").toString();
    if (server != "") {
        auto notesWindow = new NotesWindow;
        notesWindow->show();
    } else {
        auto w = new MainWindow;
        w->show();
    }

    delete settings;
    return a.exec();
}
