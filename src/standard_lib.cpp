#include "standard_lib.hpp"
#include "value/value.hpp"

void print(std::vector<Value> vals)
{

    for (auto it = vals.rbegin(); it != vals.rend(); it++)
    {
        std::cout << it->to_string() << " ";
    }

    std::cout << std::endl;
}
