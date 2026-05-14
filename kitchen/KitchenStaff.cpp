#include "KitchenStaff.h"

#include <iostream>

KitchenStaff::KitchenStaff(const std::string& name) : User(name) {}

void KitchenStaff::viewOrders(const std::vector<Order>& orders) const {
    std::cout << "Kitchen View - Orders\n";
    displayVector(orders);
}

bool KitchenStaff::updateOrderStatus(std::vector<Order>& orders, int id, const std::string& newStatus) const {
    for (std::size_t i = 0; i < orders.size(); ++i) {
        if (orders[i].getId() == id) {
            orders[i].setStatus(newStatus);
            return true;
        }
    }
    return false;
}
