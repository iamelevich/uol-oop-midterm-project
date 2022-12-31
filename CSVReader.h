#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>


class CSVReader {
public:
    CSVReader();

    static std::vector<OrderBookEntry> readCSV(const std::string &csvFile);

    static OrderBookEntry stringsToOBE(const std::string &price,
                                       const std::string &amount,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType OrderBookType);

private:
    static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};