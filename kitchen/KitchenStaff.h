#pragma once

#include <iostream>
#include <vector>

#include "Order.h"
#include "User.h"

class KitchenStaff : public User {
public:
    explicit KitchenStaff(const std::string& name);

    void viewOrders(const std::vector<Order>& orders) const;
    bool updateOrderStatus(std::vector<Order>& orders, int id, const std::string& newStatus) const;
};

template <typename T>
void displayVector(const std::vector<T>& items) {
    if (items.empty()) {
        std::cout << "No items to display.\n";
        return;
    }

    for (std::size_t i = 0; i < items.size(); ++i) {
        std::cout << items[i] << "\n";
    }
}
