#pragma once
#include <vector>

struct Value;
struct Array
{
    std::vector<Value> elements;

    Array(std::vector<Value> elements) : elements(elements) {}

    Value operator[](int index);

    Value operator[](Value index);

    void set(Value index, Value value);
};
