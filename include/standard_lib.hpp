#pragma once
#include <vector>

struct Value;
namespace StandardLib
{
    void print(std::vector<Value> vals);
    Value range(std::vector<Value> args);
}