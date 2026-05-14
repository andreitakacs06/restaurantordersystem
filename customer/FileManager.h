#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "Order.h"

class FileManager
{
private:
    std::string filename_;
    std::vector<Order> orders_; // FileManager manages a collection of Order objects.

public:
    explicit FileManager(const std::string &filename);

    // Load orders from the shared file into the internal vector.
    bool loadOrders();

    // Append a new order record to the shared file.
    bool appendOrder(const Order &order);

    // Return the current vector of orders.
    const std::vector<Order> &getOrders() const;

    // Generate the next order ID based on loaded orders.
    int getNextOrderId() const;
};

// Template utility function to display items from any printable container.
template <typename T>
void displayContainerItems(const std::vector<T> &items)
{
    for (const auto &item : items)
    {
        std::cout << item << std::endl;
    }
}

#endif // FILEMANAGER_H
