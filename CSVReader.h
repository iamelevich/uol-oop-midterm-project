#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>


class CSVReader {
public:
    CSVReader();

    static std::vector<OrderBookEntry> readCSV(const std::string &csvFile);

private:
    static OrderBookEntry lineToOBE(std::string_view strings);

    static OrderBookEntry stringsToOBE(std::vector<std::string_view> strings);
};