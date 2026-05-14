#ifndef USER_H
#define USER_H

#include <string>

// Base class for all users in the restaurant ordering system.
class User
{
private:
    std::string name_; // User name or customer name

public:
    explicit User(const std::string &name);
    virtual ~User();

    std::string getName() const;
    void setName(const std::string &name);
};

#endif // USER_H
