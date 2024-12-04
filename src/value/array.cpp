#include "value/value.hpp"

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

void Array::set(Value index, Value value)
{
    if (index.type != MyType::MYINT)
    {
        std::cerr << "Array index must be an integer" << std::endl;
        exit(1);
    }

    if (index.int_value < 0 || index.int_value >= elements.size())
    {
        elements.resize(index.int_value + 1, 0);
    }

    elements[index.int_value] = value;
}