#include <iostream>
#include <string>
#include "inc/ordered_list.hpp"

class Test
{
public:
    int t;
    Test(int t)
        :t(t)
    {
        std::cout << "constructor: " << this->t << std::endl;
    }

    Test(const Test& o)
        :t(o.t)
    {
        std::cout << "copy constructor: " << this->t << std::endl;
    }

    ~Test()
    {
        std::cout << "destructor: " << this->t << std::endl;
    }

    bool operator==(const Test& other) const
    {
        return t == other.t;
    }

    bool operator<(const Test& other) const
    {
        return t < other.t;
    }

    bool operator>(const Test& other) const
    {
        return t > other.t;
    }

    bool operator>=(const Test& other) const
    {
        return t >= other.t;
    }

    bool operator<=(const Test& other) const
    {
        return t <= other.t;
    }

    bool operator!=(const Test& other) const
    {
        return t != other.t;
    }

};

int main()
{
    fs::Ordered_list<Test*> ol;
    ol.insert(new Test(2));
    ol.insert(new Test(23));
    ol.insert(new Test(-23));

    for (auto tmp = ol.head(); tmp; tmp = tmp->next())
    {
        std::cout << tmp->data()->t << std::endl;
        delete tmp->data();
    }
    return 0;
}
