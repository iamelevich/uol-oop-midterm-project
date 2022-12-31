#include "CSVReader.h"
#include "StringUtil.h"
#include "Bot.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <chrono>

using namespace std::chrono;

CSVReader::CSVReader() = default;

std::vector<OrderBookEntry> CSVReader::readCSV(const std::string &csvFilename) {
    Bot::printBotInput();
    std::cout << "Starting data loading..." << std::endl;

    // Start execution measurement
    auto start = high_resolution_clock::now();

    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open()) {
        while (std::getline(csvFile, line)) {
            try {
                OrderBookEntry obe = stringsToOBE(StringUtil::tokenize(line, ','));
                entries.push_back(obe);
            } catch (const std::exception &e) {
                Bot::printBotInput();
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            }
        }// end of while
    } else {
        Bot::printBotInput();
        std::cout << "CSVReader::readCSV can't open file " << csvFilename << std::endl;
        exit(1);
    }

    // Stop execution measurement
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    Bot::printBotInput();
    std::cout << "Loaded " << entries.size() << " entries. It took " << duration.count() / 1000 << " milliseconds"
              << std::endl;
    return entries;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens) {
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line ";
        std::copy(tokens.begin(), tokens.end(), std::ostream_iterator<std::string>(std::cout, ","));
        std::cout << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    } catch (const std::exception &e) {
        Bot::printBotInput();
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
        Bot::printBotInput();
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }

    OrderBookEntry obe{price,
                       amount,
                       tokens[0],
                       tokens[1],
                       OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe;
}


OrderBookEntry CSVReader::stringsToOBE(const std::string &priceString,
                                       const std::string &amountString,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType orderType) {
    double price, amount;
    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    } catch (const std::exception &e) {
        Bot::printBotInput();
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        Bot::printBotInput();
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }
    OrderBookEntry obe{price,
                       amount,
                       std::move(timestamp),
                       std::move(product),
                       orderType};

    return obe;
}
     