//
// Created by Ilya Amelevich on 31/12/2022.
//

#include <string>
#include "StringUtil.h"

std::vector<std::string> StringUtil::tokenize(const std::string &line, char separator) {
    std::vector<std::string> tokens;
    std::size_t start, end;
    std::string token;
    start = line.find_first_not_of(separator, 0);
    do {
        end = line.find_first_of(separator, start);
        if (start == line.length() || start == end) break;
        if (end != std::string::npos) token = line.substr(start, end - start);
        else token = line.substr(start, line.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos);

    return tokens;
}