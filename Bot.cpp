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

void Bot::init() {
    std::string input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true) {
//        printMenu();
        input = getUserInput();
        processUserInput(input);
    }
}

// help
void Bot::printHelp() {
    std::cout << "The available commands are help, help <cmd>, prod, min, max, avg, predict, time, step, exit"
              << std::endl;
}

// help <cmd>
void Bot::printHelpForCommand(Commands command) {
    switch (command) {
        case Commands::Exit:
            std::cout << "Exit from the bot. No arguments needed" << std::endl;
            break;
        default:
            printInputInvalid();
    }
}

void Bot::printMenu() {
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}


void Bot::printMarketStats() {
    for (std::string const &p: orderBook.getKnownProducts()) {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void Bot::enterAsk() {
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = StringUtil::tokenize(input, ',');
    if (tokens.size() != 3) {
        std::cout << "Bot::enterAsk Bad input! " << input << std::endl;
    } else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    currentTime,
                    tokens[0],
                    OrderBookType::ask
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe)) {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            } else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        } catch (const std::exception &e) {
            std::cout << " Bot::enterAsk Bad input " << std::endl;
        }
    }
}

void Bot::enterBid() {
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = StringUtil::tokenize(input, ',');
    if (tokens.size() != 3) {
        std::cout << "Bot::enterBid Bad input! " << input << std::endl;
    } else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    currentTime,
                    tokens[0],
                    OrderBookType::bid
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe)) {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            } else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        } catch (const std::exception &e) {
            std::cout << " Bot::enterBid Bad input " << std::endl;
        }
    }
}

void Bot::printWallet() {
    std::cout << wallet.toString() << std::endl;
}

void Bot::gotoNextTimeframe() {
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p: orderBook.getKnownProducts()) {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale: sales) {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser") {
                // update the wallet
                wallet.processSale(sale);
            }
        }

    }

    currentTime = orderBook.getNextTime(currentTime);
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
            } else if (userInputTokenized.size() == 2) {
                printHelpForCommand(resolveCommand(userInputTokenized[1]));
            } else {
                std::cout << "Command help can be used without arguments or only with one argument" << std::endl;
            }
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
    if (command == "exit") return Commands::Exit;

    return Commands::Invalid;
}
