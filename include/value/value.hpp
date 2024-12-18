#pragma once

#include <memory>
#include <vector>
#include "ast_nodes/ast.hpp"
#include "exceptions/return.hpp"
#include "value/callable.hpp"
#include "value/array.hpp"

enum MyType
{
    MYINT,
    MYFLOAT,
    MYSTRING,
    MYBOOL,
    MYNONE,
    MYFUNCTION,
    MYARRAY,
};

struct Value
{
    // TODO: turn this into a variant
    union
    {
        int int_value;
        float float_value;
        std::string string_value;
        bool bool_value;
        Callable *callable;
        Array *array;
    };

    MyType type;

    Value() : type(MyType::MYNONE) {}
    Value(int value) : int_value(value), type(MyType::MYINT) {}
    Value(float value) : float_value(value), type(MyType::MYFLOAT) {}
    Value(std::string value) : string_value(value), type(MyType::MYSTRING) {}
    Value(bool value) : bool_value(value), type(MyType::MYBOOL) {}
    Value(Callable *value) : callable(value), type(MyType::MYFUNCTION) {}
    Value(Array *value) : array(value), type(MyType::MYARRAY) {}

    Value(const Value &other) : type(other.type)
    {
        switch (type)
        {
        case MyType::MYINT:
            int_value = other.int_value;
            break;
        case MyType::MYFLOAT:
            float_value = other.float_value;
            break;
        case MyType::MYSTRING:
            new (&string_value) std::string(other.string_value);
            break;
        case MyType::MYBOOL:
            bool_value = other.bool_value;
            break;
        case MyType::MYFUNCTION:
            callable = other.callable;
            break;
        case MyType::MYARRAY:
            array = other.array;
            break;
        default:
            break;
        }
    }

    ~Value()
    {
        if (type == MyType::MYSTRING)
        {
            string_value.~basic_string();
        }
    }

    Value operator=(const Value &other)
    {
        if (this == &other)
        {
            return *this;
        }

        type = other.type;

        switch (type)
        {
        case MyType::MYINT:
            int_value = other.int_value;
            break;
        case MyType::MYFLOAT:
            float_value = other.float_value;
            break;
        case MyType::MYSTRING:
            new (&string_value) std::string(other.string_value);
            break;
        case MyType::MYBOOL:
            bool_value = other.bool_value;
            break;
        case MyType::MYFUNCTION:
            callable = other.callable;
            break;
        case MyType::MYARRAY:
            array = other.array;
            break;
        default:
            break;
        }

        return *this;
    }

    Value operator+(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value + other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value + other.float_value);
        }
        else if (type == MyType::MYSTRING && other.type == MyType::MYSTRING)
        {
            return Value(string_value + other.string_value);
        }
        else
        {
            std::cerr << "Invalid types for addition" << std::endl;
            exit(1);
        }
    }

    Value operator-(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value - other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value - other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for subtraction" << std::endl;
            exit(1);
        }
    }

    Value operator*(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value * other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value * other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for multiplication" << std::endl;
            exit(1);
        }
    }

    Value operator/(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value / other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value / other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for division" << std::endl;
            exit(1);
        }
    }

    Value operator%(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value % other.int_value);
        }
        else
        {
            std::cerr << "Invalid types for modulo" << std::endl;
            exit(1);
        }
    }

    Value operator==(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value == other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value == other.float_value);
        }
        else if (type == MyType::MYSTRING && other.type == MyType::MYSTRING)
        {
            return Value(string_value == other.string_value);
        }
        else if (type == MyType::MYBOOL && other.type == MyType::MYBOOL)
        {
            return Value(bool_value == other.bool_value);
        }
        else
        {
            std::cerr << "Invalid types for equality" << std::endl;
            exit(1);
        }
    }

    Value operator!=(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value != other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value != other.float_value);
        }
        else if (type == MyType::MYSTRING && other.type == MyType::MYSTRING)
        {
            return Value(string_value != other.string_value);
        }
        else if (type == MyType::MYBOOL && other.type == MyType::MYBOOL)
        {
            return Value(bool_value != other.bool_value);
        }
        else
        {
            std::cerr << "Invalid types for inequality" << std::endl;
            exit(1);
        }
    }

    Value operator<(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value < other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value < other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for less than" << std::endl;
            exit(1);
        }
    }

    Value operator<=(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value <= other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value <= other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for less than or equal" << std::endl;
            exit(1);
        }
    }

    Value operator>(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value > other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value > other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for greater than" << std::endl;
            exit(1);
        }
    }

    Value operator>=(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value >= other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value >= other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for greater than or equal" << std::endl;
            exit(1);
        }
    }

    Value operator!()
    {
        if (type == MyType::MYBOOL)
        {
            return Value(!bool_value);
        }
        else
        {
            std::cerr << "Invalid type for logical not" << std::endl;
            exit(1);
        }
    }

    Value operator-()
    {
        if (type == MyType::MYINT)
        {
            return Value(-int_value);
        }
        else if (type == MyType::MYFLOAT)
        {
            return Value(-float_value);
        }
        else
        {
            std::cerr << "Invalid type for negation" << std::endl;
            exit(1);
        }
    }

    std::string to_string()
    {
        switch (type)
        {
        case MyType::MYINT:
            return std::to_string(int_value);
        case MyType::MYFLOAT:
            return std::to_string(float_value);
        case MyType::MYSTRING:
            return string_value;
        case MyType::MYBOOL:
            return bool_value ? "true" : "false";
        case MyType::MYNONE:
            return "None";
        case MyType::MYFUNCTION:
            return "Function";
        case MyType::MYARRAY:
            return "Array";
        default:
            return "Unknown";
        }
    }
};
