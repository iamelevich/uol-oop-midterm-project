#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include <set>

class OrderBook {
public:
    /** construct, reading a csv data file */
    OrderBook(const std::string &filename);

    /** return vector of all know products in the dataset */
    std::vector<std::string> getKnownProducts();

    /** check is product valid */
    bool isProductValid(const std::string &product);

    /** return vector of Orders according to the sent filters */
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                          const std::string &product,
                                          const std::string &timestamp,
                                          int check_last);

    /** returns the earliest time in the orderbook*/
    std::string getEarliestTime();

    /** returns the next time after the
      * sent time in the orderbook
      * If there is no next timestamp, wraps around to the start
      */
    std::string getNextTime(const std::string &timestamp);

    // Return next time jumping to jump size, if end of data reached - returns first timestamp
    std::string getNextTime(const std::string &timestamp, int jump);

    // Return prev timestamp before given. If no prev - return last time in orders
    std::string getPrevTime(const std::string &timestamp);

    static double getHighPrice(std::vector<OrderBookEntry> &orders);

    static double getLowPrice(std::vector<OrderBookEntry> &orders);

    static double getAvgPrice(std::vector<OrderBookEntry> &orders);

    static double getMeanPrice(std::vector<OrderBookEntry> &orders);

    static double getMeanMaxPrice(std::vector<OrderBookEntry> &orders);

    static double getMeanMinPrice(std::vector<OrderBookEntry> &orders);

    void insertOrder(OrderBookEntry &order);

private:
    std::vector<OrderBookEntry> orders;
    std::set<std::string> products;
};
