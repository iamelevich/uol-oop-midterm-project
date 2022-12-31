//
// Created by Ilya Amelevich on 31/12/2022.
//

#ifndef MIDTERM_PROJECT_STRINGUTIL_H
#define MIDTERM_PROJECT_STRINGUTIL_H

#include <vector>

class StringUtil {
public:
    static std::vector<std::string> tokenize(const std::string &line, char separator);
};

#endif //MIDTERM_PROJECT_STRINGUTIL_H
