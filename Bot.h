#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

enum Commands {
    Invalid,
    Help,
    Exit
};

class Bot {
public:
    Bot();

    /** Call this to start the sim */
    void init();

    // Need to transform text to enum
    static Commands resolveCommand(const std::string &command);

    static void printBotInput();

private:
    static void printInputInvalid();

    static void printHelp();

    static void printHelpForCommand(Commands command);

    void printMenu();

    void printMarketStats();

    void enterAsk();

    void enterBid();

    void printWallet();

    void gotoNextTimeframe();

    static std::string getUserInput();

    static void processUserInput(const std::string &userInput);

    std::string currentTime;

    OrderBook orderBook{"20200601.csv"};
    Wallet wallet;
};
