#include "CSVReader.h"
#include "StringUtil.h"
#include "Bot.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>
#include <filesystem>

using namespace std::chrono;

CSVReader::CSVReader() = default;

std::vector<OrderBookEntry> CSVReader::readCSV(const std::string &csvFilename) {
    Bot::printBotInput();
    std::cout << "Starting data loading..." << std::endl;

    // Start execution measurement
    auto start = high_resolution_clock::now();

    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename, std::ios::in | std::ios::binary};
    if (!csvFile) {
        Bot::printBotInput();
        std::cout << "CSVReader::readCSV can't open file " << csvFilename << std::endl;
        exit(1);
    }
    std::string fileContent(static_cast<size_t>(std::filesystem::file_size(csvFilename)), 0);
    csvFile.read(fileContent.data(), fileContent.size());
    if (!csvFile) {
        Bot::printBotInput();
        std::cout << "CSVReader::readCSV could not read the full contents from " << csvFilename << std::endl;
        exit(1);
    }
    // Split file content to lines
    std::vector<std::string_view> lines = StringUtil::splitLines(fileContent);

    // Process lines and update entries vector.
    // Wanted to use std::execution::par here, but clang still doesn't support it.
    std::transform(lines.begin(), lines.end(),
                   std::back_inserter(entries), lineToOBE);

    // Stop execution measurement
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    Bot::printBotInput();
    std::cout << "Loaded " << entries.size() << " entries. It took " << duration.count() / 1000 << " milliseconds"
              << std::endl;
    return entries;
}

OrderBookEntry CSVReader::lineToOBE(std::string_view line) {
    return stringsToOBE(StringUtil::tokenize(line, ','));
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string_view> tokens) {
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line ";
        std::copy(tokens.begin(), tokens.end(), std::ostream_iterator<std::string_view>(std::cout, ","));
        std::cout << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
        price = std::stod(std::string(tokens[3]));
        amount = std::stod(std::string(tokens[4]));
    } catch (const std::exception &e) {
        Bot::printBotInput();
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
        Bot::printBotInput();
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe{price,
                       amount,
                       std::string(tokens[0]),
                       std::string(tokens[1]),
                       OrderBookEntry::stringToOrderBookType(std::string(tokens[2]))};

    return obe;
}
     