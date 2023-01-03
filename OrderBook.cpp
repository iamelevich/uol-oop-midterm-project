#include "OrderBook.h"
#include "CSVReader.h"
#include "MathHelper.h"
#include <map>
#include <algorithm>

/** construct, reading a csv data file */
OrderBook::OrderBook(const std::string &filename) {
    orders = CSVReader::readCSV(filename);

    // Sort order to be sure that time order is right
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);

    // Fill products set
    for (OrderBookEntry &order: orders) {
        products.insert(order.product);
    }
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts() {
    std::vector<std::string> productsVector(products.begin(), products.end());
    return productsVector;
}

/** check is product valid or not */
bool OrderBook::isProductValid(const std::string &product) {
    return products.find(product) != products.end();
}

/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 const std::string &product,
                                                 const std::string &timestamp,
                                                 int check_last) {
    std::string current_timestamp = timestamp;
    bool is_range_started = false;
    std::vector<OrderBookEntry> orders_sub;
    // Loop in reverse order
    for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
        OrderBookEntry e = *it;
        // Just continue if this is wrong time
        if (!is_range_started && e.timestamp != current_timestamp) {
            continue;
        }
        // Start range if it's not started and timestamp is equal to current
        if (!is_range_started && e.timestamp == current_timestamp) {
            is_range_started = true;
        }
        // Change current_timestamp if we should check more than one
        if (is_range_started && e.timestamp != current_timestamp && check_last > 0) {
            current_timestamp = e.timestamp;
            check_last--;
        }
        // Stop checking if we went out of range
        if (is_range_started && e.timestamp != current_timestamp && check_last < 1) {
            break;
        }
        // Add order to vector if it fits filters
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == current_timestamp) {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

// Return average price
double OrderBook::getAvgPrice(std::vector<OrderBookEntry> &orders) {
    if (orders.empty()) return 0.0;
    double sum = orders[0].price;
    for (OrderBookEntry &e: orders) {
        sum += e.price;
    }
    return sum / (double) orders.size();
}

// Return mean price
double OrderBook::getMeanPrice(std::vector<OrderBookEntry> &orders) {
    if (orders.empty()) return 0.0;
    std::vector<double> prices = {};
    for (OrderBookEntry &e: orders) {
        prices.push_back(e.price);
    }
    return MathHelper::mean(prices);
}

// Return mean of max prices in ord
double OrderBook::getMeanMaxPrice(std::vector<OrderBookEntry> &orders) {
    if (orders.empty()) return 0.0;
    std::vector<double> prices = {};
    std::string current_timestamp = orders[0].timestamp;
    double current_max = orders[0].price;
    for (OrderBookEntry &e: orders) {
        // If this is next timestamp - push previous max to prices
        if (e.timestamp != current_timestamp) {
            prices.push_back(current_max);
            current_max = e.price;
            current_timestamp = e.timestamp;
            continue;
        }
        if (e.price > current_max) current_max = e.price;
    }
    // Add last max price of timestamp
    prices.push_back(current_max);

    return MathHelper::mean(prices);
}

// Return mean price
double OrderBook::getMeanMinPrice(std::vector<OrderBookEntry> &orders) {
    if (orders.empty()) return 0.0;
    std::vector<double> prices = {};
    std::string current_timestamp = orders[0].timestamp;
    double current_min = orders[0].price;
    for (OrderBookEntry &e: orders) {
        // If this is next timestamp - push previous max to prices
        if (e.timestamp != current_timestamp) {
            prices.push_back(current_min);
            current_min = e.price;
            current_timestamp = e.timestamp;
            continue;
        }
        if (e.price < current_min) current_min = e.price;
    }
    // Add last min price of timestamp
    prices.push_back(current_min);

    return MathHelper::mean(prices);
}

// Return min price in provided orders
double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders) {
    if (orders.empty()) return 0.0;
    double max = orders[0].price;
    for (OrderBookEntry &e: orders) {
        if (e.price > max)max = e.price;
    }
    return max;
}

// Return min price in provided orders
double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders) {
    if (orders.empty()) return 0.0;
    double min = orders[0].price;
    for (OrderBookEntry &e: orders) {
        if (e.price < min)min = e.price;
    }
    return min;
}

// Return first timestamp of the orders
std::string OrderBook::getEarliestTime() {
    return orders[0].timestamp;
}

// Return next timestamp from orders. If no next - return first one.
std::string OrderBook::getPrevTime(const std::string &timestamp) {
    std::string prev_timestamp;
    for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
        OrderBookEntry e = *it;
        if (e.timestamp < timestamp) {
            prev_timestamp = e.timestamp;
            break;
        }
    }
    if (prev_timestamp.empty()) {
        prev_timestamp = orders.back().timestamp;
    }
    return prev_timestamp;
}

// Return next timestamp from orders. If no next - return first one.
std::string OrderBook::getNextTime(const std::string &timestamp) {
    std::string next_timestamp;
    for (OrderBookEntry &e: orders) {
        if (e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp.empty()) {
        next_timestamp = getEarliestTime();
    }
    return next_timestamp;
}

// Return next timestamp from orders. If no next - return first one.
std::string OrderBook::getNextTime(const std::string &timestamp, int jump) {
    std::string next_timestamp = timestamp;
    for (OrderBookEntry &e: orders) {
        if (e.timestamp > next_timestamp) {
            next_timestamp = e.timestamp;
            jump--;
        }
        if (jump < 1) {
            break;
        }
    }
    if (next_timestamp.empty()) {
        next_timestamp = getEarliestTime();
    }
    return next_timestamp;
}
