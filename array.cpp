#include "value.hpp"

Value Array::operator[](int index)
{
    return elements[index];
}

Value Array::operator[](Value index)
{
    if (index.type != MyType::MYINT)
    {
        std::cerr << "Array index must be an integer" << std::endl;
        exit(1);
    }

    return elements[index.int_value];
}