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

[[nodiscard]] std::vector<std::string_view> StringUtil::tokenize(std::string_view str, char delim) {
    std::vector<std::string_view> output;

    for (auto first = str.begin(), second = str.begin(), last = str.end();
         second != last && first != last; first = std::next(second)) {
        second = std::find(first, last, delim);

        // we might get empty string views here, but that's ok in the case of CSV reader
        //output.emplace_back(str.substr(std::distance(str.begin(), first), std::distance(first, second)));
        output.emplace_back(&*first, std::distance(first, second));

        if (second == last)
            break;
    }

    return output;
}

[[nodiscard]] std::vector<std::string_view> StringUtil::splitLines(std::string_view str) {
    auto lines = tokenize(str, '\n');

    if (!lines.empty() && lines[0].back() == '\r') { // Windows CR conversion
        for (auto &&line: lines) {
            if (line.back() == '\r')
                line.remove_suffix(1);
        }
    }

    return lines;
}