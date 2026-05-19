
#pragma once

#include "../Event.hpp"
#include "../Cancellable.hpp"
#include <string>

class ReadFileEvent : public Event {
public:
    void *This;
    std::string *retstr;
    std::string path;
    std::string *result;

    ReadFileEvent(void *This, std::string *retstr, std::string path, std::string *result)
        : Event(), This(This), retstr(retstr), path(path), result(result) {}

    [[nodiscard]] std::string getretstr() const {
        return *retstr;
    }
    [[nodiscard]] const std::string& getpath() const {
        return path;
    }
    [[nodiscard]] std::string* getresult() const {
        return result;
    }

    void returnresult(std::string *result) {
        this->result = result;
    }
};
