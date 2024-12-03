#include "standard_lib.hpp"
#include "value.hpp"

void print(std::vector<Value> vals)
{

    for (auto val : vals)
    {
        std::cout << val.to_string() << " ";
    }

    std::cout << std::endl;
}
