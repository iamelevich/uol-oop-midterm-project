#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"

enum Commands {
    Invalid,
    Help,
    Prod,
    Min,
    Max,
    Avg,
    Mean,
    Predict,
    Time,
    Step,
    Exit
};

class Bot {
public:
    Bot();

    /** Call this to start the sim */
    [[noreturn]] void init();

    // Need to transform text to enum
    static Commands resolveCommand(const std::string &command);

    static void printBotInput();

private:
    static void printInputInvalid();

    static void printHelp();

    static void printHelpForCommand(Commands command);

    void printProducts();

    void printTime();

    void printMin(const std::string &product, const std::string &orderType);

    void printMax(const std::string &product, const std::string &orderType);

    void printAvg(const std::string &product, const std::string &orderType, const std::string &periodStr);

    void printMean(const std::string &product, const std::string &orderType, const std::string &periodStr);

    void printPredict(const std::string &product, const std::string &predictionType, const std::string &orderType,
                      const std::string &periodStr);

    void gotoNextTimeframe();

    void gotoNextTimeframe(const std::string &jumpStr);

    static std::string getUserInput();

    void processUserInput(const std::string &userInput);

    std::string currentTime;

    OrderBook orderBook{"20200601.csv"};
};
