#include "Customer.h"
#include "FileManager.h"

Customer::Customer(const std::string &name)
    : User(name)
{
}

Order Customer::createOrder(int id, const std::string &product) const
{
    Order order(id, product, "Pending");
    return order;
}

bool Customer::placeOrder(FileManager &fileManager, const std::string &product) const
{
    int nextId = fileManager.getNextOrderId();
    Order newOrder = createOrder(nextId, product);
    return fileManager.appendOrder(newOrder);
}
