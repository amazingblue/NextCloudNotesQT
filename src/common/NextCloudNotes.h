//
// Created by Alberto Luna on 21/08/17.
//

#ifndef NEXTCLOUDNOTES_NEXTCLOUDNOTES_H
#define NEXTCLOUDNOTES_NEXTCLOUDNOTES_H


#include <string>
#include <vector>
#include <QMetaType>
#include <cpr/cpr.h>
#include <json.hpp>

struct Note {
    int id;
    long modified;
    std::string title;
    std::string category;
    std::string content;
    std::string etag;
    bool favorite;
};

Q_DECLARE_METATYPE(Note*);
Q_DECLARE_METATYPE(std::vector<Note*>);

class NextCloudNotes {
public:
    NextCloudNotes(std::string& server, std::string& user, std::string& password);
    const std::vector<Note*> getNotes();
    Note* saveNote(Note* note);

private:
    const std::string BASE_URL = "/index.php/apps/notes/api/v0.2";
    std::string server;
    std::string user;
    std::string password;
    std::string getUrl();
    cpr::Authentication getAuth();
    static nlohmann::json* noteToJSON(Note* note);
    static Note* jsonToNote(nlohmann::json jsonNote);
};


#endif //NEXTCLOUDNOTES_NEXTCLOUDNOTES_H
