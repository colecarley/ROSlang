#pragma once
#include <memory>
#include <vector>

namespace DHTT
{
    struct Node
    {
        virtual ~Node() = default;
        std::vector<std::shared_ptr<Node>> children;

        void add(std::shared_ptr<Node> child)
        {
            children.push_back(std::move(child));
        }
    };

    struct And : Node
    {
    };

    struct Or : Node
    {
    };

    struct Then : Node
    {
    };

    struct Behavior : Node // always a leaf node
    {
        std::string identifier;
        std::vector<std::string> args;

        Behavior(std::string identifier, std::vector<std::string> args) : identifier(identifier), args(args) {}
    };

    struct Pseudo : Node
    {
    };
};