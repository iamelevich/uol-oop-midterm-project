#include "Bot.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "StringUtil.h"

Bot::Bot() = default;

void Bot::printBotInput() {
    std::cout << "advisorbot> ";
};

[[noreturn]] void Bot::init() {
    std::string input;
    currentTime = orderBook.getEarliestTime();

    while (true) {
        input = getUserInput();
        processUserInput(input);
    }
}

// help
void Bot::printHelp() {
    std::cout << "Commands: help, help <cmd>, prod, min, max, avg, mean, predict, time, step, step <jump>, exit"
              << std::endl;
}

// help <cmd>
void Bot::printHelpForCommand(Commands command) {
    switch (command) {
        case Commands::Prod:
            std::cout << "Print list of available products. No arguments needed." << std::endl;
            break;
        case Commands::Min:
            std::cout
                    << "Print min price for product in current time."
                    << std::endl
                    << "  min <product> <type>"
                    << std::endl
                    << "    <product> - Any of available products. You can find available by prod command"
                    << std::endl
                    << "    <type> - bid/ask"
                    << std::endl
                    << "  Example: min ETH/BTC ask"
                    << std::endl;
            break;
        case Commands::Max:
            std::cout
                    << "Print max price for product in current time."
                    << std::endl
                    << "  max <product> <type>"
                    << std::endl
                    << "    <product> - Any of available products. You can find available by prod command"
                    << std::endl
                    << "    <type> - bid/ask"
                    << std::endl
                    << "  Example: max ETH/BTC ask"
                    << std::endl;
            break;
        case Commands::Avg:
            std::cout
                    << "Compute average ask or bid for the sent product over the sent number of time steps."
                    << std::endl
                    << "  avg <product> <type> <timestamps>"
                    << std::endl
                    << "    <product> - Any of available products. You can find available by prod command"
                    << std::endl
                    << "    <type> - bid/ask"
                    << std::endl
                    << "    <timestamps> - number of last timestamps to check"
                    << std::endl
                    << "  Example: avg ETH/BTC ask 10"
                    << std::endl;
            break;
        case Commands::Mean:
            std::cout
                    << "Compute mean ask or bid for the sent product over the sent number of time steps."
                    << std::endl
                    << "  mean <product> <type> <timestamps>"
                    << std::endl
                    << "    <product> - Any of available products. You can find available by prod command"
                    << std::endl
                    << "    <type> - bid/ask"
                    << std::endl
                    << "    <timestamps> - number of last timestamps to check"
                    << std::endl
                    << "  Example: mean ETH/BTC ask 10"
                    << std::endl;
            break;
        case Commands::Predict:
            std::cout
                    << "Predict max or min ask or bid for the sent product for the next time step using running median."
                    << std::endl
                    << "  predict <predict> <product> <type> <timestamps>"
                    << std::endl
                    << "    <predict> - What need to predict. min/max"
                    << std::endl
                    << "    <product> - Any of available products. You can find available by prod command"
                    << std::endl
                    << "    <type> - bid/ask"
                    << std::endl
                    << "    <timestamps> - number of last timestamps to check"
                    << std::endl
                    << "  Example: predict min ETH/BTC ask 10"
                    << std::endl;
            break;
        case Commands::Time:
            std::cout << "Print current time step. No arguments needed." << std::endl;
            break;
        case Commands::Step:
            std::cout << "Go to the next timestamp or skip some of them. Can be used with one or none arguments."
                      << std::endl
                      << "  Example: step"
                      << std::endl
                      << "    to go to the next time step"
                      << std::endl
                      << "  Example: step 10"
                      << std::endl
                      << "    to go to the 10th time step from current"
                      << std::endl;
            break;
        case Commands::Exit:
            std::cout << "Exit from the bot. No arguments needed" << std::endl;
            break;
        default:
            printInputInvalid();
    }
}

// prod
void Bot::printProducts() {
    std::vector<std::string> products = orderBook.getKnownProducts();
    for (std::string const &product: products) {
        std::cout << product << " ";
    }
    std::cout << std::endl;
}

// min
void Bot::printMin(const std::string &product, const std::string &orderType) {
    if (!orderBook.isProductValid(product)) {
        std::cout << "Invalid product. To get available products use prod command." << std::endl;
        return;
    }
    OrderBookType orderTypeEnum = OrderBookEntry::stringToOrderBookType(orderType);
    if (orderTypeEnum != OrderBookType::ask && orderTypeEnum != OrderBookType::bid) {
        std::cout << "Invalid type. Should be ask or bid." << std::endl;
        return;
    }
    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderTypeEnum, product, currentTime, 1);
    std::cout << "The min " << orderType << " for " << product << " is " << OrderBook::getLowPrice(entries)
              << std::endl;
}

// max
void Bot::printMax(const std::string &product, const std::string &orderType) {
    if (!orderBook.isProductValid(product)) {
        std::cout << "Invalid product. To get available products use prod command." << std::endl;
        return;
    }
    OrderBookType orderTypeEnum = OrderBookEntry::stringToOrderBookType(orderType);
    if (orderTypeEnum != OrderBookType::ask && orderTypeEnum != OrderBookType::bid) {
        std::cout << "Invalid type. Should be ask or bid." << std::endl;
        return;
    }
    std::vector<OrderBookEntry> entries = orderBook.getOrders(orderTypeEnum, product, currentTime, 1);
    std::cout << "The max " << orderType << " for " << product << " is " << OrderBook::getHighPrice(entries)
              << std::endl;
}

// avg
void Bot::printAvg(const std::string &product, const std::string &orderType, const std::string &periodStr) {
    if (!orderBook.isProductValid(product)) {
        std::cout << "Invalid product. To get available products use prod command." << std::endl;
        return;
    }
    OrderBookType orderTypeEnum = OrderBookEntry::stringToOrderBookType(orderType);
    if (orderTypeEnum != OrderBookType::ask && orderTypeEnum != OrderBookType::bid) {
        std::cout << "Invalid type. Should be ask or bid." << std::endl;
        return;
    }
    try {
        const int period{std::stoi(periodStr)};
        if (period < 1) {
            std::cout << "Invalid period. Should be positive integer." << std::endl;
            return;
        }
        std::vector<OrderBookEntry> entries = orderBook.getOrders(orderTypeEnum, product, currentTime, period);
        std::cout << "The average " << product << " " << orderType << " price over the last " << period
                  << " timestamps was "
                  << OrderBook::getAvgPrice(entries)
                  << std::endl;
    }
    catch (std::invalid_argument const &ex) {
        std::cout << "Invalid period. Should be positive integer." << std::endl;
    }
    catch (std::out_of_range const &ex) {
        std::cout << "Invalid period. Should be positive integer." << std::endl;
    }
}

// mean
void Bot::printMean(const std::string &product, const std::string &orderType, const std::string &periodStr) {
    if (!orderBook.isProductValid(product)) {
        std::cout << "Invalid product. To get available products use prod command." << std::endl;
        return;
    }
    OrderBookType orderTypeEnum = OrderBookEntry::stringToOrderBookType(orderType);
    if (orderTypeEnum != OrderBookType::ask && orderTypeEnum != OrderBookType::bid) {
        std::cout << "Invalid orderType. Should be ask or bid." << std::endl;
        return;
    }
    try {
        const int period{std::stoi(periodStr)};
        if (period < 1) {
            std::cout << "Invalid period. Should be positive integer." << std::endl;
            return;
        }
        std::vector<OrderBookEntry> entries = orderBook.getOrders(orderTypeEnum, product, currentTime, period);
        std::cout << "The mean " << product << " " << orderType << " price over the last " << period
                  << " timestamps was "
                  << OrderBook::getMeanPrice(entries)
                  << std::endl;
    }
    catch (std::invalid_argument const &ex) {
        std::cout << "Invalid period. Should be positive integer." << std::endl;
    }
    catch (std::out_of_range const &ex) {
        std::cout << "Invalid period. Should be positive integer." << std::endl;
    }
}

// predict
void Bot::printPredict(const std::string &product, const std::string &predictType, const std::string &orderType,
                       const std::string &periodStr) {
    if (!orderBook.isProductValid(product)) {
        std::cout << "Invalid product. To get available products use prod command." << std::endl;
        return;
    }
    OrderBookType orderTypeEnum = OrderBookEntry::stringToOrderBookType(orderType);
    if (orderTypeEnum != OrderBookType::ask && orderTypeEnum != OrderBookType::bid) {
        std::cout << "Invalid orderType. Should be ask or bid." << std::endl;
        return;
    }
    if (predictType != "min" && predictType != "max") {
        std::cout << "Invalid predictType. Should be min or max." << std::endl;
        return;
    }
    try {
        const int period{std::stoi(periodStr)};
        if (period < 1) {
            std::cout << "Invalid period. Should be positive integer." << std::endl;
            return;
        }
        std::vector<OrderBookEntry> entries = orderBook.getOrders(orderTypeEnum, product, currentTime, period);
        double prediction;
        if (predictType == "min") {
            prediction = orderBook.getMeanMinPrice(entries);
        } else {
            prediction = orderBook.getMeanMaxPrice(entries);
        }
        std::cout << "The " << predictType << " " << orderType << " for " << product
                  << " might be "
                  << prediction
                  << std::endl;
    }
    catch (std::invalid_argument const &ex) {
        std::cout << "Invalid period. Should be positive integer." << std::endl;
    }
    catch (std::out_of_range const &ex) {
        std::cout << "Invalid period. Should be positive integer." << std::endl;
    }
}

// time
void Bot::printTime() {
    std::cout << currentTime << std::endl;
}

// step
void Bot::gotoNextTimeframe() {
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at " << currentTime << std::endl;
}

// step <jump>
void Bot::gotoNextTimeframe(const std::string &jumpStr) {
    try {
        const int jump{std::stoi(jumpStr)};
        if (jump < 1) {
            std::cout << "Invalid jump. Should be positive integer." << std::endl;
            return;
        }
        currentTime = orderBook.getNextTime(currentTime, jump);
        std::cout << "now at " << currentTime << std::endl;
    }
    catch (std::invalid_argument const &ex) {
        std::cout << "Invalid jump. Should be positive integer." << std::endl;
    }
    catch (std::out_of_range const &ex) {
        std::cout << "Invalid jump. Should be positive integer." << std::endl;
    }
}

void Bot::printInputInvalid() {
    std::cout << "Don't understand. Please use help or help <cmd> to see available commands" << std::endl;
}

std::string Bot::getUserInput() {
    std::string line;
    std::cout << "user> ";
    std::getline(std::cin, line);
    return line;
}

void Bot::processUserInput(const std::string &userInput) {
    printBotInput();

    std::vector<std::string> userInputTokenized = StringUtil::tokenize(userInput, ' ');
    if (userInputTokenized.empty()) {
        printInputInvalid();
        return;
    }

    switch (resolveCommand(userInputTokenized[0])) {
        case Commands::Help:
            if (userInputTokenized.size() == 1) {
                printHelp();
                break;
            }
            if (userInputTokenized.size() == 2) {
                printHelpForCommand(resolveCommand(userInputTokenized[1]));
                break;
            }
            std::cout << "Command help can be used without arguments or only with one argument" << std::endl;
            break;
        case Commands::Prod:
            if (userInputTokenized.size() == 1) {
                printProducts();
                break;
            }
            std::cout << "Command prod should be used without arguments" << std::endl;
            break;
        case Commands::Min:
            if (userInputTokenized.size() == 3) {
                printMin(userInputTokenized[1], userInputTokenized[2]);
                break;
            }
            std::cout << "Command min should be used with 2 arguments. Example: min ETH/BTC ask" << std::endl;
            break;
        case Commands::Max:
            if (userInputTokenized.size() == 3) {
                printMax(userInputTokenized[1], userInputTokenized[2]);
                break;
            }
            std::cout << "Command max should be used with 2 arguments. Example: max ETH/BTC ask" << std::endl;
            break;
        case Commands::Avg:
            if (userInputTokenized.size() == 4) {
                printAvg(userInputTokenized[1], userInputTokenized[2], userInputTokenized[3]);
                break;
            }
            std::cout << "Command avg should be used with 3 arguments. Example: avg ETH/BTC ask 10" << std::endl;
            break;
        case Commands::Mean:
            if (userInputTokenized.size() == 4) {
                printMean(userInputTokenized[1], userInputTokenized[2], userInputTokenized[3]);
                break;
            }
            std::cout << "Command mean should be used with 3 arguments. Example: mean ETH/BTC ask 10" << std::endl;
            break;
        case Commands::Predict:
            if (userInputTokenized.size() == 5) {
                printPredict(userInputTokenized[2], userInputTokenized[1], userInputTokenized[3],
                             userInputTokenized[4]);
                break;
            }
            std::cout << "Command predict should be used with 4 arguments. Example: predict max ETH/BTC ask 10"
                      << std::endl;
            break;
        case Commands::Time:
            if (userInputTokenized.size() == 1) {
                printTime();
                break;
            }
            std::cout << "Command time should be used without arguments" << std::endl;
            break;
        case Commands::Step:
            if (userInputTokenized.size() == 1) {
                gotoNextTimeframe();
                break;
            }
            if (userInputTokenized.size() == 2) {
                gotoNextTimeframe(userInputTokenized[1]);
                break;
            }
            std::cout << "Command step should be used without arguments or with 1 argument. Example: step 5"
                      << std::endl;
            break;
        case Commands::Exit:
            std::cout << "Bye!" << std::endl;
            exit(0);
        default:
            printInputInvalid();
    }
}

Commands Bot::resolveCommand(const std::string &command) {
    if (command == "help") return Commands::Help;
    if (command == "prod") return Commands::Prod;
    if (command == "min") return Commands::Min;
    if (command == "max") return Commands::Max;
    if (command == "avg") return Commands::Avg;
    if (command == "mean") return Commands::Mean;
    if (command == "predict") return Commands::Predict;
    if (command == "time") return Commands::Time;
    if (command == "step") return Commands::Step;
    if (command == "exit") return Commands::Exit;

    return Commands::Invalid;
}
