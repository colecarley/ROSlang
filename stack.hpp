#pragma once

template <typename T>
struct Stack
{
    std::vector<T> stack;

    void push(T item)
    {
        stack.push_back(std::move(item));
    }

    T pop()
    {
        T item = std::move(stack.back());
        stack.pop_back();
        return std::move(item);
    }
};