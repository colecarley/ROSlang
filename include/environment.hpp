#pragma once

template <typename T>
struct Environment
{
    std::vector<std::unordered_map<std::string, T>> env;

    void push_env()
    {
        env.push_back(std::unordered_map<std::string, T>());
    }

    void pop_env()
    {
        env.pop_back();
    }

    void set(std::string key, T value)
    {
        for (auto it = env.rbegin(); it != env.rend(); ++it)
        {
            if (it->find(key) != it->end())
            {
                it->at(key) = value;
                return;
            }
        }

        env.back()[key] = value;
    }

    bool contains(std::string key)
    {
        for (auto it = env.rbegin(); it != env.rend(); ++it)
        {
            if (it->find(key) != it->end())
            {
                return true;
            }
        }

        return false;
    }

    T get(std::string key)
    {
        for (auto it = env.rbegin(); it != env.rend(); ++it)
        {
            auto value = it->find(key);
            if (value != it->end())
            {
                return value->second;
            }
        }

        return T();
    }
};
