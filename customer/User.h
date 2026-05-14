#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    std::string name_;

public:
    explicit User(const std::string &name);
    virtual ~User();

    std::string getName() const;
    void setName(const std::string &name);
};

#endif
