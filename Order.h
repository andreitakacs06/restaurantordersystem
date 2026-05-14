#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ostream>

// Represents a single customer order in the restaurant system.
class Order
{
private:
    int id_;              // Unique order identifier
    std::string product_; // The product name ordered by the customer
    std::string status_;  // Current status (Pending, Ready, etc.)

public:
    Order();
    Order(int id, const std::string &product, const std::string &status);

    // Getters and setters for encapsulation.
    int getId() const;
    void setId(int id);

    std::string getProduct() const;
    void setProduct(const std::string &product);

    std::string getStatus() const;
    void setStatus(const std::string &status);

    // Format the order for display.
    friend std::ostream &operator<<(std::ostream &os, const Order &order);
};

#endif // ORDER_H
