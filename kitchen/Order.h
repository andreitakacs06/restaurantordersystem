#pragma once

#include <iostream>
#include <string>

class Order {
public:
    Order();
    Order(int id, const std::string& product, const std::string& status);

    int getId() const;
    const std::string& getProduct() const;
    const std::string& getStatus() const;

    void setProduct(const std::string& product);
    void setStatus(const std::string& status);

private:
    int id_;
    std::string product_;
    std::string status_;
};

// Operator overload for clean display of a single order.
std::ostream& operator<<(std::ostream& os, const Order& order);
