#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "User.h"
#include "Order.h"

class FileManager;

// Customer inherits from User to represent a customer actor.
class Customer : public User
{
public:
    explicit Customer(const std::string &name = "Customer");

    // Create a new order object using a generated id and requested product.
    Order createOrder(int id, const std::string &product) const;

    // Place the order by writing it into the shared data file.
    bool placeOrder(class FileManager &fileManager, const std::string &product) const;
};

#endif // CUSTOMER_H
