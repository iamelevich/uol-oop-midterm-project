//
// Created by Ilya Amelevich on 31/12/2022.
//

#ifndef MIDTERM_PROJECT_STRINGUTIL_H
#define MIDTERM_PROJECT_STRINGUTIL_H

#include <vector>

class StringUtil {
public:
    static std::vector<std::string> tokenize(const std::string &line, char separator);

    static std::vector<std::string_view> tokenize(std::string_view str, char separator);

    static std::vector<std::string_view> splitLines(std::string_view str);
};

#endif //MIDTERM_PROJECT_STRINGUTIL_H
