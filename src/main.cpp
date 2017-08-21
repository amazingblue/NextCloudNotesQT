#include "src/common/NextCloudNotes.h"
#include "src/gui/mainwindow.h"
#include "src/gui/noteswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<std::vector<Note*>>();
    qRegisterMetaType<Note>();
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
