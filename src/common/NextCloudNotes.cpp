//
// Created by Alberto Luna on 21/08/17.
//

#include <cpr/cpr.h>
#include <iostream>
#include <json.hpp>
#include "NextCloudNotes.h"

using json = nlohmann::json;

NextCloudNotes::NextCloudNotes(std::string& server, std::string& user, std::string& password) {
    this->server = server;
    this->user = user;
    this->password = password;
}

const std::vector<Note*> NextCloudNotes::getNotes() {
    auto response = cpr::Get(cpr::Url{ getUrl() + "/notes" }, cpr::Authentication{ user, password });
    if(response.status_code != 200) {
        std::cout << response.error.message << '\n';
        std::cout << response.status_code << '\n';
        std::cout << response.text << '\n';
        return std::vector<Note*>();
    }
    auto jsonResponse = json::parse(response.text);
    std::vector<Note*> notes;
    for (auto& element : jsonResponse) {
        auto note = new Note();
        note->id = element["id"];
        note->etag = element["etag"];
        note->title = element["title"];
        note->content = element["content"];
        note->favorite = element["favorite"];
        note->modified = element["modified"];
        notes.push_back(note);
    }
    return notes;
}

std::string NextCloudNotes::getUrl() {
    return server + BASE_URL;
}
