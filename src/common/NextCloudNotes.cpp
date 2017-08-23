//
// Created by Alberto Luna on 21/08/17.
//

#include <cpr/cpr.h>
#include <iostream>
#include <json.hpp>
#include "NextCloudNotes.h"

using json = nlohmann::json;

nlohmann::json *NextCloudNotes::noteToJSON(Note *note) {
    auto *jsonNote = new nlohmann::basic_json<>();
    (*jsonNote)["id"] = note->id;
    (*jsonNote)["etag"] = note->etag;
    (*jsonNote)["title"] = note->title;
    (*jsonNote)["content"] = note->content;
    (*jsonNote)["favorite"] = note->favorite;
    (*jsonNote)["modified"] = note->modified;
    return jsonNote;
}

Note *NextCloudNotes::jsonToNote(json jsonNote) {
    auto note = new Note();
    note->id = jsonNote["id"];
    note->etag = jsonNote["etag"];
    note->title = jsonNote["title"];
    note->content = jsonNote["content"];
    note->favorite = jsonNote["favorite"];
    note->modified = jsonNote["modified"];
    return note;
}

NextCloudNotes::NextCloudNotes(std::string &server, std::string &user, std::string &password) {
    this->server = server;
    this->user = user;
    this->password = password;
}

const std::vector<Note *> NextCloudNotes::getNotes() {
    auto response = cpr::Get(cpr::Url{getUrl() + "/notes"}, getAuth());
    if (response.status_code != 200) {
        std::cout << response.error.message << '\n';
        std::cout << response.status_code << '\n';
        std::cout << response.text << '\n';
        return std::vector<Note *>();
    }
    auto jsonResponse = json::parse(response.text);
    std::vector<Note *> notes;
    for (auto &element : jsonResponse) {
        notes.push_back(NextCloudNotes::jsonToNote(element));
    }
    return notes;
}

Note *NextCloudNotes::saveNote(Note *note) {
    auto jsonNote = NextCloudNotes::noteToJSON(note);
    auto stringNote = jsonNote->dump();
    delete jsonNote;
    auto response = cpr::Put(
            cpr::Url{getUrl() + "/notes/" + std::to_string(note->id)},
            cpr::Body{stringNote},
            cpr::Header{{"Content-Type", "application/json"}},
            getAuth()
    );
    if (response.status_code != 200) {
        std::cout << response.error.message << '\n';
        std::cout << response.status_code << '\n';
        std::cout << response.text << '\n';
        return nullptr;
    }
    auto updatedNote = NextCloudNotes::jsonToNote(json::parse(response.text));
    note->modified = updatedNote->modified;
    note->title = updatedNote->title;
    delete updatedNote;
    return note;
}

std::string NextCloudNotes::getUrl() {
    return server + BASE_URL;
}

cpr::Authentication NextCloudNotes::getAuth() {
    return cpr::Authentication({user, password});
}
