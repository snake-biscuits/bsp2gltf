#pragma once
#include <string>



std::string sanitise(std::string s) {
    // TODO: lowercase
    size_t index = 0;
    while ((index = s.find("\\", 0)) != std::string::npos) {
        s.replace(index, 1, "/");
        index += 1;
    }
    return s;
}
