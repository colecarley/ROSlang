#include "standard_lib.hpp"
#include "value/value.hpp"

void StandardLib::print(std::vector<Value> vals)
{

    for (auto it = vals.rbegin(); it != vals.rend(); it++)
    {
        std::cout << it->to_string() << " ";
    }

    std::cout << std::endl;
}

Value StandardLib::range(std::vector<Value> args)
{
    if (args.size() == 1)
    {
        if (args[0].type == MyType::MYINT)
        {
            std::vector<Value> range;
            for (int i = 0; i < args[0].int_value; i++)
            {
                range.push_back(Value(i));
            }
            return Value(new Array(range));
        }
        else
        {
            std::cerr << "Expected integer argument to range" << std::endl;
            exit(1);
        }
    }
    else if (args.size() == 2)
    {
        if (args[0].type == MyType::MYINT && args[1].type == MyType::MYINT)
        {
            std::vector<Value> range;
            for (int i = args[0].int_value; i < args[1].int_value; i++)
            {
                range.push_back(Value(i));
            }
            return Value(new Array(range));
        }
        else
        {
            std::cerr << "Expected integer arguments to range" << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cerr << "Expected 1 or 2 arguments to range" << std::endl;
        exit(1);
    }
}